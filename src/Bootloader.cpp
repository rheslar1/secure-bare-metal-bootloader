#include "bootloader/Bootloader.hpp"

#include <algorithm>
#include <cctype>
#include <ostream>
#include <string_view>
#include <utility>

namespace bootloader {
namespace {

bool isHexDigest(std::string_view value) {
  return value.size() == 64U &&
         std::all_of(value.begin(), value.end(), [](const char item) {
           return std::isxdigit(static_cast<unsigned char>(item)) != 0;
         });
}

void addEvent(BootDecision& decision,
              std::string step,
              const bool passed,
              std::string detail) {
  decision.events.push_back(
      BootEvent{std::move(step),
                passed ? CheckStatus::Pass : CheckStatus::Fail,
                std::move(detail)});
}

bool contains(const FlashRegion& region,
              const std::uint32_t address,
              const std::uint32_t sizeBytes) {
  const std::uint64_t start = region.address;
  const std::uint64_t end = start + region.sizeBytes;
  const std::uint64_t imageStart = address;
  const std::uint64_t imageEnd = imageStart + sizeBytes;
  return imageStart >= start && imageEnd <= end && sizeBytes > 0U;
}

const FlashRegion& regionForSlot(const FlashLayout& layout, std::string_view slot) {
  return slot == "slot_a" ? layout.slotA : layout.slotB;
}

BootAction actionForSlot(std::string_view slot) {
  return slot == "slot_a" ? BootAction::BootSlotA : BootAction::BootSlotB;
}

bool layoutValid(const FlashLayout& layout, std::string& reason) {
  if (layout.bootloader.sizeBytes == 0U || layout.slotA.sizeBytes == 0U ||
      layout.slotB.sizeBytes == 0U || layout.scratch.sizeBytes == 0U) {
    reason = "all flash regions must have non-zero size";
    return false;
  }

  const bool ordered = layout.bootloader.address < layout.slotA.address &&
                       layout.slotA.address < layout.slotB.address &&
                       layout.slotB.address < layout.scratch.address;
  if (!ordered) {
    reason = "flash regions must be ordered bootloader, slot_a, slot_b, scratch";
    return false;
  }

  if (layout.minimumEraseBytes == 0U ||
      (layout.slotA.address % layout.minimumEraseBytes) != 0U ||
      (layout.slotB.address % layout.minimumEraseBytes) != 0U) {
    reason = "slot addresses must align to erase size";
    return false;
  }

  reason = "flash layout aligned with two application slots";
  return true;
}

bool imageAllowedInSlot(const FlashLayout& layout,
                        const ImageHeader& image,
                        std::string& reason) {
  if (image.slot != "slot_a" && image.slot != "slot_b") {
    reason = "image slot must be slot_a or slot_b";
    return false;
  }

  const auto& region = regionForSlot(layout, image.slot);
  if (!contains(region, image.vectorAddress, image.imageSizeBytes)) {
    reason = image.slot + " image does not fit inside flash partition";
    return false;
  }

  reason = image.slot + " fits partition " + region.name;
  return true;
}

bool imageBootable(const FlashLayout& layout,
                   const BootState& state,
                   const IImageVerifier& verifier,
                   const ImageHeader& image,
                   BootDecision& decision) {
  std::string reason;
  const bool fits = imageAllowedInSlot(layout, image, reason);
  addEvent(decision, image.slot + "-layout", fits, reason);
  if (!fits) {
    return false;
  }

  const bool signedOk = verifier.verify(image, reason);
  addEvent(decision, image.slot + "-signature", signedOk, reason);
  if (!signedOk) {
    return false;
  }

  const bool rollbackOk = image.rollbackCounter >= state.deviceRollbackCounter;
  reason = rollbackOk ? "rollback counter accepted"
                      : "rollback counter " +
                            std::to_string(image.rollbackCounter) +
                            " below device counter " +
                            std::to_string(state.deviceRollbackCounter);
  addEvent(decision, image.slot + "-rollback", rollbackOk, reason);
  if (!rollbackOk) {
    return false;
  }

  if (image.pending && image.bootAttempts >= state.maxPendingAttempts) {
    addEvent(decision,
             image.slot + "-attempts",
             false,
             "pending image exceeded trial boot attempts");
    return false;
  }

  addEvent(decision,
           image.slot + "-attempts",
           true,
           image.pending ? "pending image still inside trial limit"
                         : "confirmed image does not consume trial attempts");
  return image.confirmed || image.pending;
}

}  // namespace

std::string toString(const CheckStatus status) {
  return status == CheckStatus::Pass ? "PASS" : "FAIL";
}

std::string toString(const BootAction action) {
  switch (action) {
    case BootAction::BootSlotA:
      return "BOOT_SLOT_A";
    case BootAction::BootSlotB:
      return "BOOT_SLOT_B";
    case BootAction::EnterDfu:
      return "ENTER_DFU";
    case BootAction::Halt:
      return "HALT";
  }
  return "UNKNOWN";
}

SecureBootloader::SecureBootloader(FlashLayout layout,
                                   BootState state,
                                   const IImageVerifier& verifier)
    : layout_(std::move(layout)), state_(state), verifier_(verifier) {}

BootDecision SecureBootloader::decide(const ImageHeader& slotA,
                                      const ImageHeader& slotB) const {
  BootDecision decision;

  auto finish = [&](const bool accepted,
                    const BootAction action,
                    std::string slot,
                    std::string reason) {
    decision.accepted = accepted;
    decision.action = action;
    decision.selectedSlot = std::move(slot);
    decision.reason = std::move(reason);
    return decision;
  };

  std::string reason;
  const bool layoutOk = layoutValid(layout_, reason);
  addEvent(decision, "flash-layout", layoutOk, reason);
  if (!layoutOk) {
    return finish(false, BootAction::Halt, "", reason);
  }

  if (state_.tamperLatched) {
    addEvent(decision, "tamper-gate", false, "tamper latch is set");
    return finish(false, BootAction::Halt, "", "tamper latch is set");
  }
  addEvent(decision, "tamper-gate", true, "tamper latch clear");

  if (state_.dfuRequested) {
    addEvent(decision, "dfu-request", true, "operator requested DFU mode");
    return finish(true, BootAction::EnterDfu, "", "DFU requested");
  }

  const ImageHeader* preferred = nullptr;
  if (slotA.pending && !slotB.pending) {
    preferred = &slotA;
  } else if (slotB.pending && !slotA.pending) {
    preferred = &slotB;
  } else {
    preferred = slotB.rollbackCounter > slotA.rollbackCounter ? &slotB : &slotA;
  }

  const ImageHeader* fallback = preferred->slot == slotA.slot ? &slotB : &slotA;

  if (imageBootable(layout_, state_, verifier_, *preferred, decision)) {
    return finish(true,
                  actionForSlot(preferred->slot),
                  preferred->slot,
                  "selected " + preferred->slot + " version " +
                      preferred->version);
  }

  addEvent(decision,
           "fallback-policy",
           true,
           "preferred slot rejected, checking alternate slot");
  if (imageBootable(layout_, state_, verifier_, *fallback, decision) &&
      fallback->confirmed) {
    return finish(true,
                  actionForSlot(fallback->slot),
                  fallback->slot,
                  "fallback to confirmed " + fallback->slot + " version " +
                      fallback->version);
  }

  addEvent(decision, "dfu-fallback", true, "no bootable image found");
  return finish(true, BootAction::EnterDfu, "", "enter DFU recovery");
}

bool SimulatedEcdsaSha256Verifier::verify(const ImageHeader& image,
                                          std::string& reason) const {
  if (!image.signaturePresent) {
    reason = "image signature missing";
    return false;
  }

  if (!isHexDigest(image.sha256)) {
    reason = "sha256 digest is not a 64-character hex value";
    return false;
  }

  if (image.signature != "ECDSA-P256:VALID") {
    reason = "ECDSA P-256 signature check failed";
    return false;
  }

  reason = "ECDSA P-256 signature and SHA-256 digest accepted";
  return true;
}

TextBootReporter::TextBootReporter(std::ostream& stream) : stream_(stream) {}

void TextBootReporter::publish(const BootDecision& decision) const {
  stream_ << "boot=" << (decision.accepted ? "PASS" : "FAIL")
          << " action=" << toString(decision.action)
          << " selected=\"" << decision.selectedSlot << "\" reason=\""
          << decision.reason << "\"\n";
  for (const auto& event : decision.events) {
    stream_ << "  [" << toString(event.status) << "] " << event.step << ": "
            << event.detail << '\n';
  }
}

FlashLayout stm32FlashLayout() {
  return FlashLayout{
      FlashRegion{"bootloader", 0x08000000U, 64U * 1024U},
      FlashRegion{"slot_a", 0x08010000U, 448U * 1024U},
      FlashRegion{"slot_b", 0x08080000U, 448U * 1024U},
      FlashRegion{"scratch", 0x080F0000U, 64U * 1024U},
      4096U};
}

ImageHeader factoryImage() {
  return ImageHeader{
      "slot_a",
      "1.2.0",
      0x08010000U,
      320U * 1024U,
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef",
      "ECDSA-P256:VALID",
      12U,
      true,
      true,
      false,
      0U};
}

ImageHeader pendingUpdateImage() {
  return ImageHeader{
      "slot_b",
      "1.3.0",
      0x08080000U,
      340U * 1024U,
      "3b7e1f0c9d4a5b682817263544e5f60718c9aabbccddeeff0011223344556677",
      "ECDSA-P256:VALID",
      13U,
      true,
      false,
      true,
      1U};
}

}  // namespace bootloader
