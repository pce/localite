
Prototype of a local Content Browser

- the Filesystem as Database by design

## TODO

- Support more Fileformats (JSON) and Example Clients
- Content Browser
  - drag and drop
  - edit
- Export (or Sync) Options 

## DONE

- Browse Files
  - Preview Images

## Make

    mkdir build && cd build
    cmake ..
    make

    cd ..
    build/Localite


## Build

vcpkg might be unable to determine target architecture.  
Consider providing a value for the CMAKE_OSX_ARCHITECTURES cache variable:


    set(CMAKE_OSX_ARCHITECTURES arm64 CACHE STRING "Target architectures" FORCE)
    

Or, when invoking CMake, specify the architecture using flags:

    cmake -DCMAKE_OSX_ARCHITECTURES=x86_64;arm64 ..  # for universal binary Intel (x86_64) and Silicon

    cmake -DCMAKE_OSX_ARCHITECTURES=arm64 ..         #  Apple Silicon (arm64)


In CMakeLists.txt adjust the path to vcpkg or install it with: 

    cd ../../
    mkdir external && cd external 
    gcl git@github.com:microsoft/vcpkg.git
    # windows
    .\vcpkg\bootstrap-vcpkg.bat 
    # unix (linux, macos)
    ./vcpkg/bootstrap-vcpkg.sh   


Or vcpkg in the project dir:

    # set (VCPKG_DIR ${CMAKE_SOURCE_DIR}/external/vcpkg)



## Run

Browse sample project: _data

