/////////////////////////////////////////////////////////////////////////////
// Author:      Alexandre CARPENTIER
// Modified by:
// Created:     01/01/23
// Copyright:   (c) Alexandre CARPENTIER
// Licence:     LGPL-2.1-or-later
/////////////////////////////////////////////////////////////////////////////
#ifndef _USB_H__
#define _USB_H__
#include <Windows.h>
#include <locale>
#include <codecvt>
#include <iostream>
#include <vector>
#include <cassert>
#include "encoding.h"

// From datasheet
#include <devioctl.h>

//===================================================================
// The following are the I/O control codes that this driver supports.
#define FILE_DEVICE_USBTMC          0x8000
#define IOCTL_INDEX                 0x0800
#define IOCTL_USBTMC(idx,meth)      CTL_CODE(FILE_DEVICE_USBTMC,(idx),(meth),FILE_ANY_ACCESS)

#define IOCTL_USBTMC_GETINFO        IOCTL_USBTMC(IOCTL_INDEX,   METHOD_BUFFERED) // 0x80002000
#define IOCTL_USBTMC_CANCEL_IO      IOCTL_USBTMC(IOCTL_INDEX+1, METHOD_BUFFERED) // 0x80002004
#define IOCTL_USBTMC_WAIT_INTERRUPT IOCTL_USBTMC(IOCTL_INDEX+2, METHOD_BUFFERED) // 0x80002008
#define IOCTL_USBTMC_RESET_PIPE     IOCTL_USBTMC(IOCTL_INDEX+7, METHOD_BUFFERED) // 0x8000201C
#define IOCTL_USBTMC_SEND_REQUEST   IOCTL_USBTMC(IOCTL_INDEX+32,METHOD_BUFFERED) // 0x80002080
#define IOCTL_USBTMC_GET_LAST_ERROR IOCTL_USBTMC(IOCTL_INDEX+34,METHOD_BUFFERED) // 0x80002088

//=======================================================================
// The following are required data structures used for DeviceIoControl.
// Applications will pass data to the driver using these data structures.

// This data structure is used for:
// IOCTL_USBTMC_SEND_REQUEST
typedef struct USBTMC_IO_BLOCK
{
	UCHAR  bmRequestType;
	UCHAR  bRequest;
	USHORT wValue;
	USHORT wIndex;
	USHORT wLength;
	PUCHAR pbyData;              // ignore – use lpOutBuffer   instead – usbscan compatible
	UCHAR  fTransferDirectionIn; // ignore – use bmRequestType instead – usbscan compatible
} USBTMC_IO_BLOCK, * PUSBTMC_IO_BLOCK;

// This data structure is used for:
// IOCTL_USBTMC_CANCEL_IO
// IOCTL_USBTMC_RESET_PIPE
typedef enum USBTMC_PIPE_TYPE
{
	USBTMC_INTERRUPT_IN_PIPE = 1,
	USBTMC_READ_DATA_PIPE = 2,
	USBTMC_WRITE_DATA_PIPE = 3,
	USBTMC_ALL_PIPES = 4
} USBTMC_PIPE_TYPE, * PUSBTMC_PIPE_TYPE;

// This data structure is used for:
// IOCTL_USB_GETINFO
typedef struct USBTMC_DRV_INFO
{
	USHORT major;
	USHORT minor;
	USHORT build;
	WCHAR  manufacturer[64];
} USBTMC_DRV_INFO, * PUSBTMC_DRV_INFO;

//============================================================================
// Class GUID for all USBTMC devices is {A9FDBB24-128A-11D5-9961-00108335E361}
#define USBTMC_CLASS_GUID  (GUID)\
   { 0xa9fdbb24, 0x128a, 0x11d5, { 0x99, 0x61, 0x00, 0x10, 0x83, 0x35, 0xe3, 0x61 } }


#define URB_Class 0x20
#define URB_Vendor 0x40
#define URB_Reserved 0x60
#define URB_Standard 0x00

#define URB_DEVICE 0
#define URB_INTERFACE 1
#define URB_ENDPOINT 2
#define URB_OTHER 3

#define URB_GET_STATUS 0
#define URB_CLEAR_FEATURE 1
#define URB_SET_FEATURE 3
#define URB_GET_DESCRIPTOR 6
#define URB_SET_DESCRIPTOR 0x07
#define URB_GET_CONFIGURATION 0x08
#define URB_GET_INTERFACE 0x0A

#define TGT_DEVICE  0x00
#define TCT_INTFC   0x01
#define TGT_ENDPT   0x02
#define TGT_OTHER   0x03

#define REQ_STD     0x00
#define REQ_VENDOR  0x40
#define REQ_CLASS   0x20

#define DIR_FROM_DEVICE 0x80
#define DIR_TO_DEVICE   0x00



DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE, 0xa5dcbf10, 0x6530, 0x11d2, 0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed);
//DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE, 0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED);

#include "setupapi.h"
#include "winusb.h"

#include "cProtocol.h"

// A Usb class is a protocol
// IVI specification link bellow
// https://www.ivifoundation.org/downloads/Architecture%20Specifications/Ivi-6%202_USBTMC_2018-11-01.docx
class cUsb : public IProtocol {
public:
	cUsb();
	cUsb(std::wstring addr);
	virtual err_struct init();
	err_struct write(std::wstring scpi);
	err_struct read(std::wstring& scpi);
	virtual err_struct close();

	err_struct last_error;

private:
	HANDLE hUsb;

	std::string instrument_name;
};

#endif