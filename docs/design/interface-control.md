# Secure Bare-Metal Bootloader Interface Control

## Boundary Rules

- Domain logic depends on interfaces, not direct hardware or OS calls.
- Adapters may depend on vendor SDKs, Linux APIs, RTOS services, protocol stacks, or filesystems.
- Test fixtures implement the same interfaces as target adapters.
- Errors cross boundaries as typed issue codes plus human-readable detail.

## Primary Interfaces

| Interface | Responsibility | Example Implementations |
| --- | --- | --- |
| `IInputSource` | Collect raw samples, frames, commands, image metadata, or board state. | Scripted fixture, sensor driver, protocol reader, file reader. |
| `IProcessor` | Convert raw input into domain state. | Parser, feature extractor, control loop, boot decision engine. |
| `IPolicy` | Validate safety, readiness, resource, and integrity rules. | Battery policy, update policy, anomaly threshold, timing guard. |
| `IOutputSink` | Publish accepted or rejected evidence. | UART, MQTT, SQLite, JSON report, GPIO/PWM, flash log. |
| `IClock` | Provide deterministic time. | Fake clock, RTOS tick, Linux steady clock, hardware timer. |

## Data Contracts

| Contract | Fields |
| --- | --- |
| Runtime profile | Project id, board profile, firmware version, validation mode. |
| Input sample | Timestamp, source id, payload, validity flag. |
| Domain state | Normalized values, derived metrics, readiness flags. |
| Decision | Accepted flag, issue list, next action, evidence payload. |
| Report | Project id, scenario, status, metrics, issue codes, timestamp. |
