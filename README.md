# SoftwareRenderer (Gleiss)

A lightweight, hardware-independent 3D **Software Renderer** written in **C** from scratch using **SDL3** for window management and framebuffer display. 

This project implements a complete, customizable 3D graphics pipeline entirely on the CPU—handling matrix math, vertex transformation, primitive assembly, barycentric rasterization, and custom shader pipelines without relying on OpenGL, Vulkan, or GPU hardware acceleration.

---

## Features

* **3D Math Engine:** Built-in vector (`vec2`, `vec3`) and 4x4 matrix operations (Model, View/LookAt, Perspective Projection).
* **Programmable-Style Shader Architecture:** 
  * Vertex Shader batch pass (`Flat` & `Gouraud`).
  * Custom Fragment Shader passes.
* **Barycentric Rasterizer:** Fast 2D bounding-box rasterizer using edge functions and barycentric coordinates 
* **Z-Buffering:** Pixel-accurate depth buffer for sorting overlapping 3D geometry.
* **Multiple Lighting Modes:**
  * **Flat Shading:** Per-face normal calculation and uniform color output.
  * **Gouraud Shading:** Smooth per-vertex diffuse lighting interpolation across triangle surfaces.
* **Double Buffering:** Clean rendering via raw CPU byte buffer streaming to an SDL3 texture.

---

## Pipeline Overview

[ Mesh Data ]

1. Vertex Shader Pass ─────► Transforms positions (MVP Matrix) & computes per-vertex attributes.

2. Primitive Assembly ─────► Groups transformed vertices into 3-vertex faces.

3. Barycentric Rasterizer ──► Calculates bounding boxes, interpolates Z-depth and barycentric weights.

4. Fragment Shader Pass ───► Calculates final RGB color and writes to RGBA framebuffer after Depth Test.

SDL3 Texture Update ──────► Flushes CPU pixel array onto screen.## Getting Started

### Prerequisites

* **C Compiler:** GCC or Clang (supporting C99 or later).
* **SDL3 Library:** Installed on your system system-wide.

### Building & Running

#### Linux / macOS

```bash
# Clone the repository
git clone https://github.com/mars-tx/gleiss.git
cd gleiss

# Compile the project
gcc -O3 src/*.c -Iinclude -lSDL3 -lm -o renderer

# Run the software renderer
./renderer
