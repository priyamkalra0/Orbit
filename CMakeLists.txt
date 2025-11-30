cmake_minimum_required(VERSION 3.10)
project(Orbit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

add_executable(
        main
        src/Entrypoint.cpp
        src/Core/Game.cpp
        src/Entity/Planet.cpp
        src/Entity/Orbit.cpp
        src/Entity/Player.cpp
        src/Core/Navigation.cpp
        src/Core/Assist.cpp
        src/Core/Level.cpp
        src/Core/Collision.cpp
        src/Graphics/Color.cpp
        src/Graphics/Particles.cpp
)

target_include_directories(main PUBLIC include)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Window SFML::System)
