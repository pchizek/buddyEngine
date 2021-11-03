/*
 * Loader.h
 * For loading level data from xml files using the tinyxml2 library
 * 
 * Written by: Patrick Chizek	https://github.com/pchizek
 * tinyxml2 by: Lee Thomason	https://github.com/leethomason 
 */
#pragma once

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "tinyxml2.h"
#include <xmlTools.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <objManager2d.h>

using namespace std;
using namespace tinyxml2;
using namespace engine;

void loadAsset(XMLElement* assetElement);		/*!< Load the texture asset specified in the level XML file into the map of assets. */
void loadObject(XMLElement* objectElement);		/*!< Load the data for an object from the XML file. */

void loadControlScheme();						/*!< Load the control scheme from the controlSchemes XML file. */

void loadAssets(XMLDocument* levelDoc);			/*!< Load all texture assets in the level XML document. */
void loadEnvironment(XMLDocument* levelDoc);	/*!< Load all objects in the level XML document. */

void loadLevel(const char filename[]);			/*!< Load the entire level file specified. */
