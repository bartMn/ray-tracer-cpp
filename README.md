# Raytracer in C++

This project is a raytracer written from scratch in C++ as part of the coursework for **Computer Graphics: Rendering (INFR11246)**. The raytracer is capable of rendering 3D scenes described in JSON files and supports features such as reflections, shading, and texture mapping.

## Project Structure

- **`Code/`**: Contains all the source code files for the raytracer.  
- **`TestSuite/`**: Stores the rendered images (PPM files) for the scenes.   
- **`jsonFiles/`**: Contains JSON files that describe the scenes to be rendered.  

## Compilation

### Windows
To compile the project on Windows, use the following command:
    ```bash
    mingw32-make -f makefile.mak

## Linux
On Linux, ensure you have a compatible C++ compiler (e.g., g++) and make utility installed. Use the provided makefile to compile the project:
    ```bash
    make -f makefile.mak

## Running the Raytracer
1. Ensure the JSON files describing the scenes are placed in the jsonFiles/ directory.
2. The scenes to be rendered are specified in the scenes vector in the raytracer.cpp file (around line 145). Update this list to include the names of the scenes you want to render.
3. Run the compiled executable. The rendered images will be saved in the TestSuite/ folder as PPM files.


## Features
1. Scene Description: Scenes are described using JSON files, which specify objects, materials, light sources, and camera settings.
2. Reflections: Supports reflective surfaces with adjustable reflectivity.
3. Shading: Implements Lambertian shading and Phong shading for realistic lighting effects.
4. Texture Mapping: Allows applying textures to objects using PPM files.
5. Parallel Rendering: Utilizes multithreading to speed up rendering.


## Output
Rendered images are saved in the TestSuite/ folder as PPM files.  
Tone-mapped images (using Reinhard tone mapping) are also saved in the TestSuite/ folder with the prefix tonemapped_.

## Example Usage
1. Add a JSON file describing a scene to the jsonFiles/ folder (e.g., scene.json).
2. Update the scenes vector in raytracer.cpp to include "scene".
3. Compile and run the program.
4. The rendered image will be saved as TestSuite/scene.ppm.


## Dependencies
1. JSON for Modern C++: Used for parsing JSON files.
2. C++ Standard Library: Includes threading, file I/O, and mathematical utilities.


## Notes
The program is cross-platform and can run on both Windows and Linux.  
Ensure the jsonFiles/ directory contains valid JSON files describing the scenes.  
The rendered images are in PPM format, which can be viewed using image viewers like GIMP or converted to other formats using tools like ImageMagick.
