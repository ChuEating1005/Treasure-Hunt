# Treasure Hunt

This project is a 3D animation featuring a character named Steve, implemented using C++ with OpenGL and GLFW.

## Prerequisites

Before you can run this project, ensure you have the following installed:

- C++ Compiler (e.g., g++, clang++)
- CMake
- OpenGL
- GLFW
- GLM
- GLAD

## Building the Project

1. **Clone the Repository**

   First, clone the repository to your local machine:

   ```bash
   git clone https://github.com/ChuEating1005/Treasure-Hunt.git
   cd Treasure-Hunt
   ```
   
2. **Build with CMake**

   Create a build directory and compile the project using CMake:

   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```


## Running the Project

After building the project, you can run the executable:
```bash
./Treasure-Hunt
```

## Controls

### Steve

- **W**: Move forward
- **S**: Move backward
- **A**: Move left
- **D**: Move right
- **Left Mouse Button**: Swing Steve's right hand
- **V**: Toggle between first-person and third-person view
- **B**: Make Steve fall (simulate death)
- **N**: Revive Steve
- **O**: Open the chest
- **C**: Close the chest

### Creeper

- **L**: Turn head left
- **J**: Turn head right
- **H**: Toggle scale and shimmer
- **K**: Move forward (hold to move, release to stop)
- **I**: Jump (can be triggered while walking)

### Camera

- **Third-Person View**:
  - **Arrow Keys**: Move camera (UP/DOWN to zoom, LEFT/RIGHT to rotate)
- **First-Person View**:
  - **Cursor Movement**: Look around

## Additional Information

- Ensure your system supports OpenGL and has the necessary drivers installed.
- The project uses GLM for mathematics and GLAD for loading OpenGL functions.

## Troubleshooting

- If you encounter any issues with missing libraries, ensure all dependencies are correctly installed and linked.
- For any OpenGL-related errors, verify that your graphics drivers are up to date.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
