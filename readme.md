# <p align=center> GeimBoi
GeimBoi is a GameBoy DMG emulator written in C++ using [SDL2](https://www.libsdl.org/download-2.0.php) and [ImGui](https://github.com/ocornut/imgui). GeimBoi was done as a programming exercise and to learn more about emulation. This Emulator isn't perfect by ANY means and HAS bugs.

![gameboy_ss](pictures/geimboy.png)

## Features:
- **MBC1:** support (passes Mooneye's mbc1 tests)
- **MBC2:** support (passes Mooneye's mbc2 tests)
- **MBC3:** only rom/ram banking.
- **Saving:** roms with a battery gets it's ram dumbed into a *.sav file on exit and loaded on game power on.

## Todo:
- **Audio:** This emulator doesn't have any audio emulation at this time.
- **Accuracy:** This emulator is not currently timing/cycle accurate, and i'm not planning to make this 100% perfect, but atleast to pass some of the timing tests.
- **GBC Support:** GameBoy Color support down the line.
- **SaveState:** There're not currently savestates available.
- **GUI:** Lacks a lot of features, like bindable controls.

## Screenshots:
![pokemonrb_ss](pictures/pokemonrb.png)
![supermarioland_ss](pictures/supermarioland.png)

## Credits:
### Software:
- [SDL2](https://www.libsdl.org/download-2.0.php) a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D.
- [Dear ImGui](https://github.com/ocornut/imgui) a bloat-free graphical user interface library for C++.
- [ImGuiSDL2](https://github.com/Tyyppi77/imgui_sdl) a lightweight SDL2 based renderer for Dear ImGui.
### Docs:
- [Pan Docs](https://gbdev.io/pandocs/) a technical reference guide for the GameBoy.
- [Gekkio's Complete Technical Guide](https://gekkio.fi/files/gb-docs/gbctr.pdf)