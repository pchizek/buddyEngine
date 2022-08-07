#pragma once

#include <vector>
#include <stdint.h>
typedef enum {

	DC,
	AC

} signalType;

class signal{
public:

	signal();
	uint16_t signalNumber;


	enum signalType;
	float voltage;

	vector<uint8_t> generators;
	vector<uint8_t> sinks;

	

private:

};


class designBlock {
public:

	uint16_t designBlockNumber;

	//callback for design block function


private:

};


class fault {

public:

	uint16_t firstSignalNumber;
	uint16_t offendingDesignBlockNumber;
	uint8_t offendingComponentNumber;

	// store good/bad value of variable changed for fault

private:

};