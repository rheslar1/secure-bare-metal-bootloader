# Secure Bare-Metal Bootloader System Design

## Executive Summary

Custom bootloader with flash memory partitioning, rollback-safe image slots, cryptographic signature verification, and update diagnostics.

The system is designed as a production-style embedded project with a clear separation between application orchestration, hardware-facing adapters, domain logic, safety policy, telemetry or persistence, and validation evidence.

## System Objectives

- Deliver the behavior described by the project focus while remaining host-buildable for CI.
- Isolate board, kernel, RTOS, bus, or driver details behind explicit interfaces.
- Provide deterministic pass/fail behavior for nominal, degraded, and rejected paths.
- Preserve design traceability from requirements through tests and documentation.
- Make future hardware evidence easy to add without rewriting the core design.

## External Actors

| Actor | Interaction |
| --- | --- |
| Target hardware or simulator | Provides sensor samples, bus frames, control inputs, image metadata, or board state. |
| Operator or commissioning tool | Configures runtime profile, thresholds, credentials, or deployment mode. |
| Edge gateway or host PC | Receives telemetry, logs, reports, commands, or manufacturing results. |
| CI runner | Builds the host model and runs deterministic validation tests. |
| Production fixture | Exercises hardware self-test, flashing, calibration, and acceptance evidence. |

## Logical Components

| Component | Responsibility |
| --- | --- |
| Application entry point | Parse mode, load configuration, run the orchestrator, and expose process status. |
| Runtime orchestrator | Own the high-level sequence and coordinate injected dependencies. |
| Domain model | Represent project-specific state, measurements, commands, firmware metadata, or control decisions. |
| Adapter layer | Hide hardware buses, OS APIs, drivers, transports, model engines, filesystems, or target services. |
| Safety and policy layer | Reject unsafe inputs, stale metadata, invalid timing, failed health checks, or resource violations. |
| Evidence layer | Emit deterministic logs, reports, telemetry, SQL, traces, or manufacturing records. |
| Test layer | Use scripted adapters to prove behavior without target hardware. |

## Runtime Flow

1. Startup loads static project metadata and runtime configuration.
2. The adapter layer collects the next input window, command, frame, image, or hardware state.
3. The domain model normalizes raw input into typed C++17 values.
4. Policy gates validate safety, readiness, timing, memory, update, or integrity assumptions.
5. Accepted state is transformed into telemetry, control output, persistence records, or a report.
6. Rejected state emits stable issue codes and exits through a safe or degraded path.
7. Tests verify both accepted and rejected paths through the same public runtime contracts.

## State Model

| State | Purpose | Typical Exit |
| --- | --- | --- |
| `Boot` | Initialize platform assumptions and static metadata. | `SelfTest` |
| `SelfTest` | Validate dependencies, fixtures, adapters, or board identity. | `Ready` or `Fault` |
| `Ready` | Runtime is configured and waiting for work. | `Acquire` |
| `Acquire` | Collect input from hardware, OS, transport, or simulator. | `Evaluate` |
| `Evaluate` | Normalize input and run domain-specific logic. | `PolicyGate` |
| `PolicyGate` | Apply safety, integrity, timing, and resource rules. | `Publish`, `Degraded`, or `Fault` |
| `Publish` | Emit telemetry, persistence, command output, or report. | `Ready` |
| `Degraded` | Recoverable fault with diagnostic evidence. | `Ready` or `Fault` |
| `Fault` | Unrecoverable or safety-critical failure. | Reset, maintenance, or manual recovery |

## Failure Model

| Failure Class | Example | Handling |
| --- | --- | --- |
| Input failure | Missing sensor sample, invalid frame, corrupt metadata. | Reject active cycle and emit issue code. |
| Adapter failure | Bus timeout, driver unavailable, transport offline. | Enter degraded mode or queue evidence. |
| Resource failure | Memory, flash, CPU, latency, or power budget exceeded. | Reject startup or active cycle. |
| Safety failure | Unsafe command, low battery, invalid update, untrusted image. | Block side effects and preserve diagnostics. |
| Validation failure | Test fixture or production self-test mismatch. | Fail CI or manufacturing acceptance. |
