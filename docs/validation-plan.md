# Validation Plan

## Current Host Checks

- CMake configure completes.
- C++17 secure boot policy builds.
- Executable selects a pending signed update from slot B.
- CTest verifies signature fallback, rollback rejection, DFU request, tamper halt, and boot diagnostics.
- GitHub Actions runs configure, build, executable smoke run, and CTest.

## Hardware Evidence To Add

- Linker map showing bootloader, slot A, slot B, and scratch partitions.
- UART/USB DFU transcript for a staged update.
- Signature verification log with accepted SHA-256 digest.
- Rollback counter persistence evidence across reset.
- Tamper latch halt evidence.
- CI screenshot after the public repository is pushed.

## Project-Specific Evidence Target

Deep hardware/software integration, secure boot fundamentals, memory maps, and recoverable field updates.
