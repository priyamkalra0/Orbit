# Orbit

Orbit is a simple, computer-assisted, 2D procedural game based on orbital physics, written in SFML 3.0 and C++17.   
The player can navigate through space by toggling the gravitational orbits of different planets. The key is to time these orbit toggles such that the player character acquires a stable orbit around a neighbouring planet.

## Controls
*   Spacebar: Toggle current orbit  
...and that's all.

## Dependencies

*   SFML 3.0

## Build

This project uses CMake. To build, navigate to the project root and run:

```bash
mkdir build
cd build && cmake ..
cmake --build .
```

## Run

Navigate to the build directory and simply execute the compiled binary:

```./main```
