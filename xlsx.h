#pragma once
/*
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <objbase.h>
#pragma comment(lib, "Ole32.lib")

#import "C:\Program Files (x86)\Common Files\Microsoft Shared\OFFICE14\MSO.dll" \
    rename("RGB","RGB_mso") rename("DocumentProperties","DocumentProperties_mso")

//using namespace Office;

#import "C:\Program Files (x86)\Common Files\Microsoft Shared\VBA\VBA6\VBE6EXT.OLB"

//using namespace VBIDE;

#import "C:\Program Files (x86)\Microsoft Office\Office14\EXCEL.EXE" \
    rename( "DialogBox", "ExcelDialogBox" ) \
    rename( "RGB", "ExcelRGB" ) \
    rename( "CopyFile", "ExcelCopyFile" ) \
    rename( "ReplaceText", "ExcelReplaceText" ) \
    exclude( "IFont", "IPicture" )
    */
int create_xlsx_from_saved_data(const char* path);