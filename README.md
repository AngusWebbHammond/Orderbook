# C++ Order Book

This is a project built to help understand what an Orderbook is, and to be used as a portfolio project, if you do find any issues with it, please raise an issue/pull request.

This is currently only tested to run on Windows, using the MSVC 2022 Compiler.

This project is licensed under the MIT license: [LICENSE](/LICENSE)

---

## How to Build

1. **Build the Build Environment**
   You can do this 2 ways. To do this make sure you are in the root directory of this project.

   - **Run the preconfigured script**
     On Windows the setup is simple, just run the preconfigured script:

     ```bash
     .\BuildCMake.bat
     ```

   - **Or Run**

     ```bash
     mkdir build
     cd build
     cmake ..
     cd ..
     ```

1. **Build the Executable and Run**
   This can be done in 2 ways aswell:

   - **Run the preconfigured script**
     Like previously on Windows, run the following script, this should be ran anytime you make any changes to the project header or source files:

     ```bash
     .\BuildAndRun.bat
     ```

   - **Or Run**

     ```bash
     cmake --build build
     .\build\Debug\main.exe
     ```
