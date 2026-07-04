# 3D OpenGL Maze Engine

<img width="500" height="500" alt="0001-0526-Trim-ezgif com-video-to-gif-converter" src="https://github.com/user-attachments/assets/ac168c2b-de01-43f3-a027-75f86fe4c4d7" />

## Detailed Description
This project is a 3D first-person maze game and sandbox engine built in C++ using **OpenGL** for low-level graphics rendering and **SFML** as a backend framework for window creation, context management, and input processing. 

The application demonstrates core graphics programming concepts by rendering a structured brick layout using custom shaders, asset texture wrapping, and a wide-open skybox environment map using an HDRI panoramic texture. Navigation handles seamlessly via a custom-coded, first-person camera controller coupled with an Axis-Aligned Bounding Box (AABB) collision detection algorithm, preventing the player from passing or clipping through the maze barriers.

---

## Features

* **Modern OpenGL Workflow:** Leverages direct vertex/index data arrays and custom shader processing to render structural 3D game spaces.
* **AABB Collision System:** Custom implementation of Axis-Aligned Bounding Box mathematical boundaries ensuring stable player-to-wall interaction handling.
* **HDRI Environment Skybox:** Renders an immersive high-definition panoramic outdoor sky layout acting as an organic background layer.
* **SFML Utility Backend:** Utilizes SFML strictly as a lightweight window management utility, keeping the rendering loop fast, decoupled, and close to pure OpenGL.

---

## Tech Stack

* **Language:** C++17 / C++20
* **Graphics API:** OpenGL 3.3+ (Core Profile)
* **Window/Input Management:** SFML (Simple and Fast Multimedia Library)
