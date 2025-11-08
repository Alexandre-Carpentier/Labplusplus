# Lab++

## Installing external depedencies (wxWidgets)
1) With vcpkg:
Install wxWidgets as static lib (no dlls)
```
# ./vcpkg.exe install wxwidgets:x64-windows-static
```
Don't forget to set correctly the define VCPKG_ROOT path if your installation is external to Visual Studio.
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
### MSVC Visual Studio
Open the folder directly in Visual studio. type "CTRL-S" when openning the top CMakeLists.txt.
Select target application = labplusplus.exe and click generate.

### MSVC Visual Studio Code
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
+ Use IVI foundation VISA library
+ Use NI VISA library
+ Use WIN32 native lab++ VISA driver (you don't need to install driver on Windows). This way you can use instruments without NT administrator authority.

### Nationnal Instrument DAQmx driver
In order to use DAQ from NI you must install proprietary driver from NI: https://www.ni.com/fr/support/downloads/drivers/download.ni-daq-mx.html
By default

### Gnuplot software
Package installer is binded inside Labplusplus.exe, when running the app user is asked to install it if needed.
Gnuplot is available here: http://gnuplot.info/download.html

### VISA diver
To communicate with instruments it is possible to send commands with Serial line, Usb, Ethernet,... To do so, install a proprietery lib from any vendor or use the built in Windows driver. Labplusplus use a wrapper over CreateFile and other Ring3 api available natively.
The main advantage is to not need admin right to install a proprietary device. This is particulary usefull when you need to setup a quick measurement without calling the company IT group to run installer with administrative right. The consortium between VISA is IVI foundation.

## Using Labplusplus


