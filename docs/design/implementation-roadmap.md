# Secure Bare-Metal Bootloader Implementation Roadmap

## Phase 1: Design Package

- Add design docs and UML assets.
- Link package from README.
- Commit and push to GitHub.

## Phase 2: Host Runtime

- Add C++17 CMake source tree.
- Define value types, interfaces, runtime facade, and issue codes.
- Implement scripted adapters and deterministic tests.

## Phase 3: Target Adapters

- Add board, OS, RTOS, bus, driver, transport, or persistence adapters.
- Preserve host adapters for CI.
- Add toolchain or target build notes.

## Phase 4: Evidence

- Capture logs, traces, screenshots, measurements, or reports.
- Add evidence under `docs/evidence/`.
- Update validation matrix with measured results.

## Phase 5: Hardening

- Add fault injection tests.
- Add static analysis or formatting workflow.
- Add release checklist and manufacturing/deployment notes.
