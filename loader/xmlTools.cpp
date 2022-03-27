/*
 * xmlTools.cpp
 * Functions for interfacing with XML files using tinyxml2
 *
 * Written by: Patrick Chizek	https://github.com/pchizek
 * tinyxml2 by: Lee Thomason	https://github.com/leethomason
 */

#include <stdio.h>
#include <iostream>
#include <string>
#include "tinyxml2.h"
#include <unordered_map>
#include <array>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <xmlTools.h>

#define ASSET_PATH "resources\\textures\\"

using namespace std;
using namespace tinyxml2;

//#pragma region parsing
/* Parses a string to extract various integer and array values
 *
 * Arguments:
 * inputString: an object of type std::string that values will be extracted from
 *
 * (Overloaded)
 * tupleArray: an array of 2 integers (such as 2D coordinates)
 * num: a single integer
 *
 *
 * Returns: 0 on success, -1 on error
 */

void parse(const char* inputChar, int* outputInt) {

	*outputInt = stoi(inputChar);

}

void parse(const char* inputChar, int outputArr[], int outputArrSize) {

	string inputString(inputChar);
	size_t len = inputString.length();

	if ((inputString[0] == '[') && (inputString[len - 1] == ']')) {

		len = inputString.length();

		size_t thisComma = 0, nextComma;

		for (int i = 0; i < outputArrSize; i++) {

			if (thisComma == string::npos) {
				exception("Bad Array: Invalid length");
			}

			nextComma = inputString.find(',', thisComma + 1);

			if (nextComma != string::npos) {
				outputArr[i] = stoi(inputString.substr(thisComma + 1, (nextComma - thisComma)));
			}
			else {
				outputArr[i] = stoi(inputString.substr(thisComma + 1, (len - 2 - thisComma)));
			}

			thisComma = nextComma;

		}

	}
	else {
		exception("Bad Array: Syntax");
	}

}

void parse(const char* inputChar, float outputArr[], int outputArrSize) {

	string inputString(inputChar);
	size_t len = inputString.length();

	if ((inputString[0] == '[') && (inputString[len - 1] == ']')) {

		len = inputString.length();

		size_t thisComma = 0, nextComma;

		for (int i = 0; i < outputArrSize; i++) {

			if (thisComma == string::npos) {
				exception("Bad Array: Invalid length");
			}

			nextComma = inputString.find(',', thisComma + 1);

			if (nextComma != string::npos) {
				outputArr[i] = stof(inputString.substr(thisComma + 1, (nextComma - thisComma)));
			}
			else {
				outputArr[i] = stof(inputString.substr(thisComma + 1, (len - 2 - thisComma)));
			}

			thisComma = nextComma;

		}

	}
	else {
		exception("Bad Array: Syntax");
	}

}

#pragma region string_inputs

void parse(string inputString, vector<int>& outputVector) {

	// Get size of vector
	size_t vectorSize = count(inputString.begin(),inputString.end(),',')+1;
	size_t len = inputString.length();

	if ((inputString[0] == '[') && (inputString[len - 1] == ']')) {

		size_t thisComma = 0, nextComma;

		for (int i = 0; i < vectorSize; i++) {

			if (thisComma == string::npos) {
				exception("Bad Vector: Invalid length");
			}

			nextComma = inputString.find(',', thisComma + 1);

			if (nextComma != string::npos) {
				outputVector.push_back(stoi(inputString.substr(thisComma + 1, (nextComma - thisComma))));
			}
			else {
				outputVector.push_back(stoi(inputString.substr(thisComma + 1, (len - 2 - thisComma))));
			}

			thisComma = nextComma;

		}

	}
	else {
		exception("Bad Array: Syntax");
	}

}

elementType getElementType(string inputString) {

	// Evaluate string
	
	// Check if string is integer(s)
	if (inputString.find("[") != string::npos) {

		if (inputString.find(",") != string::npos) {
			return arg_t_int_v;
		}
		else {
			return arg_t_int;
		}

	}

	// Check if string is float(s)
	else if (inputString.find("{") != string::npos) {
		
		if (inputString.find(",") != string::npos) {
			return arg_t_float_v;
		}
		else {
			return arg_t_float;
		}

	}

	// Check if string is meant to be text
	else if (inputString.find('"') != string::npos) {
		return arg_t_string;
	}

	else {
		exception("Error: Element of variable type defined incorrectly");
	}

}

#pragma endregion

void parse(const char* inputChar, array<int, 3> &outputStdArr) {

	int interArr[3];
	parse(inputChar, interArr, 3);
	
	for (int i = 0; i < 3; i++) {
		outputStdArr.at(i) = interArr[i];
	}
}

void parse(const char* inputChar, array<int, 5> &outputStdArr) {

	int interArr[5];
	parse(inputChar, interArr, 5);

	for (int i = 0; i < 5; i++) {
		outputStdArr.at(i) = interArr[i];
	}

}

//#pragma endregion