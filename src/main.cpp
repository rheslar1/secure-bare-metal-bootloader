#include "bootloader/Bootloader.hpp"

#include <iostream>

int main() {
  bootloader::SimulatedEcdsaSha256Verifier verifier;
  bootloader::SecureBootloader bootloader(
      bootloader::stm32FlashLayout(),
      bootloader::BootState{12U, 3U, false, false},
      verifier);
  bootloader::TextBootReporter reporter(std::cout);

  std::cout << "Secure Bare-Metal Bootloader\n";
  std::cout << "Flash model: dual slot + scratch, ECDSA P-256 + SHA-256\n\n";

  const auto decision =
      bootloader.decide(bootloader::factoryImage(), bootloader::pendingUpdateImage());
  reporter.publish(decision);
  return decision.accepted ? 0 : 1;
}
