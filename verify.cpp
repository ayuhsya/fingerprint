#include <iostream>
#include "include/NBioAPI.h"
#include <fstream>
#include <string.h>

// Defining variables
NBioAPI_HANDLE g_hBSP;
NBioAPI_DEVICE_ID m_DeviceID;
NBioAPI_RETURN ret;

// Main function
int main(){

	// Retrieve data from saved file
	NBioAPI_FIR_TEXTENCODE g_TextFIR; // Set input FIR.

	int length;
	std::ifstream t("timestamp.txt");
	std::string text_stream((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());	

	length = strlen(text_stream.c_str());


		if ( NBioAPI_Init(&g_hBSP) != NBioAPIERROR_NONE ){
		std::cout<<"Init Failed";
		return 0;
		}
		else{
		  	// Open device, device auto detect
			m_DeviceID = NBioAPI_DEVICE_ID_AUTO;
			ret = NBioAPI_OpenDevice(g_hBSP, m_DeviceID);
			
			// Open device
			if ( ret != NBioAPIERROR_NONE ){
				std::cout<<"Device open failed";
				return 0;
			}
			else{
			  	std::cout<<"Device successfully initiated.\nPlace finger on sensor.\n";
				
				//Enroll Fingerprint variables
				NBioAPI_FIR_HANDLE g_hCapturedFIR, g_hCapturedFIR2;
				NBioAPI_FIR_PURPOSE m_Purpose;

				// Fill g_TextFIR structure
				g_TextFIR.IsWideChar = NBioAPI_FALSE;			// It depends on application
				g_TextFIR.TextFIR = new NBioAPI_CHAR [length + 1];
				memcpy(g_TextFIR.TextFIR, text_stream.c_str(), length + 1);

				NBioAPI_INPUT_FIR inputFIR;						// Set input FIR.
				NBioAPI_BOOL result;
				inputFIR.Form = NBioAPI_FIR_FORM_TEXTENCODE;	// Set input FIR to text encoded FIR
				inputFIR.InputFIR.TextFIR = &g_TextFIR;

				if ( g_hBSP != NBioAPI_INVALID_HANDLE){	// Check NBioBSP handle
					ret = NBioAPI_Verify(
					g_hBSP,					// Handle of NBioBSP module
					&inputFIR,				// Stored FIR
					&result,				// Result of verification
					NULL,					// Payload in FIR
					10000,					// Timeout for scan image
					NULL,					// audit data
					NULL);					// Window option
				}

				if ( result == NBioAPI_TRUE)
					std::cout<<"Verified\n";
				else
					std::cout<<"Verification Failed";
			}
		}
	return 0;
}