# Asteroids but Roguelike

An enhanced version of a Computer Science major memory management programming subject project. This project is a roguelike-inspired take on the classic Asteroids arcade game, demonstrating modern C++ practices and proper memory management using STL containers.

## Overview

This project was originally developed as a memory management programming exercise for a Computer Science major course. The original implementation used manual pointer management with raw pointers and manual memory allocation/deallocation. This enhanced version has been refactored to use modern C++ STL containers and best practices, eliminating manual memory management while maintaining the same gameplay mechanics.

## Key Enhancements

### Memory Management Refactoring
- **Replaced manual pointer management with std::vector**: The `PoliReg` class now uses `std::vector<Punto2D>` instead of raw pointers (`Punto2D*`)
- **Eliminated manual new/delete operations**: All dynamic memory allocation is now handled automatically by STL containers
- **Removed manual memory management functions**: Functions like `Redim()`, `liberaMemoria()`, and `reservaMemoria()` have been removed
- **Automatic cleanup**: No destructor needed for memory cleanup - STL handles it automatically

### Global Variable Encapsulation
- **Created GameState class**: All global game state variables are now encapsulated in a dedicated `GameState` class
- **Replaced C-style arrays with std::vector**: Game entities (asteroids, bullets) are now stored in STL vectors
- **Improved encapsulation**: Added proper getters and setters for game state management
- **Better code organization**: Game logic is now more modular and maintainable

## Features

- **Classic Asteroids gameplay with roguelike elements**
- **Player ship with horizontal movement**
- **Regular shots and special charged attacks**
- **Asteroid field with collision detection**
- **Asteroids split when hit (lose vertices)**
- **Lives system and game over/win conditions**
- **Title screen and restart functionality**

## Technical Details

### Original Implementation
- Manual pointer management for polygon vertices
- Global variables for game state
- C-style arrays for game entities
- Manual memory allocation/deallocation

### Enhanced Implementation
- `std::vector` for dynamic arrays
- `GameState` class for encapsulated state
- STL containers for all collections
- RAII principles for resource management
- C++14 standard with `constexpr` for compile-time constants

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
├── lib/              # Raylib library
├── bin/              # Build output (created during build)
├── obj/              # Object files (created during build)
├── makefile          # Build configuration
└── README.md         # This file
```

## Game Controls

- **LEFT / A**: Move ship left
- **RIGHT / D**: Move ship right
- **W / UP (hold)**: Charge special attack
- **W / UP (release)**: Fire regular shot
- **ENTER**: Start game / Restart
- **R**: Restart game

## Gameplay

1. Destroy all asteroids to win
2. Avoid colliding with asteroids
3. Regular shots reduce asteroid vertices
4. Hold UP to charge a powerful special attack
5. You have 3 lives - game over when all lives are lost

## Educational Value

This project serves as an excellent example of:
- Converting legacy C++ code to modern C++ practices
- Proper use of STL containers for memory management
- Encapsulation and object-oriented design principles
- Game state management patterns
- RAII (Resource Acquisition Is Initialization) principles

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
