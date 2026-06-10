#ifndef BOOTLOADER_BOOTLOADER_HPP_
#define BOOTLOADER_BOOTLOADER_HPP_

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

namespace bootloader {

enum class CheckStatus {
  Pass,
  Fail
};

enum class BootAction {
  BootSlotA,
  BootSlotB,
  EnterDfu,
  Halt
};

enum class DfuCommand {
  None,
  EnterDfu,
  EraseScratch,
  AcceptPending
};

std::string toString(CheckStatus status);
std::string toString(BootAction action);
std::string toString(DfuCommand command);

struct FlashRegion {
  std::string name;
  std::uint32_t address{};
  std::uint32_t sizeBytes{};
};

struct FlashLayout {
  FlashRegion bootloader;
  FlashRegion slotA;
  FlashRegion slotB;
  FlashRegion scratch;
  std::uint32_t minimumEraseBytes{4096};
};

struct ImageHeader {
  std::string slot;
  std::string version;
  std::uint32_t vectorAddress{};
  std::uint32_t imageSizeBytes{};
  std::uint32_t resetHandlerAddress{};
  std::string sha256;
  std::string signature;
  std::string signerKeyId;
  std::uint32_t rollbackCounter{};
  bool signaturePresent{};
  bool manifestPresent{true};
  bool confirmed{};
  bool pending{};
  std::uint8_t bootAttempts{};
};

struct BootState {
  std::uint32_t deviceRollbackCounter{};
  std::uint8_t maxPendingAttempts{3};
  bool dfuRequested{};
  bool tamperLatched{};
  bool scratchDirty{};
  bool pendingStateWriteLocked{};
  DfuCommand dfuCommand{DfuCommand::None};
};

struct BootEvent {
  std::string step;
  CheckStatus status{CheckStatus::Fail};
  std::string detail;
};

struct BootHandoffPlan {
  std::uint32_t vectorTableAddress{};
  std::uint32_t resetHandlerAddress{};
  bool interruptsMasked{};
  bool mspLoaded{};
};

struct BootDecision {
  bool accepted{};
  BootAction action{BootAction::Halt};
  std::string selectedSlot;
  std::string reason;
  BootHandoffPlan handoff;
  BootState nextState;
  std::vector<BootEvent> events;
};

class IImageVerifier {
 public:
  virtual ~IImageVerifier() = default;
  virtual bool verify(const ImageHeader& image, std::string& reason) const = 0;
};

class SecureBootloader {
 public:
  SecureBootloader(FlashLayout layout,
                   BootState state,
                   const IImageVerifier& verifier);

  BootDecision decide(const ImageHeader& slotA,
                      const ImageHeader& slotB) const;

 private:
  FlashLayout layout_;
  BootState state_;
  const IImageVerifier& verifier_;
};

class SimulatedEcdsaSha256Verifier final : public IImageVerifier {
 public:
  bool verify(const ImageHeader& image, std::string& reason) const override;
};

class TextBootReporter {
 public:
  explicit TextBootReporter(std::ostream& stream);

  void publish(const BootDecision& decision) const;

 private:
  std::ostream& stream_;
};

FlashLayout stm32FlashLayout();
ImageHeader factoryImage();
ImageHeader pendingUpdateImage();

}  // namespace bootloader

#endif  // BOOTLOADER_BOOTLOADER_HPP_
