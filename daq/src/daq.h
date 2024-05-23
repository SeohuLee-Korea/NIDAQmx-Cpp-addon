#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#include "NIDAQmx.h"

using namespace std;

class daq {
public:
	daq();
	~daq();

	/* Setters: must set properties first */
	// DAQ setting. The device name can be found in NIMAX.
	// Channels will be used in ascending order, e.g. if you set numChannel = 2, channel 0 and 1 will be used
	void initDAQ(string _deviceName, int _numChannel, float64 _samplingRate, string _fileName = "result.csv", float64 _timeout = 86400.);

	// Manually set the file name to save the data. The defualt name is "result.csv"
	void setFileName(string _fileName);

	/* Runners: practical operation*/
	// Read analog voltage in finite time.
	void readAnalogVoltage(float64 _inputMinVoltage, float64 _inputMaxVoltage, float64 _runningTime);
	//void readAnalogVoltageCont(float64 _inputMinVoltage, float64 _inputMaxVoltage); // Not developed yet

	// Save the data in .csv file.
	void saveData();

	// Return the data for other implementations
	float64* getData();

	// Clear the data buffer for new measurement.
	void clearData();
	
protected:// Callback functions for continuous reading
	//static int32 CVICALLBACK everyNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void* callbackData);
	//static int32 CVICALLBACK doneCallback(TaskHandle taskHandle, int32 status, void* callbackData);
	// Not developed yet

protected: // Instance variables
	// Name of the device. It has to be manually found in NIMAX
	string deviceName; // e.g. "cDAQ1Mod1"

	// Set input/output channels to use
	// Note: NI9229 only support analog input (AI); AO, DI, DO are not supported
	int numChannel; // Channels will be used in ascending order, e.g. if you set numChannel = 2, channel 0 and 1 will be used
	
	// Set input/output values
	float64 inputMinVoltage; // Input: min value to measure (related to resolution)
	float64 inputMaxVoltage; // Input: max value to measure (related to resolution)
	float64 outputVoltage; // Iutput: value to generate

	// Set other variables
	float64 samplingRate; // Rate of sampling. Need to be in the range; NI9229 ranges in minimum 1.613k ~ maximum 50k
	float64 runningTime; // Time of measuring (unit: second)
	float64 timeout; // Maximum running time. Need sufficiently long time. (unit: second)

	// Set the name of a file to write data
	string fileName; // name of file to write data

	// Buffer to save data
	float64* data;

	// boolean checkers
	bool bInit = false;
	bool bData = false;

protected: // Task handle of daq. Do not touch these
	TaskHandle taskHandle;
};
