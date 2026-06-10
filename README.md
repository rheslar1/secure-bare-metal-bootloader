# Secure Bare-Metal Bootloader

Custom bootloader with flash memory partitioning, rollback-safe image slots, cryptographic signature verification, and update diagnostics.

## Portfolio Purpose

This repository is an Embedded Systems project scaffold for the Rheslar portfolio. It is designed to become a hardware-backed project with build output, validation logs, and reviewable implementation evidence.

## Stack

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
python -m unittest discover -s tests
```

## Implementation Slices

- Native starter executable that exposes the project identity, stack, and validation target.
- Architecture document with control boundaries, data flow, safety assumptions, and evidence plan.
- Unit smoke test that keeps source, docs, and CI files present as the repo grows.
- GitHub Actions workflow for configure, build, executable smoke run, and repository validation.

## Evidence Target

Deep hardware/software integration, secure boot fundamentals, memory maps, and recoverable field updates.

## Remote

Intended public repository: https://github.com/rheslar1/secure-bare-metal-bootloader
