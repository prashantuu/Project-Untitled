# TopDownShooter

A top-down 2D shooter built from scratch in C++ using SFML 3, as a semester project.
Move around an arena, shoot enemies before they reach you, and survive as long as you can.

## Features

- WASD movement with mouse-aimed rotation
- Shooting with bullet travel, lifetime, and collision
- Enemies that continuously chase the player and spawn periodically over time
- Player health with damage cooldown (invincibility frames) so hits feel fair
- Tile-based level with wall/floor tiles
- Camera that smoothly follows the player and stays clamped within the level bounds
- On-screen HUD showing health and remaining enemy count
- Sound effects (gunshot, enemy death, player hit, game over) and looping background music
- Game Over screen on death

## Controls

| Action        | Input          |
|---------------|----------------|
| Move          | `W` `A` `S` `D` |
| Aim           | Mouse position  |
| Shoot         | Left mouse button |
| Close window  | Window close button |

## Technologies Used

- **C++17**
- **SFML 3** (Graphics, Window, Audio, System modules)

## Folder Structure

```
TopDownShooter/
│   .gitignore
│   README.md
│
├── assets/
│   ├── audio/
│   │   ├── sfx/           # gunshot, hit, death, game over sounds
│   │   └── music/         # looping background track
│   ├── fonts/              # UI font
│   ├── maps/                # (reserved for future external map files)
│   ├── sprites/
│   │   ├── player/
│   │   ├── enemy/
│   │   ├── weapon/
│   │   ├── bullet/
│   │   ├── tiles/
│   │   └── ui/
│   └── shaders/            # (reserved for future use)
│
├── build/                   # build output (ignored by git)
├── docs/
│
├── src/
│   │   Main.cpp
│   │
│   ├── audio/                # AudioManager — centralized sound/music playback
│   ├── bullet/                # Bullet — travels, expires, reports collision bounds
│   ├── core/                  # Game — owns everything, runs the main loop
│   ├── enemy/                  # Enemy, EnemyManager — chasing AI, spawning, collision
│   ├── graphics/               # Camera — follows the player, clamped to world bounds
│   ├── player/                  # Player — movement, aiming, health, shooting
│   ├── ui/                       # HUD — screen-space health/enemy count display
│   ├── weapon/                  # Weapon — owned by Player, spawns Bullets
│   └── world/                    # TileMap — parses and renders the level layout
│
├── tests/
└── tools/
```

## How to Build

### Prerequisites
- A C++17-compatible compiler (MSVC, GCC, or Clang)
- [CMake](https://cmake.org/) 3.16+
- [SFML 3](https://www.sfml-dev.org/) installed or fetched via CMake `FetchContent`/vcpkg

### Build steps
```bash
git clone https://github.com/prashantuu/Project-Untitled.git
cd Project-Untitled/TopDownShooter
mkdir build && cd build
cmake ..
cmake --build .
```

Run the produced executable from the project root (not from inside `build/`), so that
relative asset paths like `assets/sprites/player/player.png` resolve correctly.

### Assets
This project expects real image/audio/font files in `assets/`. Placeholder art was
sourced from free, open-license packs (e.g. [Kenney.nl](https://kenney.nl)) and
sound effects from [freesound.org](https://freesound.org). Replace any file in
`assets/` with your own art using the same filename to customize the look and feel.

## Screenshots

*(Add screenshots here once available — e.g. gameplay, HUD, and Game Over screen.)*

```
assets/docs/screenshot-gameplay.png
assets/docs/screenshot-gameover.png
```

## Architecture Notes

The project follows a strict single-responsibility ownership chain:

```
Game
├── TileMap        (renders the level)
├── Player
│   └── Weapon
│       └── Bullet (owns nothing shared — reads a texture Weapon owns)
├── EnemyManager
│   └── Enemy      (reads a texture EnemyManager owns)
├── Camera          (follows a position it's given, knows nothing about Player)
├── AudioManager    (plays named sounds; nothing else knows audio exists)
└── HUD             (stateless — displayed values are passed in each frame)
```

Each class only knows about the layer directly below it. For example, `Game` never
touches a `Bullet` directly — it asks `Player` "did you get hit?", and `Player` asks
its `Weapon`, which is the only class that actually owns bullets.