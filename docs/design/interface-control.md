# Secure Bare-Metal Bootloader Interface Control Document

## Boundary Overview

The project boundary is the runtime executable or firmware module. Everything outside that boundary is treated as an adapter: physical hardware, kernel APIs, RTOS services, buses, transports, model engines, cloud services, or production fixtures.

## Primary Interfaces

| Interface | Responsibility | Example Implementations |
| --- | --- | --- |
| `IInputSource` | Collect the next sensor sample, bus frame, command, image metadata, or board state. | Scripted fixture, I2C/SPI driver, ADC driver, SocketCAN, file reader, RTOS queue. |
| `IHealthCheck` | Validate runtime readiness before side effects. | Battery check, firmware slot check, kernel module check, provisioning check. |
| `IProcessor` | Convert raw input into project-specific domain state. | Feature extractor, protocol parser, control loop, boot decision engine. |
| `IPolicy` | Approve or reject state transitions. | Safety bounds, rollback policy, anomaly threshold, transport readiness. |
| `IOutputSink` | Emit results. | UART, MQTT, SQLite, JSON report, GPIO/PWM output, flash log. |
| `IClock` | Provide deterministic time. | Fake clock, RTOS tick, Linux steady clock, hardware timer. |

## Data Contracts

| Contract | Required Fields |
| --- | --- |
| Runtime profile | Project id, board profile, firmware version, validation mode. |
| Input sample | Timestamp, source id, raw value or payload, validity flag. |
| Domain state | Normalized values, derived metrics, readiness flags. |
| Decision | Accepted flag, issue list, next action, evidence payload. |
| Telemetry/report | Project id, scenario, status, metrics, issue codes, timestamp. |

## Integration Rules

- Adapters may depend on vendor, OS, or hardware APIs; domain logic may not.
- Domain values should be copyable or movable C++17 value types.
- Error conditions should cross boundaries as typed issues, not raw strings only.
- Test fixtures must implement the same interface as target adapters.
- Persistent records and telemetry must include enough context to reproduce the decision.

## Versioning

Interface changes should update:

- Public headers or contract docs.
- Scenario tests.
- Validation plan.
- Telemetry/report schema notes.
- Release or migration notes when field data is affected.
