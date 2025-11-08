# Lab++

## Build with CMake

### MSVC VS
### MSVC VS code
install support for C/C++ plugin. Install support for CMake support.
Open a terminal and do the standard building process.
# mkdir build && cd build
# cmake ..
# make
# .\Labplusplus.exe

## Install

Some optionnal depedencies can be used. 
1) DAQmx: DAQmx is mandatory if you use NI DAQs, like USB6001, CompactRio chassis,...
2) GNUPlot: You might export the recorded files to pdf files. Lab++ use GnuPlot to convert data to pdf graph. You can customize option in the GnuPlot script to change the pdf visual.
3) VISA library: To communicate with instruments over RS232/TCPIP/USB. You can:
-Use IVI foundation VISA library
-Use NI VISA library
-Use WIN32 native lab++ VISA driver (you don't need to install driver on Windows). This way you can use instruments without NT administrator authority.

### Nationnal Instrument DAQmx driver
In order to use DAQ from NI you must install proprietary driver from NI: https://www.ni.com/fr/support/downloads/drivers/download.ni-daq-mx.html
By default

