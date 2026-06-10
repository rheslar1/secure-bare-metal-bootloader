# Flash Layout

The host model uses an STM32-style 1 MiB flash map:

- `bootloader`: `0x08000000`, 64 KiB
- `slot_a`: `0x08010000`, 448 KiB
- `slot_b`: `0x08080000`, 448 KiB
- `scratch`: `0x080F0000`, 64 KiB

The boot policy requires each image vector table and byte range to fit entirely within its declared slot. Slot addresses must align to the configured erase size.

## Boot Rules

- Halt immediately if the tamper latch is set.
- Enter DFU immediately if the operator requests recovery.
- Prefer a pending update while its trial boot attempts remain below the limit.
- Verify SHA-256 digest shape and ECDSA P-256 signature evidence.
- Reject images below the stored rollback counter.
- Fall back to a confirmed image when a pending image fails validation.
- Enter DFU when no bootable image is available.
