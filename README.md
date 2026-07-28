# Doodle Jump

A C++17 / SFML 3 implementation of Doodle Jump.

## Quick Start

**Requirements:**
- MinGW-w64 g++ (C++17)
- SFML 3
- GNU Make

**Build & Run:**
```bash
make                    # compile
make run                # run game 
./bin/doodle-jump.exe   # another way to run game
make clean              # cleanup
```

## Setup (Windows)

1. Install SFML 3 (MinGW build from [sfml-dev.org](https://www.sfml-dev.org))
2. Copy SFML's `include/` → MinGW's `include/`, `lib/` → MinGW's `lib/`
3. Copy SFML's `.dll` files to `bin/` folder next to the .exe

## Controls

- **Left/Right arrows** — move
- **Mouse** — menu/game-over/settings

## Features

- Infinite procedurally-generated platforms (Normal, Moving, Breakable)
- Spring boosters
- Monsters (shootable enemies)
- Black-holes
- Guaranteed reachability
- Camera follows player upward (smooth scrolling)
- Persistent high score
- Three levels of difficulty

## Design

- Small, single-purpose classes
- Polymorphic platform types via abstract `Platform` base
- Template `ResourceManager<T>` with SFML 3 loader traits

## Project Structure

```
doodle-jump/
├── include/          # Headers organized by subsystem
├── src/              # Implementations mirroring include/
├── assets/           # Game sprites
├── sounds/           # Game sounds
├── fonts/            # Game font
├── makefile          # Build script
├── settings.txt      # Difficulty and sound volume
└── highscore.txt     # Persisted high score
```

---

Author:

Maryam Jafari
github.com/imaryamjafari
