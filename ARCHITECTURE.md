# Secure Bare-Metal Bootloader Architecture

## Goal

Deep hardware/software integration, secure boot fundamentals, memory maps, and recoverable field updates.

## Runtime Shape

1. Hardware or simulator input is sampled through a narrow driver boundary.
2. A control profile normalizes state into a deterministic decision surface.
3. Safety checks reject unsafe commands before they reach the actuator, transport, or update path.
4. Telemetry and validation logs are emitted for repeatable review.

## Boundaries

- `src/`: native starter implementation and future device-specific drivers.
- `docs/`: validation plans, timing notes, hardware captures, and acceptance evidence.
- `tests/`: repo-level smoke tests and future simulator or host-side unit tests.
- `.github/workflows/`: CI entry point for build and validation evidence.

## Validation Plan

- Build the host starter with CMake.
- Run the executable and confirm the reported profile matches this repository.
- Add hardware-specific logs after the first board, simulator, or bus test.
- Capture CI, terminal, and hardware evidence for the portfolio detail page.

## Expansion Notes

Replace the starter profile with the project-specific implementation slice while preserving the same review boundaries: build, tests, architecture notes, validation logs, and screenshots.
