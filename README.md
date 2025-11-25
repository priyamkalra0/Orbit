# Orbit

A simple, computer-assisted, 2D procedural game based on orbital physics, written in C++17 with SFML 3.0.  
The player can navigate through space by toggling the gravitational orbits of different planets. The key is to time these orbit toggles such that the player character acquires a stable orbit around a neighbouring planet.

![sample-video](https://github.com/priyamkalra0/Orbit/releases/download/sample-video/sample.gif)

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

## Features

* [x] Window Management
* [x] Physics
* [x] Assist System
* [x] Camera
* [x] Sandbox
* [x] Colors
* [ ] Collision
* [ ] Progression
* [ ] UI
