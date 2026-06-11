# Secure Bare-Metal Bootloader Validation Plan

## Host Validation

| Layer | Evidence |
| --- | --- |
| Configure | `cmake -S . -B build` succeeds. |
| Build | `cmake --build build` succeeds with warnings enabled. |
| Unit tests | `ctest --test-dir build --output-on-failure` passes. |
| CLI smoke | Nominal and failure scenarios produce deterministic status. |
| Documentation | Design package and diagrams are present in `docs/design`. |

## Scenario Matrix

| Scenario | Expected Result |
| --- | --- |
| Nominal | Accepted decision and generated evidence. |
| Invalid input | Rejected decision with stable issue code. |
| Resource limit | Rejected or warning decision depending on severity. |
| Adapter failure | Degraded or fault path with reproducible diagnostics. |
| Recovery path | Runtime returns to ready state or requests safe reset. |

## Hardware Validation

| Area | Evidence To Capture |
| --- | --- |
| Boot or startup | Serial log, boot time, version banner, reset reason. |
| Bus or peripheral | Logic analyzer, `dmesg`, register dump, or protocol trace. |
| Timing | GPIO pulse, cycle counter, scheduler trace, or latency histogram. |
| Power | Active/sleep current capture and battery threshold behavior. |
| Output | Telemetry frame, database row, command output, actuator response, or report artifact. |
| Fault handling | Injected error and resulting issue code. |

## CI Acceptance

CI should fail on:

- Compile errors or warnings promoted to errors where supported.
- Unit-test regression.
- Missing design artifacts.
- Broken diagram source XML.
- CLI scenario returning unexpected status.

## Production Evidence

Future production validation should include fixture scripts, sample reports, calibration records, firmware version records, and pass/fail criteria tied back to requirements.
