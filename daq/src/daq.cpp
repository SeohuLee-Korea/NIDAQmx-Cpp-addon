#include "daq.h"

daq::daq() {
	DAQmxCreateTask("", &taskHandle); // Create input task
};
daq::~daq() {
	DAQmxClearTask(taskHandle); // Clear input task
};

void daq::initDAQ(string _deviceName, int _numChannel, float64 _samplingRate, string _fileName, float64 _timeout) {
	deviceName = _deviceName;
	numChannel = _numChannel;
	samplingRate = _samplingRate;
	fileName = _fileName;
	timeout = _timeout;
	bInit = true;
}

void daq::setFileName(string _fileName) {
	if (!bInit) {
		cerr << "Error: DAQ is not initialized." << endl;
	}
	fileName = _fileName;
}

void daq::readAnalogVoltage(float64 _inputMinVoltage, float64 _inputMaxVoltage, float64 _runningTime) {
	if (!bInit) {
		cerr << "Error: DAQ not initialized." << endl;
		return;
	}
	if (bData) {
		cerr << "Error: Data buffer is not empty. Please clear first." << endl;
		return;
	}

	inputMinVoltage = _inputMinVoltage;
	inputMaxVoltage = _inputMaxVoltage;
	runningTime = _runningTime;

	// Start a task
	cout << "Start finite reading analog voltage" << endl << endl;
	cout << "------------------------------------------" << endl;

	// Create analog input (AI) voltage channels
	string physicalInputChannels = "";
	for (int i = 0; i < numChannel; i++) {
		physicalInputChannels += deviceName + "/ai" + to_string(i) + ",";
	}
	physicalInputChannels.erase(physicalInputChannels.size() - 1); // Delete the redundant last comma
	cout << "selected input channel(s): " << physicalInputChannels << endl;

	DAQmxCreateAIVoltageChan(
		taskHandle,
		physicalInputChannels.c_str(),
		"",
		DAQmx_Val_Diff, // Measure the difference between (+) and (-) ports
		inputMinVoltage, // Minimum expected voltage to read
		inputMaxVoltage, // Maximum expected voltage to read
		DAQmx_Val_Volts, // unit: voltage
		NULL);

	// Set sample clock timing
	uInt64 numSamplesPerChannel = static_cast<uInt64> (samplingRate * runningTime);
	DAQmxCfgSampClkTiming(
		taskHandle,
		"",
		samplingRate,
		DAQmx_Val_Rising,
		DAQmx_Val_FiniteSamps,
		numSamplesPerChannel);
	    // DAQmx_Val_Rising: acquire a sample at the rising edge of clock
	    // DAQmx_Val_FiniteSamps : collect finite # of samples
	cout << "intended sampling rate: " << samplingRate << endl;

	// Check the desired sampling rate is correctly congifured
	float64 actualSamplingRate;
	DAQmxGetSampClkRate(taskHandle, &actualSamplingRate);
	cout << "actual sampling rate: " << actualSamplingRate << endl << endl;
	cout << "------------------------------------------" << endl;

	// Read data
	cout << "reading data..." << endl;
	uInt64 numTotalSamples = numChannel * numSamplesPerChannel; // Get total # of samples measured
	data = new float64[numTotalSamples]; // Allocate data buffer
	int32 numSuccessfullySampledData = 0; // Variable for restoring successfully sampled data

	cout << "\t" << "# of samples per channel meaing to read: " << numSamplesPerChannel << endl;

	DAQmxStartTask(taskHandle); // Start input task
	auto startReading = chrono::high_resolution_clock::now();

	DAQmxReadAnalogF64(
		taskHandle,
		static_cast<int32>(numSamplesPerChannel),
		timeout,
		DAQmx_Val_GroupByScanNumber,
		data, // restored data (array type)
		static_cast<uInt32>(numTotalSamples), // size of data array
		&numSuccessfullySampledData,
		NULL);
	auto endReading = chrono::high_resolution_clock::now();

	cout << "\t" << "# of samples per channel successfully read: " << numSuccessfullySampledData << endl;

	DAQmxStopTask(taskHandle); // stop input task

	auto readingDuration = chrono::duration_cast<chrono::milliseconds>(endReading - startReading);
	cout << "\t" << "elapsed time: " << readingDuration.count() / 1000. << " seconds" << endl << endl;
	cout << "------------------------------------------" << endl;

	bData = true;
}

//void daq::readAnalogVoltageCont(float64 _inputMinVoltage, float64 _inputMaxVoltage) {
//	if (!bInit) {
//		cout << "DAQ not initialized. Quit readAnalogVoltage()" << endl;
//		return;
//	}
//	if (bData) {
//		cout << "Data is already restored in the buffer. Please clear first. Quit readAnalogVoltageCont()." << endl;
//		return;
//	}
//	inputMinVoltage = _inputMinVoltage;
//	inputMaxVoltage = _inputMaxVoltage;
//
//	// Start a task
//	cout << "Start continuous reading analog voltage" << endl << endl;
//	cout << "------------------------------------------" << endl;
//
//	// Create analog input (AI) voltage channels
//	string physicalInputChannels = "";
//	for (int i = 0; i < numChannel; i++) {
//		physicalInputChannels += deviceName + "/ai" + to_string(i) + ",";
//	}
//	physicalInputChannels.erase(physicalInputChannels.size() - 1); // Delete the redundant last comma
//	cout << "selected input channel(s): " << physicalInputChannels << endl;
//
//	DAQmxCreateAIVoltageChan(
//		taskHandle,
//		physicalInputChannels.c_str(),
//		"",
//		DAQmx_Val_Diff, // Measure the difference between (+) and (-) ports
//		inputMinVoltage, // Minimum expected voltage to read
//		inputMaxVoltage, // Maximum expected voltage to read
//		DAQmx_Val_Volts, // unit: voltage
//		NULL);
//
//	// Set sample clock timing
//	uInt64 numSamplesPerChunk = samplingRate / 100.;
//	DAQmxCfgSampClkTiming(
//		taskHandle,
//		"",
//		samplingRate,
//		DAQmx_Val_Rising,
//		DAQmx_Val_ContSamps,
//		numSamplesPerChunk);
//
//	// DAQmx_Val_Rising: acquire a sample at the rising edge of clock
//	// DAQmx_Val_FiniteSamps : collect finite # of samples
//	cout << "intended sampling rate: " << samplingRate << endl;
//
//	// Check the desired sampling rate is correctly congifured
//	float64 actualSamplingRate;
//	DAQmxGetSampClkRate(taskHandle, &actualSamplingRate);
//	cout << "actual sampling rate: " << actualSamplingRate << endl << endl;
//	cout << "------------------------------------------" << endl;
//
//	DAQmxRegisterEveryNSamplesEvent(taskHandle, DAQmx_Val_Acquired_Into_Buffer, numSamplesPerChunk, 0, everyNCallback, NULL);
//	DAQmxRegisterDoneEvent(taskHandle, 0, doneCallback, NULL);
//	DAQmxStartTask(taskHandle);
//}

void daq::saveData() {
	if (!bInit) {
		cerr << "Error: DAQ is not initialized." << endl;
		return;
	}
	if (!bData) {
		cerr << "Error: No data in buffer. Please read first." << endl;
		return;
	}

	// open file
	string filePath = "./data/" + fileName;
	ofstream csv(filePath);
	if (!csv.is_open()) {
		cerr << "The file is open, cannot write the data. Please close the file." << endl;
		return;
	}

	cout << "writing data..." << endl;

	// write labels
	auto startWriting = chrono::high_resolution_clock::now();

	string label = "time";
	for (int i = 0; i < numChannel; i++) {
		label += ",ch" + to_string(i);
	}
	csv << label << endl;

	// write data
	float64 time = 0.;
	for (int i = 0; i < static_cast<uInt64> (samplingRate * runningTime); i++) {
		time += 1. / samplingRate;
		csv << time << ",";
		for (int j = 0; j < numChannel; j++) {
			csv << data[i * numChannel + j] << ",";
		}
		csv << endl;
	}
	csv.close();

	auto endWriting = chrono::high_resolution_clock::now();

	auto writingDuration = chrono::duration_cast<chrono::milliseconds>(endWriting - startWriting);
	cout << "\t" << "elapsed time: " << writingDuration.count() / 1000. << " seconds" << endl << endl;
}

float64* daq::getData() {
	if (!bInit) {
		cerr << "Error: DAQ is not initialized." << endl;
		return nullptr;
	}
	if (!bData) {
		cerr << "Error: No data in buffer. Please read first." << endl;
		return nullptr;
	}

	return data;
}

void daq::clearData() {
	if (!bInit) {
		cerr << "Error: DAQ is not initialized." << endl;
		return;
	}
	if (!bData) {
		cerr << "Error: No data in buffer. Please read first." << endl;
		return;
	}

	delete[] data; // free the buffer
}



//-----------------------------------------------------------------------------------------------------------------
//int32 CVICALLBACK daq::everyNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void* callbackData) {
//	daq* instance = static_cast<daq*>(callbackData);
//	
//	uInt64 numSamplesPerChunk = static_cast<uInt64> (instance->samplingRate / 100.);
//	uInt64 numTotalSamples = instance->numChannel * numSamplesPerChunk;
//	int32 numSuccessfullySampledData = 0;
//	
//	static int32 totalRead = 0;
//	DAQmxReadAnalogF64(
//		taskHandle,
//		static_cast<int32>(numSamplesPerChunk),
//		instance->timeout,
//		DAQmx_Val_GroupByScanNumber,
//		instance->data, // restored data (array type)
//		static_cast<uInt32>(numTotalSamples), // size of data array
//		&numSuccessfullySampledData,
//		NULL);
//	cout << "The number of data acquired: " << totalRead + numSuccessfullySampledData << flush;
//	return 0;
//}
//
//int32 CVICALLBACK daq::doneCallback(TaskHandle taskHandle, int32 status, void* callbackData) {
//	return 0;
//}