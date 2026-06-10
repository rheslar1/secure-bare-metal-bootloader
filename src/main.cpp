#include "bootloader/Bootloader.hpp"

#include <iostream>
#include <string>

namespace {

void printUsage(const char* programName) {
  std::cout << "Usage: " << programName << " [mode]\n\n"
            << "Modes:\n"
            << "  --normal             Select the signed pending slot-B update\n"
            << "  --bad-signature      Fall back to confirmed slot A\n"
            << "  --rollback           Reject both images and enter DFU\n"
            << "  --dfu                Enter operator DFU mode\n"
            << "  --tamper             Halt because tamper latch is set\n"
            << "  --dirty-scratch      Enter DFU recovery after interrupted swap\n"
            << "  --locked-state       Halt because boot-state storage is locked\n"
            << "  --help               Show this help text\n";
}

}  // namespace

int main(int argc, char** argv) {
  const std::string mode = argc > 1 ? argv[1] : "--normal";
  if (mode == "--help") {
    printUsage(argv[0]);
    return 0;
  }

  bootloader::BootState state{12U, 3U, false, false};
  auto factory = bootloader::factoryImage();
  auto update = bootloader::pendingUpdateImage();

  if (mode == "--bad-signature") {
    update.signature = "ECDSA-P256:BAD";
  } else if (mode == "--rollback") {
    factory.rollbackCounter = 1U;
    update.rollbackCounter = 2U;
  } else if (mode == "--dfu") {
    state.dfuCommand = bootloader::DfuCommand::EnterDfu;
  } else if (mode == "--tamper") {
    state.tamperLatched = true;
  } else if (mode == "--dirty-scratch") {
    state.scratchDirty = true;
  } else if (mode == "--locked-state") {
    state.pendingStateWriteLocked = true;
  } else if (mode != "--normal") {
    printUsage(argv[0]);
    return 1;
  }

  bootloader::SimulatedEcdsaSha256Verifier verifier;
  bootloader::SecureBootloader bootloader(bootloader::stm32FlashLayout(), state, verifier);
  bootloader::TextBootReporter reporter(std::cout);

  std::cout << "Secure Bare-Metal Bootloader\n";
  std::cout << "Flash model: dual slot + scratch, ECDSA P-256 + SHA-256\n\n";

  const auto decision = bootloader.decide(factory, update);
  reporter.publish(decision);
  return decision.accepted ? 0 : 1;
}
