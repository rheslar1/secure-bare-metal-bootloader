#include "bootloader/Bootloader.hpp"

#include <cassert>
#include <sstream>
#include <string>

namespace {

bool contains(const std::string& value, const std::string& needle) {
  return value.find(needle) != std::string::npos;
}

bootloader::BootDecision decide(bootloader::ImageHeader slotA,
                                bootloader::ImageHeader slotB,
                                bootloader::BootState state = {12U, 3U, false, false}) {
  bootloader::SimulatedEcdsaSha256Verifier verifier;
  bootloader::SecureBootloader loader(
      bootloader::stm32FlashLayout(), state, verifier);
  return loader.decide(slotA, slotB);
}

void selectsPendingSignedUpdate() {
  const auto decision =
      decide(bootloader::factoryImage(), bootloader::pendingUpdateImage());

  assert(decision.accepted);
  assert(decision.action == bootloader::BootAction::BootSlotB);
  assert(decision.selectedSlot == "slot_b");
  assert(contains(decision.reason, "1.3.0"));
}

void fallsBackWhenPendingSignatureFails() {
  auto update = bootloader::pendingUpdateImage();
  update.signature = "ECDSA-P256:BAD";
  const auto decision = decide(bootloader::factoryImage(), update);

  assert(decision.accepted);
  assert(decision.action == bootloader::BootAction::BootSlotA);
  assert(contains(decision.reason, "fallback"));
}

void rejectsRollbackCounterAndEntersDfuWithoutFallback() {
  auto factory = bootloader::factoryImage();
  auto update = bootloader::pendingUpdateImage();
  factory.rollbackCounter = 1U;
  update.rollbackCounter = 2U;
  const auto decision = decide(factory, update, {10U, 3U, false, false});

  assert(decision.accepted);
  assert(decision.action == bootloader::BootAction::EnterDfu);
  assert(contains(decision.reason, "DFU"));
}

void honorsOperatorDfuRequest() {
  const auto decision = decide(bootloader::factoryImage(),
                               bootloader::pendingUpdateImage(),
                               {12U, 3U, true, false});

  assert(decision.accepted);
  assert(decision.action == bootloader::BootAction::EnterDfu);
  assert(decision.events.size() == 3U);
}

void tamperLatchHaltsBoot() {
  const auto decision = decide(bootloader::factoryImage(),
                               bootloader::pendingUpdateImage(),
                               {12U, 3U, false, true});

  assert(!decision.accepted);
  assert(decision.action == bootloader::BootAction::Halt);
  assert(contains(decision.reason, "tamper"));
}

void reporterIncludesDiagnostics() {
  const auto decision =
      decide(bootloader::factoryImage(), bootloader::pendingUpdateImage());
  std::ostringstream output;
  bootloader::TextBootReporter reporter(output);
  reporter.publish(decision);

  assert(contains(output.str(), "BOOT_SLOT_B"));
  assert(contains(output.str(), "slot_b-signature"));
}

}  // namespace

int main() {
  selectsPendingSignedUpdate();
  fallsBackWhenPendingSignatureFails();
  rejectsRollbackCounterAndEntersDfuWithoutFallback();
  honorsOperatorDfuRequest();
  tamperLatchHaltsBoot();
  reporterIncludesDiagnostics();
  return 0;
}
