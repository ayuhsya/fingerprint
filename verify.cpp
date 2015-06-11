#include <iostream>
#include "include/NBioAPI.h"
#include <fstream>
#include <string.h>
#include <cstring>
#include <stdlib.h>

// Defining variables
NBioAPI_HANDLE g_hBSP;
NBioAPI_DEVICE_ID m_DeviceID;
NBioAPI_RETURN ret;

// Main function
int main(){

	// Retrieve data from saved file
	NBioAPI_FIR_TEXTENCODE g_TextFIR; // Set input FIR.
	int length;

	// fill text_stream buffer from file or database
	std::ifstream t("timestamp.txt");
	std::string text_stream((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());	

	length = strlen(text_stream.c_str());

	// Fill g_TextFIR structure
	g_TextFIR.IsWideChar = NBioAPI_FALSE;			// It depends on application
	g_TextFIR.TextFIR = new NBioAPI_CHAR [length + 1];
	memcpy(g_TextFIR.TextFIR, text_stream.c_str(), length + 1);

	NBioAPI_INPUT_FIR inputFIR;						// Set input FIR.
	NBioAPI_BOOL result;
	inputFIR.Form = NBioAPI_FIR_FORM_TEXTENCODE;	// Set input FIR to text encoded FIR
	inputFIR.InputFIR.TextFIR = &g_TextFIR;

	if ( g_hBSP != NBioAPI_INVALID_HANDLE)	// Check NBioBSP handle
	ret = NBioAPI_Verify(
	g_hBSP,					// Handle of NBioBSP module
	&inputFIR,				// Stored FIR
	&result,				// Result of verification
	NULL,					// Payload in FIR
	10000,					// Timeout for scan image
	NULL,					// audit data
	NULL);					// Window option

	if ( result == NBioAPI_TRUE)
		std::cout<<"Verified";
	else
		std::cout<<"Verification Failed";
	return 0;
}