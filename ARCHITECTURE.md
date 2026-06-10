# Secure Bare-Metal Bootloader Architecture

## Goal

Deep hardware/software integration, secure boot fundamentals, memory maps, and recoverable field updates.

## Runtime Shape

1. The bootloader validates the flash map and slot alignment.
2. Tamper and operator DFU gates run before image selection.
3. Pending updates are preferred while trial boot attempts remain below the limit.
4. Each candidate image is checked for slot fit, SHA-256 digest shape, ECDSA signature, rollback counter, and confirmation/pending state.
5. A failed pending image falls back to the confirmed alternate slot.
6. If no slot is bootable, the bootloader enters DFU recovery.

## C++17 Design Shape

- `SecureBootloader` owns boot policy and slot selection.
- `IImageVerifier` isolates cryptographic verification from policy.
- `SimulatedEcdsaSha256Verifier` provides deterministic CI behavior.
- `TextBootReporter` serializes diagnostics for UART/USB DFU style logs.

## SOLID Notes

- Single Responsibility: layout, image verification, boot policy, and reporting are separated.
- Open/Closed: hardware crypto or software crypto verifiers can replace the simulator.
- Liskov Substitution: any verifier can implement `IImageVerifier`.
- Interface Segregation: the verifier exposes only image verification.
- Dependency Inversion: boot policy depends on an abstract verifier.

## Boundaries

- `include/bootloader/`: boot policy data types and interfaces.
- `src/`: secure boot policy, simulated verifier, and CLI demo.
- `docs/`: validation plans, timing notes, hardware captures, and acceptance evidence.
- `tests/`: host-side tests for boot selection and recovery behavior.
- `.github/workflows/`: CI entry point for build and validation evidence.

## Validation Plan

- Build the host boot policy with CMake.
- Run the executable and confirm the pending signed update is selected.
- Run CTest to validate fallback, rollback, DFU, tamper, and diagnostics.
- Add hardware UART/USB DFU logs after target integration.
- Capture CI, terminal, and hardware evidence for the portfolio detail page.

## Expansion Notes

- Replace the simulated verifier with mbedTLS, tinycrypt, PSA Crypto, or hardware accelerator calls.
- Add linker script and vector-table handoff code for the selected MCU.
- Persist rollback counters and trial attempts in flash or OTP-backed storage.
