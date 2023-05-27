# wemodevicecpp

## Project Overview
A native application to manage and control the [Wemo](https://www.wemo.com/) devices. This project is developed using native C/C++ language so that it can be ported to any environment. 

## About Wemo
Wemo is an IoT platform for smart lighting solutions, entertainment, security, temperature and more smart services.

## License
wemodevicecpp is licensed under the MIT license; see LICENSE in the source distribution for details.


## Prerequisites
Before building, you'll need to install following dependencies

### Installing prerequisites on Linux
On Debian-based Linux distributions such as Ubuntu, these dependencies can be satisfied with the following command:

    sudo apt-get install cmake libcurl4-gnutls-dev libpugixml-dev

## Building
wemodevicecpp should work with any Linux distribution and added cross compilation support.

    Tested on:
        x86_64
        Arm
        
### Checking out the wemodevicecpp
To check out the wemodevicecpp repository:

    git clone https://github.com/srisham/wemodevicecpp.git
    
### Build
Use cmake tools to build the wemodevicecpp. Run the following commands to build the app:

    mkdir build
    cd build
    cmake ..
    make
    
### Cross Compilation
In order to cross compile this module, use the cmake-tookchains file available in the [toolchain](cmake/toolchain) directory. Update the arm_toolchain.cmake file with your toolchain details. Refer the sample version in this [example](cmake/toolchain/example).
To cross-compile, run the following commands:

    mkdir build_arm
    cd build_arm
    cmake  -DCMAKE_TOOLCHAIN_FILE=<path_to_toolchain_cmake_file>/arm_toolchain.cmake ..
    make
    
## Usage
wemodevicecpp supports both interactive and cli methods. 
    
If only 'command' passed as input parameter, it will enable the app in interactive mode. User can enter the command specific inputs to customize the bulb.

    $ wemodevicecpp on
    
In cli mode, user can all pass necessary 'options' as arguments in one go. 

    $ wemodevicecpp getdeviceinfo --ip 192.168.20.105

To know the list of 'options' required for each command, run the 'help' option in the following way:

    $ wemodevicecpp off --help

## Supported Features
Following features are supported now:

    On/Off device
    Get Status
    Get Device Info

## Future Improvements
    Windows Support
