# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

Firmware for a Digispark/ATtiny85 board that acts as a USB HID keyboard and types a pre-configured password when plugged in. The entire firmware is the single sketch `attiny-pass-key.ino`. Educational project.

## Commands

All workflows go through `mise` (tool versioning + tasks + env loading).

```bash
mise install               # install pinned arduino-cli (defined in mise.toml)
mise run setup             # create .env from .env.example, update Arduino core/lib indexes
mise run arduino:compile   # compile sketch, injecting password from .env
mise run arduino:upload    # flash via micronucleus (plug in board when prompted, ~60s window)
mise run arduino:monitor   # serial monitor
mise run arduino:run       # compile + upload + monitor
```

**IMPORTANT: Never run `arduino-cli` directly — always go through mise tasks.** mise loads `.env`, which sets `ARDUINO_CONFIG_FILE=arduino-cli.yaml`; without it, arduino-cli falls back to the global config instead of this repo's sandboxed setup.

## Verification

There are no tests or linters: `mise run arduino:compile` is the only correctness gate. Run it after every sketch or config edit to verify the firmware still builds.

## Architecture

- **Password injection happens at compile time, not runtime.** The `arduino:compile` task passes `-DATTINY_PASS_KEY_PASSWORD="..."` (value from `ATTINY_PASS_KEY_PASSWORD` in `.env`) via `compiler.cpp.extra_flags`. The sketch falls back to `"default_password"` if the macro is undefined. **IMPORTANT: the password is baked into the firmware binary — never commit, release, or share build artifacts from `.arduino/`, and never print the password in command output.**
- **Self-contained Arduino environment.** `arduino-cli.yaml` redirects all arduino-cli directories (data, downloads, user) into `.arduino/` inside the repo, which is gitignored. Platforms and tools are installed there, not globally.
- **Board profile** lives in `sketch.yaml`: FQBN `ATTinyCore:avr:attinyx5micr` (ATTinyCore with micronucleus bootloader). It pins two platforms: ATTinyCore 1.5.2 (the compiler target) and digistump 1.7.5 (only needed as the source of the `DigisparkKeyboard` library, referenced by a path inside `.arduino/data/internal/...`). That library path only exists after platforms are installed — compile resolves dependencies on first run.

## CI

`.github/workflows/ci.yml` runs on every push to `main`: installs mise, runs `setup` + `arduino:compile`, then creates a GitHub release tagged `build-<sha>` with the `.arduino` directory tarball. CI has no `.env`, so released firmware contains the `default_password` fallback.
