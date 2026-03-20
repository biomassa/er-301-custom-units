# er-301-custom-units
This is a fork of the er-301-custom-units repository by user tmsfet. Huge thanks to them!

## Building

We need [odevices/er-301](https://github.com/odevices/er-301) as a submodule to access it's header files during the build:
```
git submodule update --init
```

### Make Commands

| Command | Description |
|----|----|
| `make -j all` | Build everything |
| `make -j <mod>` | Build a single module |
| `make -j <mod>-install` | Build and copy to the emulator dir at `~/.od/front/ER-301/packages` |
| `make emu` | Build and start the emulator |
| `make release` | Build and package everything for release |
| `make <mod>-install-sd` | Copy a release package to the SD card |

## Example units

Working example of a middle layer (Lua) unit is in the folder /harmonic.
Working examples of low level units (C++ and Lua) are in /mods folder.

## Docker file

The docker file needed for cross-compilation into the am335x architecture is in docker/

## Notes

Code needs to use Neon intrinsics for vectorization / optimization.
