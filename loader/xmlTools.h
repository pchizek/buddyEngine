/*
 * xmlTools.h
 * Functions for interfacing with XML files using tinyxml2
 *
 * Written by: Patrick Chizek	https://github.com/pchizek
 * tinyxml2 by: Lee Thomason	https://github.com/leethomason
 */
#pragma once

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "tinyxml2.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace tinyxml2;

void parse(const char* inputChar, int* outputInt);
void parse(const char* inputChar, int outputArr[], int outputArrSize);
void parse(const char* inputChar, float outputArr[], int outputArrSize);
void parse(string inputString, vector<int>& outputVector);
void parse(const char* inputChar, array<int,3> &outputStdArr);
void parse(const char* inputChar, array<int,5> &outputStdArr);