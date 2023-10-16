#include "xlsx.h"
int create_xlsx_from_saved_data(const char* path)
{
	return 0;
}

/*
int create_xlsx_from_saved_data(const char *path)
{
		// Load Ecxel instance via COM

	//Initialise the COM interface
	HRESULT hres = CoInitialize(NULL);
	//Define a pointer to the Excel application
	Excel::_ApplicationPtr xl;
	//Start one instance of Excel
	xl.CreateInstance(L"Excel.Application");

	//Set Excel visible
	xl->PutVisible(0, VARIANT_TRUE);

	//Add a (new) workbook
	xl->Workbooks->Add(Excel::xlWorksheet);
	//Get a pointer to the active worksheet
	Excel::_WorksheetPtr pSheet = xl->ActiveSheet;
	//Set the name of the sheet
	pSheet->Name = "Chart Data";

		// Open data file

	std::ifstream infile(path);
	std::string line;

		// Extract header from data measurement

	if (!std::getline(infile, line))
	{
		std::cout << "[!] No file header to read, build \".xslx\" failed.\n";
		xl->Quit();
		return -1;
	}

	std::vector<std::string> token;
	std::stringstream ss(line);

	size_t index = 0;
	size_t row_pos = 1;
	std::string element;
	while (std::getline(ss, element, '\t'))
	{
		token.push_back(element);
		pSheet->Cells->Item[row_pos][index+1] = token.at(index).c_str();
		index++;
	}
	token.clear();
	row_pos++;

	//pSheet->Cells->Item[1][1] = "Time (s)";

	//pSheet->Cells->Item[1][2] = "Analog0 (V)";



		// Read every line and transfert to excel sheet

	for (size_t i = 0; i < 20000000; i++)
	{


		// Read new line
		if (!std::getline(infile, line))
		{
			std::cout << "[*] Eof reached.\n";
			break;
		}

		index = 0;

		// Split item from the line separated by \t
		std::stringstream ss(line);
		while (std::getline(ss, element, '\t'))
		{
			token.push_back(element);
			pSheet->Cells->Item[row_pos][index+1] = token.at(index).c_str();
			index++;
		}
		token.clear();
		row_pos++;
		std::cout << "\r[*] Lines analyzed:"<< row_pos;
	}

		// Build chart

	//The sheet "Chart Data" now contains all the data
	//required to generate the chart
	//In order to use the Excel Chart Wizard,
	//we must convert the data into Range Objects
	//Set a pointer to the first cell containing our data
	Excel::RangePtr pBeginRange = pSheet->Cells->Item[1][1];
	//Set a pointer to the last cell containing our data
	Excel::RangePtr pEndRange = pSheet->Cells->Item[row_pos][index];
	//Make a "composite" range of the pointers to the start
	//and end of our data
	//Note the casts to pointers to Excel Ranges
	Excel::RangePtr pTotalRange =
		pSheet->Range[(Excel::Range*)pBeginRange][(Excel::Range*)pEndRange];

	// Create the chart as a separate chart item in the workbook
	Excel::_ChartPtr pChart = xl->ActiveWorkbook->Charts->Add();
	//Use the ChartWizard to draw the chart.
	//The arguments to the chart wizard are

	pChart->ChartWizard(
		(Excel::Range*)pTotalRange,//Source: the data range,
		(long)Excel::xlXYScatter,//Gallery: the chart type,
		6L, //Format: a chart format (number 1-10),
		(long)Excel::xlColumns, //PlotBy: whether the data is stored in columns or rows,
		1L, //CategoryLabels: an index for the number of columns containing category (x) labels (because our first column of data represents the x values, we must set this value to 1)
		1L, // 	//SeriesLabels: an index for the number of rows containing series (y) labels (our first row contains y labels, so we set this to 1)
		true,//HasLegend: boolean set to true to include a legend
		"Measurement", //Title: the title of the chart
		"Time(s)", //CategoryTitle: the x-axis title
		"Analog0"//ValueTitle: the y-axis title
	);

	//Give the chart sheet a name
	pChart->Name = "Graph";


		// Save the file

	//wchar_t filename[64] = L"Export.xlsx";
	//wchar_t path[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH- wcslen(L"\\") - wcslen(filename), path);
	//wcscat_s(path, L"\\");
	//wcscat_s(path, filename);
	//Excel::XlSaveAsAccessMode ptrmode = (Excel::XlSaveAsAccessMode)1;
	//xl->ActiveWorkbook->SaveAs(path, const _variant_t& FileFormat, const _variant_t& Password, const _variant_t& WriteResPassword, const _variant_t& ReadOnlyRecommended, const _variant_t& CreateBackup, enum XlSaveAsAccessMode AccessMode, const _variant_t& ConflictResolution, const _variant_t& AddToMru, const _variant_t& TextCodepage, const _variant_t& TextVisualLayout, const _variant_t& Local, long lcid);
	//xl->ActiveWorkbook->SaveAs(path,NULL,NULL,NULL,NULL,NULL, ptrmode,NULL,NULL,NULL,NULL,NULL,FALSE);
	//xl->Save();

		// Exit Excel

	//xl->Quit();

	std::cout << "[*] Xlsx file created with success.\n";
	return 0;
}
*/