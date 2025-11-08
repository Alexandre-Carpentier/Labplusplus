# Lab++

## Installing external depedencies
1) With vcpkg:
Install wxWidgets as static lib (no dlls)
```
# .\vcpkg.exe install wxwidgets:x64-windows-static
```
don't forget to set correctly the VCPKG_ROOT path if your installation is external to Visual Studio.
add the generated toolchain in "project/app/CMakeLists.txt"
```
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
```
You can also point the absolute path for to your lib located by default here on Windows:
```
C:\XXX\vcpkg\packages\wxwidgets_x64-windows-static\include
C:\XXX\vcpkg\packages\wxwidgets_x64-windows-static\lib
```

2) With wxWidgets installer:
https://github.com/wxWidgets/wxWidgets/releases/download/v3.3.1/wxMSW-3.3.1-Setup.exe
Add to environnement variable the custom path to your librairie folder:
```
WXWIN C:\XXX\WxWidget-331
```

3) Check installation
If installation success find_package will retrieve automatically wxWidgets.
```
find_package(wxWidgets REQUIRED COMPONENTS net core base gl)
```
## Build with CMake
### MSVC VS
Open the folder directly in Visual studio. type "CTRL-S" when openning the top CMakeLists.txt.
Select target application = labplusplus.exe and click generate.

### MSVC VS code
install support for C/C++ plugin. Install support for CMake support.
Open a terminal and do the standard building process.
```
# mkdir build && cd build
# cmake ..
# make
# .\Labplusplus.exe
```
## Install

Some optionnal depedencies can be used. 
1) DAQmx: DAQmx is mandatory if you use NI DAQs, like USB6001, CompactRio chassis,...
2) GNUPlot: You might export the recorded files to pdf files. Lab++ use GnuPlot to convert data to pdf graph. You can customize option in the GnuPlot script to change the pdf visual.
3) VISA library: To communicate with instruments over RS232/TCPIP/USB.

You can:
-Use IVI foundation VISA library
-Use NI VISA library
-Use WIN32 native lab++ VISA driver (you don't need to install driver on Windows). This way you can use instruments without NT administrator authority.

### Nationnal Instrument DAQmx driver
In order to use DAQ from NI you must install proprietary driver from NI: https://www.ni.com/fr/support/downloads/drivers/download.ni-daq-mx.html
By default

