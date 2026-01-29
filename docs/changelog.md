# Changelog

A human‑friendly record of what has changed in the SOS Static IP Helper project.  
This project does not use semantic versioning — updates are logged chronologically.

---

## 2025‑01‑29 — Autosave, Menu Restore, SOS Fix, Code Cleanup

### Added
- **Autosave to NVS**: all `set` commands now immediately persist changes  
  (with clear “Updated.” + “Saved to NVS.” messages)
- **Restored full welcome menu** shown at boot and via `help`
- **README overhaul** with clearer features, usage, and hardware notes
- **CHANGELOG.md** (this file)

### Fixed
- **SOS heartbeat timing bug**  
  The original guard prevented the state machine from advancing.  
  Replaced with a correct non‑blocking timing check.

### Improved
- Code structure reorganised for readability:
  - Grouped related functions (config, menu, YAML, SOS, serial, button)
  - Cleaned up serial command handler
  - Clearer comments and section headers
- BOOT button logic simplified without changing behaviour

---

## 2025‑01‑28 — Initial Public Release

- Basic static IP helper working
- SOS heartbeat implemented
- Serial command interface for editing fields
- Netplan YAML generator
- BOOT button short/long press actions
- No persistence yet (values reset on reboot)
- Early README with basic usage notes

---

If you’re reading this and want to contribute improvements, feel free to open a PR or fork the project.
