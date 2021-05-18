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
#include "xmlTools.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "..//managers/objManager2d.h"

using namespace std;
using namespace tinyxml2;
using namespace engine;

void loadAsset(XMLElement* assetElement);
void loadObject(XMLElement* objectElement);

void loadAssets(XMLDocument* levelDoc);
void loadEnvironment(XMLDocument* levelDoc);

void loadLevel(const char filename[]);

#endif