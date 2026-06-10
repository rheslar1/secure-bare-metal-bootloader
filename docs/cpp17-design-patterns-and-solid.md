# C++17 Design Patterns and SOLID Notes

This repository's host model is intentionally written in C++17 so the project can be built and reviewed without target hardware.

## Patterns Used

| Pattern | Implementation |
| --- | --- |
| Strategy | `IValidationRule` implementations validate capabilities, connectivity, and metric thresholds. |
| Adapter | `ISampleSource` and `ITelemetryEncoder` isolate data acquisition and output formatting. |
| Composite | `CompositeValidator` combines independent rules into one validation pass. |
| Facade | `ProjectRuntime` exposes one `runOnce()` orchestration surface. |
| DTO / Value Object | `ProjectProfile`, `RuntimeSample`, `ValidationIssue`, and `RuntimeReport` carry deterministic state. |

## SOLID Mapping

- Single Responsibility: profile, sample source, validation, telemetry encoding, and report writing are separate types.
- Open/Closed: new validation rules can be added through `IValidationRule`.
- Liskov Substitution: any `ISampleSource` or `ITelemetryEncoder` can replace the host implementations.
- Interface Segregation: interfaces stay small and focused.
- Dependency Inversion: `ProjectRuntime` depends on abstractions and injected collaborators.

## Review Entry Points

- `include/project_runtime/ProjectRuntime.hpp`
- `src/ProjectRuntime.cpp`
- `src/main.cpp`
- `tests/ProjectRuntimeTests.cpp`

The executable supports nominal, unsafe, and offline scenarios:

```bash
./build/secure_bare_metal_bootloader --nominal
./build/secure_bare_metal_bootloader --unsafe
./build/secure_bare_metal_bootloader --offline
```
