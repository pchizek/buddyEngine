/*
 * Loader.h
 * For loading level data from xml files using the tinyxml2 library
 * 
 * Written by: Patrick Chizek	https://github.com/pchizek
 * tinyxml2 by: Lee Thomason	https://github.com/leethomason 
 */

#ifndef _LOADER_H
#define _LOADER_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "tinyxml2.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "..//managers/objectManager2d.h"

using namespace std;
using namespace tinyxml2;

class node {

public:
	string data;
	node* next;

};

int extractElementSet(node* firstRef, XMLElement* parentElement, const char elementName[]);

int parse(string inputString, int tupleArray[2]);

int loadLevel(const char filename[]);

#endif