# Secure Bare-Metal Bootloader Implementation Roadmap

## Phase 1: Design Package

- Add system design, requirements, interface control, runtime design, validation plan, roadmap, and UML diagrams.
- Link the package from `README.md`.
- Commit and push documentation to the project repository.

## Phase 2: Host Reference Runtime

- Add C++17 CMake project structure.
- Define domain value types and issue codes.
- Implement scripted adapters.
- Implement runtime facade and policy gates.
- Add deterministic tests and CLI scenarios.

## Phase 3: Target Adapter Layer

- Add hardware, OS, RTOS, bus, driver, transport, or persistence adapters.
- Preserve host adapters for CI.
- Add target build notes and toolchain file where needed.

## Phase 4: Hardware Evidence

- Capture logs, traces, screenshots, measurements, or reports from real or simulated target hardware.
- Add evidence under `docs/evidence/`.
- Update validation matrix with measured results.

## Phase 5: Production Hardening

- Add fault injection tests.
- Add static analysis or formatting workflow.
- Add release checklist.
- Add manufacturing or deployment scripts.
- Add security/update validation where applicable.
