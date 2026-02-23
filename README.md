## CreateFileW Hook (Educational Demo)

This DLL demonstrates API interception of `CreateFileW` using Microsoft Detours.

### What it does

- Installs a hook in `DllMain` on `DLL_PROCESS_ATTACH`.
- Intercepts calls to `CreateFileW`.
- Logs matched file paths for debugging/telemetry purposes.
- Removes the hook on `DLL_PROCESS_DETACH`.

### Components

- `TrueCreateFileW` – pointer to the original API.
- `HookedCreateFileW(...)` – custom handler that inspects file paths.
- `SetupHook()` / `RemoveHook()` – Detours transaction wrappers.
- `DllMain(...)` – lifecycle entry point for attach/detach.

### Requirements

- Windows
- Microsoft Detours (`include/detours.h`)
- C++ toolchain for building a DLL

### Notes

- Use in controlled test environments only.
- API hooking can affect process stability if implemented incorrectly.
- Prefer explicit error handling and thread-safe logging in production code.
