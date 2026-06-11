# Secure Bare-Metal Bootloader Validation Plan

## Host Validation

| Layer | Evidence |
| --- | --- |
| Configure | CMake or native build configuration succeeds. |
| Build | Compiler completes with warnings enabled. |
| Unit tests | Scripted nominal and failure scenarios pass. |
| CLI smoke | Executable returns deterministic status codes. |
| Documentation | `docs/design` package and diagrams are present. |

## Scenario Matrix

| Scenario | Expected Result |
| --- | --- |
| Nominal | Accepted decision and generated evidence. |
| Invalid input | Rejected decision with stable issue code. |
| Resource limit | Warning or rejection depending on severity. |
| Adapter failure | Degraded or fault path with diagnostics. |
| Recovery path | Runtime returns to ready state or requests safe reset. |

## Hardware Evidence To Add

- Boot or startup log.
- Bus, protocol, register, or sensor trace.
- Timing or latency measurement.
- Power/current capture when relevant.
- Telemetry, report, database, or command output artifact.
- Fault injection record with issue code.
