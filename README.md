# game_of_life

An implementation of Conway's Game of Life written in C, rendered in the terminal using ncurses. This was the first version of what eventually became [Illumo](https://github.com/melj0202/CSim) — a full rendering engine with an OpenGL backend.

---

## What It Does

Simulates Conway's Game of Life — a cellular automaton where cells live or die each generation based on how many neighbors they have. The simulation runs in the terminal using ncurses for rendering, with support for preset starting configurations loaded from files.

- **Toroidal grid** — edges wrap around, so cells on one side treat the opposite edge as their neighbor
- **Variable simulation speed** — adjust delay up or down at runtime with `+` and `_`
- **Live stats** — generation count and alive cell count displayed each frame
- **Preset loader** — colony configurations are parsed from plain text files and centered on the canvas at startup

---

## Building and Running

Linux only. Requires ncurses.

```bash
make
./life <preset_file>
```

Preset configurations are included in the `presets/` directory.

```bash
./life presets/glider.txt
```

## Controls

| Key | Action |
|---|---|
| S | Start simulation |
| P | Pause / unpause |
| + | Speed up |
| _ | Slow down |
| Q | Quit |

---

## Context

This was an early project written in C to learn the language and get comfortable with terminal I/O, 2D grid logic, and basic simulation loops. The limitations of terminal rendering — resolution, color, performance — eventually motivated a full rewrite in C++ with OpenGL, which became [Illumo](https://github.com/melj0202/CSim).

---

## Lineage

1. **game_of_life** ← you are here — C, ncurses, terminal rendering
2. **CSim** — C++, OpenGL/GLFW, real-time rendering
3. **Illumo** — Full architectural rewrite, render pipeline abstraction, ECS scene system, infinite grid
