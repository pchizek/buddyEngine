# buddyEngine
A C++ based, 2D game engine.

## Acknowledgements
- <b>Uses [SFML (Simple Fast Multimedia Library)](https://github.com/SFML/SFML) by Laurent Gomilla et al.</b>
- <b>Uses [tinyXML-2](https://github.com/leethomason/tinyxml2) by Lee Thomason.</b>
  
## Install
### Visual Studio 16 2019 + CMake GUI 

1. Clone or download buddyEngine
2. Download [the latest release of SFML](https://www.sfml-dev.org/download.php) to the same location as buddyEngine.
3. Use the CMake GUI to configure SFML [as per their tutorial](https://www.sfml-dev.org/tutorials/2.5/compile-with-cmake.php) but with additional steps:<br>
  Set `BUILD_SHARED_LIBS`  to `FALSE`<br>
  Set `SFML_USE_STATIC_STD_LIBS` TO `TRUE`
4. Open `SFML.sln` and build for <b>both Debug and Release</b>
5. Build `front.sln` and run/debug.
