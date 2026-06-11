# Secure Bare-Metal Bootloader Design Package

## Purpose

Custom bootloader with flash memory partitioning, rollback-safe image slots, cryptographic signature verification, and update diagnostics.

This package defines the project as an implementation-ready embedded system. It covers system architecture, requirements, interface boundaries, runtime design, validation evidence, and phased delivery.

## Project Profile

| Field | Value |
| --- | --- |
| Repository | `rheslar1/secure-bare-metal-bootloader` |
| Primary stack | C++17, C++ Design Patterns, SOLID, Bare metal C, Linker scripts, Flash layout, ECDSA, SHA-256, UART/USB DFU |
| Review proof point | Deep hardware/software integration, secure boot fundamentals, memory maps, and recoverable field updates. |

## Artifacts

- [System Design](system-design.md)
- [Requirements](requirements.md)
- [Interface Control](interface-control.md)
- [Runtime Design](runtime-design.md)
- [Validation Plan](validation-plan.md)
- [Implementation Roadmap](implementation-roadmap.md)
- [Draw.io UML](diagrams/system-design.drawio)
- [PNG UML](diagrams/system-design.png)
