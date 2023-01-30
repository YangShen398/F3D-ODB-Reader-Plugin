# F3D ODB Reader Plugin

This project builds a plugin to read ODB files from Abaqus for F3D. See [this Github site](https://github.com/f3d-app/f3d) for more information about F3D.

# Build

F3D, VTK, and Abaqus SDK are required dependencies to build this project. Follow the instructions from the official sites to build and install [F3D](https://github.com/f3d-app/f3d) and [VTK](https://github.com/Kitware/VTK). Abaqus C++ SDK comes with the installation of Abaqus. This project is built with CMake.

To configure the project using CMake, a CMake variable `AbqSDK_PRIVATE_INCLUDE_DIR` must be defined to let CMake know where to find Abaqus C++ SDK. `AbqSDK_PRIVATE_INCLUDE_DIR`, on my machine for example, is given as `C:/Program Files/Dassault Systemes/SimulationServices/V6R2019x/win_b64/code/include;C:/Program Files/Dassault Systemes/SimulationServices/V6R2019x`. Once the configuration is successful, meaning F3D, VTK, and Abaqus C++ SDK are found, you can build the project to generate the dynamic library.

You can enable `BUILD_TESTING` to enable testing and use `ctest` command in the build directory to test the plugin.

# Usage

The target is built into a dynamic library (.dll on Windows). Read this [page](https://github.com/f3d-app/f3d/blob/f62b50d5c054375685b526394bb9d21203c270ac/doc/libf3d/PLUGINS.md) for how plugins are used in f3d. An example to launch it from command line is:
`f3d.exe --load-plugins ./f3d-plugin-ODBReader.dll --input ./Job-1.odb`
