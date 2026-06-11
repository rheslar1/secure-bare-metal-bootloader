# Secure Bare-Metal Bootloader System Design

## Executive Summary

Custom bootloader with flash memory partitioning, rollback-safe image slots, cryptographic signature verification, and update diagnostics.

The system is structured as a production-style embedded project with clear boundaries between application orchestration, adapters, domain logic, safety policy, telemetry or persistence, and validation evidence.

## Objectives

- Build a host-testable C++17 model of the runtime.
- Keep board, OS, bus, driver, model, transport, and persistence details behind adapter interfaces.
- Support nominal, degraded, and rejected paths with deterministic issue codes.
- Preserve traceability from requirements through validation evidence.
- Provide a clean path from simulator-backed tests to hardware-backed proof.

## Logical Components

| Component | Responsibility |
| --- | --- |
| Application entry point | Startup, mode selection, final status. |
| Runtime orchestrator | Coordinates the use case through injected dependencies. |
| Domain model | Represents typed state, calculations, decisions, and issue codes. |
| Adapter layer | Isolates hardware, OS, RTOS, protocol, file, model, or transport APIs. |
| Policy layer | Applies safety, readiness, resource, security, and integrity gates. |
| Evidence layer | Emits logs, telemetry, reports, persistence records, or control output. |
| Test layer | Proves behavior with scripted adapters and CI-friendly scenarios. |

## Runtime Flow

1. Load project profile and configuration.
2. Collect input through hardware or simulator adapters.
3. Normalize input into typed domain state.
4. Evaluate policy gates before side effects.
5. Emit accepted output or rejected diagnostic evidence.
6. Return deterministic status for CI, fixture, or operator use.

## State Model

| State | Purpose |
| --- | --- |
| `Boot` | Initialize runtime assumptions and metadata. |
| `SelfTest` | Validate adapters, fixtures, board identity, or environment. |
| `Ready` | Wait for work or scheduled cycle. |
| `Acquire` | Collect raw input. |
| `Evaluate` | Transform input into a project decision. |
| `PolicyGate` | Approve, warn, reject, or fault. |
| `Publish` | Emit telemetry, report, persistence, or control output. |
| `Degraded` | Recoverable fault with evidence. |
| `Fault` | Unrecoverable or safety-critical failure. |

## Failure Model

| Failure Class | Handling |
| --- | --- |
| Invalid input | Reject active cycle and emit stable issue code. |
| Adapter failure | Enter degraded mode or queue evidence. |
| Resource limit | Reject startup or cycle if safety is affected. |
| Safety or integrity violation | Block side effects and preserve diagnostics. |
| Validation failure | Fail CI or manufacturing acceptance. |
