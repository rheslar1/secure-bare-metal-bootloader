# Secure Bare-Metal Bootloader Deep Architecture

## Executive Overview

Custom bootloader with flash memory partitioning, rollback-safe image slots, cryptographic signature verification, and update diagnostics.

This repository is structured as a reviewable embedded-systems project rather than a loose code sample. The architecture separates runtime orchestration, hardware or simulator adapters, safety validation, telemetry or reporting, and CI evidence so each part can be reasoned about independently.

## Technology Profile

C++17, C++ Design Patterns, SOLID, Bare metal C, Linker scripts, Flash layout, ECDSA, SHA-256, UART/USB DFU

## System Context

The system boundary is the device or embedded Linux application represented by this repository. Inputs arrive from sensors, buses, operator commands, update packages, provisioning records, or simulator fixtures. Outputs are telemetry frames, actuator commands, firmware/update decisions, reports, and validation logs.

## Primary Runtime Flow

1. Startup loads a board or project profile and establishes the configured runtime identity.
2. Hardware-facing adapters or simulator fixtures collect the next input sample.
3. The domain/control layer normalizes the sample into a deterministic state model.
4. Safety and readiness gates reject invalid inputs, unsafe commands, unsupported boards, stale update images, or missing provisioning material.
5. Accepted state is mapped into telemetry, command output, persistence records, or human-readable evidence.
6. The executable returns a deterministic pass/fail result so CI can verify the behavior without target hardware.

## Component Responsibilities

| Component | Responsibility |
| --- | --- |
| Application entry point | Owns startup, CLI or demo flow, and final process status. |
| Domain model | Represents the project-specific device state, control policy, and validation rules. |
| Adapter boundary | Isolates sensors, buses, files, provisioning stores, transports, firmware slots, or board-specific APIs. |
| Safety gates | Validate battery, timing, configuration, update, telemetry, and hardware assumptions before side effects. |
| Evidence/reporting | Emits deterministic output for reviewers, CI logs, and portfolio documentation. |
| Tests and CI | Keep the host model buildable and protect expected failure modes. |

## Source Boundaries

- Source boundary: `include, src`
- Executable entry: `src/main.cpp`
- Test coverage: `tests/BootloaderTests.cpp`
- Documentation: `README.md`, `ARCHITECTURE.md`, `docs/validation-plan.md`, and this file.
- UML assets: `docs/diagrams/full-system-uml.drawio` and `docs/diagrams/full-system-uml.png`.

## SOLID and Pattern Notes

- Single Responsibility: startup, state modeling, validation, reporting, and tests stay in separate files or classes.
- Open/Closed: hardware adapters and validation policies can be extended without rewriting the top-level flow.
- Liskov Substitution: simulator adapters can stand in for target hardware when they satisfy the same narrow contract.
- Interface Segregation: each boundary exposes only the behavior needed by the orchestrator.
- Dependency Inversion: high-level runtime logic depends on abstractions or stable data contracts rather than direct hardware calls.

## Failure Model

The host-buildable implementation should make failure explicit. Common rejection paths include unsupported board profiles, missing provisioning, invalid credentials or firmware metadata, low battery, out-of-range samples, timeout/offline transports, unsafe control output, and exceeded timing or power budgets.

## Validation Matrix

| Layer | Validation Evidence |
| --- | --- |
| Build | CMake configure and native compiler output. |
| Runtime | Executable smoke run with deterministic text output. |
| Safety | Unit tests or CLI modes that verify accepted and rejected paths. |
| Documentation | Architecture, validation plan, and UML diagram checked into the repo. |
| Hardware path | Future serial logs, screenshots, power captures, bus traces, or flashing logs. |

## Deployment and Hardware Path

The repository is ready to grow from a host model into target-backed firmware or embedded Linux deployment. The intended path is to replace simulator adapters with board drivers, keep the same validation vocabulary, add hardware logs to `docs/`, and let CI continue proving host-side behavior even when target hardware is not attached.

## UML Diagram

The full UML diagram is maintained as Draw.io source and as a PNG export:

- [Full UML Draw.io source](diagrams/full-system-uml.drawio)
- [Full UML PNG export](diagrams/full-system-uml.png)
