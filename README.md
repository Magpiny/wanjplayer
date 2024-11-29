# wanjplayer

Media player in C++

This software uses wxWidgets for GUI and libvlcpp to handle media files
I did this version to make it easy to port to windows as gstreamer does not work on windows.

NOTE: There are two versions of this app;
The first version in dev branch uses platform specific media files handlers 
while the second version uses libvlcpp library to handle media files in all
platforms.

## Dependencies
1. wxWidgets 3.2.6
2. libvlcpp

## PREREQUISITES
* wxWidgets 3.2.6
* CMake 3.30.3
* Latest versions of gcc or clang
* Make build system

## Usage:

#### Create a new directory (Assuming that you're on a unix system)
```bash
 mkdir wanjplayer
```
#### Move into the newly created directory
```bash
  cd wanjplayer
```
##### Clone the repo
```bash
 git clone https://github.com/Magpiny/wanjplayer.git
```

```bash
  cd wanjplayer
```
#### Assuming you have the lates version of wxWidgets and cmake installed in your system. Build the app
```bash
   cmake -S . build
```
#### cd into the build directory and build the app
```bash
  cd build
```
```bash
  cmake --build .
```

#### Run the app
```bash
  ./WanjPlayer
```
#### Exit the app
 Press exit/quit from the app (The recommended way)
 Alternatively press CTRL+C / CMD+C from the terminal

## Extend the app and share with your friends
## Submit a PR or feature request


