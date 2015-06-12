#include <iostream>
#include "include/NBioAPI.h"
#include <fstream>
#include <time.h>
#include <string.h>

// Defining variables
NBioAPI_HANDLE g_hBSP;
NBioAPI_DEVICE_ID m_DeviceID;
NBioAPI_RETURN ret;

// Main function
int main(){

	// Initialize device
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
			NBioAPI_FIR_HANDLE g_hCapturedFIR;
			NBioAPI_FIR_PURPOSE m_Purpose;
	
			m_Purpose = NBioAPI_FIR_PURPOSE_ENROLL;
			ret = NBioAPI_Capture(
				g_hBSP,           // Handle of NBioBSP Module
				m_Purpose,        // Purpose of capture
				&g_hCapturedFIR,  // Handle of captured FIR
				3910000,          // Capture timeout
				NULL,             // Audit data
				NULL              // Window option
				);
			std::cout<<"Print recorded\n";
			
			/*
			if ( ret == NBioAPIERROR_NONE ){
				int length;
				length = strlen(g_firText.TextFIR);

				if (g_firText.IsWideChar == NBioAPI_TRUE)
				text_stream = new char [(length + 1)*2];
				else
				text_stream = new char [length + 1];
	
				memcpy(text_stream, g_firText.TextFIR, length + 1);
	
				// Save text_stream to File or Database
				std::ofstream fpdata;
				fpdata.open ("./timestamp2.txt");
				fpdata << text_stream;
				fpdata.close();
	
				delete [] text_stream;
				*/
	
			//NBioAPI_FreeTextFIR(g_hBSP, &g_firText); // Free TextFIR handle
			
			//std::cout<<"FingerPrint saved.";
			NBioAPI_BOOL result;
			NBioAPI_INPUT_FIR storedFIR, inputFIR;

			// Read stored data and convert into FIR(fir1)
			std::ifstream t("timestamp.txt");
			std::string text_stream((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());
			int length;

			length = strlen(text_stream.c_str());

			NBioAPI_FIR_TEXTENCODE g_storedTextFIR; // Set input FIR.
			g_storedTextFIR.IsWideChar = NBioAPI_FALSE;			// It depends on application
			g_storedTextFIR.TextFIR = new NBioAPI_CHAR [length + 1];
			memcpy(g_storedTextFIR.TextFIR, text_stream.c_str(), length + 1);

			storedFIR.Form = NBioAPI_FIR_FORM_TEXTENCODE; // stored FIR
			storedFIR.InputFIR.TextFIR = &g_storedTextFIR;
			
			// Read input stream and convert into FIR(fir2)

			// Text encode the FIR generated
			NBioAPI_FIR_TEXTENCODE g_newTextFIR; // Text encoded FIR
	
			// Get Text encoded FIR from FIR handle
			ret = NBioAPI_GetTextFIRFromHandle(g_hBSP, g_hCapturedFIR, &g_newTextFIR, NBioAPI_FALSE);

			inputFIR.Form = NBioAPI_FIR_FORM_TEXTENCODE; // input FIR to be compared
			inputFIR.InputFIR.TextFIR = &g_newTextFIR;

			ret = NBioAPI_VerifyMatch( // Matching use with stored FIR
			g_hBSP,		// Handle of NBioBSP module
			&storedFIR,	// stored FIR
			&inputFIR,	// input FIR for matching
			&result,	// Result of matching
			NULL	// Payload
			);
			if ( result == NBioAPI_TRUE)
			std::cout<<"Matched with stored.\n";
	}
	}
	return 0;
}

	
