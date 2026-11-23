# Fatass Teto Kasaneroids

A roguelike-inspired take on the classic Asteroids arcade game, featuring Vocaloid characters and enhanced gameplay mechanics. This project demonstrates modern C++ practices with STL containers and proper memory management.

## Overview

Fatass Teto Kasaneroids is an enhanced Asteroids clone where you control various Vocaloid characters to destroy Teto plush asteroids. The game features multiple playable characters, game modes, and visual enhancements, all built on a refactored C++ codebase using modern best practices.

## Features

### Gameplay
- **6 Playable Characters**: Choose from Pearto, Miku, Neru, Luka, Kaito, or Gumi
- **Two Game Modes**:
  - **Single Round**: Clear all asteroids to win
  - **Infinite Rounds**: Progress through increasingly difficult levels, gaining extra lives
- **Special Attack System**: Hold UP to charge a powerful baguette attack
- **Lives System**: Start with 3 lives, gain extra lives in infinite mode
- **Dynamic Lives Display**: Shows individual hearts (1-3 lives) or "Nx <3" format (4+ lives)

### Visual Features
- **Character Sprites**: Each character has unique portrait displayed during gameplay
- **Teto Plush Asteroids**: Asteroids display fatass Teto plush images behind semi-transparent polygons
- **Baguette Special Attack**: The charged special shot displays a baguette sprite
- **Semi-transparent Polygons**: Asteroid polygons are 25% transparent to show plush images behind
- **Keyboard Navigation**: All menus use keyboard controls (WASD/Arrow keys + Enter)

### Technical Enhancements
- **STL Containers**: Replaced manual pointer management with `std::vector`
- **GameState Class**: Encapsulated game state management
- **Performance Optimizations**: Efficient collision detection that only processes active game objects
- **Modern C++**: C++14 standard with RAII principles

## Game Controls

### Menu Navigation
- **UP / W**: Move selection up
- **DOWN / S**: Move selection down
- **LEFT / A**: Move selection left (character select)
- **RIGHT / D**: Move selection right (character select)
- **ENTER**: Confirm selection / Start game

### Gameplay
- **LEFT / A**: Move ship left
- **RIGHT / D**: Move ship right
- **W / UP (hold)**: Charge special baguette attack
- **W / UP (release)**: Fire regular shot
- **R**: Restart game (on game over/victory screen)

## Gameplay Modes

### Single Round
- Destroy all 10 asteroids to win
- 3 lives - game over when all lives are lost
- No level progression

### Infinite Rounds
- Clear levels to progress
- Each level cleared grants +1 extra life
- Level counter displayed in UI
- Asteroids respawn at new positions each level
- Continue playing until game over

## Building the Project

### Prerequisites
- g++ compiler with C++14 support
- Raylib library (included in `lib/` directory)
- make build system

### Build Instructions

```bash
# Clean previous builds
make clean

# Build the project
make

# Run the game
./bin/juego_asteroids
```

### Build Output
The executable will be created at `./bin/juego_asteroids`

## Project Structure

```
.
├── include/           # Header files
│   ├── Asteroide.h
│   ├── Bala.h
│   ├── GameState.h
│   ├── Juego.h
│   ├── PoliReg.h
│   └── Punto2D.h
├── src/              # Source files
│   ├── Asteroide.cpp
│   ├── Bala.cpp
│   ├── GameState.cpp
│   ├── Juego.cpp
│   ├── PoliReg.cpp
│   └── Punto2D.cpp
├── assets/           # Game assets
│   ├── fatass_teto.png   # Asteroid texture
│   ├── pearto.png        # Pearto character
│   ├── miku.png          # Miku character
│   ├── neru.png          # Neru character
│   ├── luka.png          # Luka character
│   ├── kaito.png         # Kaito character
│   ├── gumi.png          # Gumi character
│   └── baguette.png      # Special attack texture
├── lib/              # Raylib library
├── bin/              # Build output (created during build)
├── obj/              # Object files (created during build)
├── makefile          # Build configuration
├── README.md         # This file
├── LICENSE           # MIT License
└── .gitignore        # Git ignore rules
```

## Technical Details

### Memory Management Refactoring
- **Replaced manual pointer management**: The `PoliReg` class uses `std::vector<Punto2D>` instead of raw pointers
- **Eliminated manual new/delete**: All dynamic memory allocation handled by STL containers
- **Removed manual memory functions**: Functions like `Redim()`, `liberaMemoria()`, and `reservaMemoria()` removed
- **Automatic cleanup**: STL handles memory management automatically

### Global Variable Encapsulation
- **GameState class**: All game state variables encapsulated in dedicated class
- **STL vectors**: Game entities stored in STL vectors instead of C-style arrays
- **Proper encapsulation**: Getters and setters for game state management
- **Modular design**: Game logic organized into maintainable functions

### Performance Optimizations
- **Active object processing**: Collision and movement functions only process living asteroids
- **Updated loop counters**: Local variables updated when objects are destroyed
- **Proper index management**: Loops correctly adjust indices when objects are removed
- **Reduced iterations**: Performance improves as game progresses and asteroids are destroyed

## Educational Value

This project demonstrates:
- Converting legacy C++ code to modern C++ practices
- Proper use of STL containers for memory management
- Encapsulation and object-oriented design principles
- Game state management patterns
- RAII (Resource Acquisition Is Initialization) principles
- Performance optimization in game loops
- Asset management and sprite rendering

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Credits

Based on the original memory management programming exercise for Computer Science major course.

Enhanced with character sprites, game modes, and modern C++ refactoring.
