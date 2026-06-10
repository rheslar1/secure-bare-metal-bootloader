# Secure Bare-Metal Bootloader

Custom bootloader with flash memory partitioning, rollback-safe image slots, cryptographic signature verification, and update diagnostics.

## Portfolio Purpose

This repository implements a host-testable secure boot policy for a bare-metal dual-slot firmware layout. It models flash partition checks, ECDSA/SHA-256 image validation, rollback counters, pending-update trial boots, confirmed-image fallback, DFU recovery, and tamper halt behavior.

## Stack

- C++17
- C++ Design Patterns
- SOLID
- Bare metal C
- Linker scripts
- Flash layout
- ECDSA
- SHA-256
- UART/USB DFU

## Quick Start

```bash
cmake -S . -B build
cmake --build build
./build/secure_bare_metal_bootloader
ctest --test-dir build --output-on-failure
```

## Implementation Slices

- STM32-style flash layout with bootloader, slot A, slot B, and scratch partitions.
- Image header policy for vector address, size, SHA-256 digest, ECDSA signature, rollback counter, confirmation, and trial boot attempts.
- Pending update selection with fallback to a confirmed factory image.
- Operator DFU mode and tamper-latch halt behavior.
- Text diagnostics suitable for UART/USB DFU logs.
- CTest coverage for update selection, signature fallback, rollback rejection, DFU, tamper halt, and report evidence.

## Evidence Target

Deep hardware/software integration, secure boot fundamentals, memory maps, and recoverable field updates.

## Remote

Intended public repository: https://github.com/rheslar1/secure-bare-metal-bootloader
