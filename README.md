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

<!-- cpp17-solid-implementation:start -->
## C++17, Design Patterns, and SOLID Implementation

This repository includes a host-buildable C++17 implementation, not only documentation. The implementation applies:

- Strategy pattern for validation rules.
- Adapter interfaces for input samples and telemetry/reporting.
- Composite validation for combining safety and readiness checks.
- Facade orchestration through the project runtime class.
- SOLID boundaries between profile data, input acquisition, validation, telemetry encoding, and tests.
<!-- cpp17-solid-implementation:end -->

<!-- deep-architecture-links:start -->
## Deep Architecture and UML

- [Deep architecture](docs/deep-architecture.md)
- [Full UML Draw.io source](docs/diagrams/full-system-uml.drawio)
- [Full UML PNG export](docs/diagrams/full-system-uml.png)
<!-- deep-architecture-links:end -->

<!-- DESIGN_PACKAGE_START -->
## Detailed Design Package

This repository includes a structured design package for **Secure Bare-Metal Bootloader**. The package captures the system boundary, runtime flow, hardware/software interfaces, validation strategy, and implementation roadmap.

| Artifact | Link |
| --- | --- |
| Design Index | [docs/design/README.md](docs/design/README.md) |
| System Design | [docs/design/system-design.md](docs/design/system-design.md) |
| Requirements | [docs/design/requirements.md](docs/design/requirements.md) |
| Interface Control | [docs/design/interface-control.md](docs/design/interface-control.md) |
| Runtime Design | [docs/design/runtime-design.md](docs/design/runtime-design.md) |
| Validation Plan | [docs/design/validation-plan.md](docs/design/validation-plan.md) |
| Implementation Roadmap | [docs/design/implementation-roadmap.md](docs/design/implementation-roadmap.md) |
| Draw.io UML | [docs/design/diagrams/system-design.drawio](docs/design/diagrams/system-design.drawio) |
| PNG UML | [docs/design/diagrams/system-design.png](docs/design/diagrams/system-design.png) |
<!-- DESIGN_PACKAGE_END -->
