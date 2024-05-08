# Valencia
Valencia is a utility designed to extract offsets from Apex Legends by performing signature scans. This tool is useful for developers interested in game modification and reverse engineering.

## Getting Started
These instructions will guide you on how to set up and run Valencia on your local machine for development and testing purposes.

## Prerequisites
Before you begin, ensure you have the following installed:
* Git
* CMake
* A C++ compiler (e.g., GCC, Clang)

## Cloning the Repository
To clone Valencia, use the following Git command: 
`git clone https://github.com/orange-cpp/valencia`

## Compiling the Program
Navigate to the cloned repository directory: `cd valencia`

Configure the build environment. You can specify Debug or Release mode. Here is how you can set up for a release build:
`cmake --preset linux-release -S .`

Now, compile the project: `cmake --build cmake-build/build/linux-release --target server -j 6`
This command builds the project using 6 parallel jobs, which should speed up the compilation process.

## Usage
To run Valencia, execute the following command: `./valencia /path/to/r5apex.exe`
Replace `/path/to/r5apex.exe` with the actual path to the Apex Legends executable on your system.

After running, Valencia will output the scanned offsets, which can be used for further development or analysis.

# Screenshot
Below is a screenshot of Valencia in action:

![img.png](https://i.imgur.com/B9Eeod6.png)