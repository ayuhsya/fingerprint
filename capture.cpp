#include <iostream>
#include "NBioAPI.h"
#include <fstream>
#include <time.h>
#include <string>

// Defining variables
NBioAPI_HANDLE g_hBSP;
NBioAPI_DEVICE_ID m_DeviceID;
NBioAPI_RETURN ret;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss for timestamp
const std::string currentDateTime() {
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

  return buf;
}

// Main function
int main(){

// Initialize device
if ( NBioAPI_Init(&g_hBSP) != NBioAPIERROR_NONE ){
	std::cout<<"Init Failed";
	return;
	}
else{
  // Open device, device auto detect
	m_DeviceID = NBioAPI_DEVICE_ID_AUTO;
	ret = NBioAPI_OpenDevice(g_hBSP, m_DeviceID);
	
	// Open device
	if ( ret != NBioAPIERROR_NONE ){
		std::cout<<"Failed";
		return;
		}
	else{
	  std::cout<<"Device successfully initiated.\nPlace finger on sensor.\n";
		
		//Enroll Fingerprint variables
		NBioAPI_FIR_HANDLE g_hCapturedFIR, g_hCapturedFIR2;
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
		
		// Text encode the FIR generated

		NBioAPI_FIR_TEXTENCODE g_firText; // Text encoded FIR

		// Get Text encoded FIR from FIR handle
		ret = NBioAPI_GetTextFIRFromHandle(g_hBSP, g_hCapturedFIR, &g_firText, NBioAPI_FALSE);
		if ( ret == NBioAPIERROR_NONE ){
			char* text_stream;
			int length;
			length = strlen(g_firText.TextFIR);
			std::cout<<length;
			if (g_firText.IsWideChar == NBioAPI_TRUE)
			text_stream = new char [(length + 1)*2];
			else
			text_stream = new char [length + 1];

			memcpy(text_stream, g_firText.TextFIR, length + 1);


	
