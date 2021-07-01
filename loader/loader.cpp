/*
 * Loader.cpp
 * For loading level data from xml files using the tinyxml2 library
 *
 * Written by: Patrick Chizek	https://github.com/pchizek
 * tinyxml2 by: Lee Thomason	https://github.com/leethomason
 */

#include <stdio.h>
#include <iostream>
#include <string>
#include "tinyxml2.h"
#include "loader.h"
#include <unordered_map>
#include "..//managers/objManager2d.h"
#include "xmlTools.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define ASSET_PATH "resources\\textures\\"

using namespace std;
using namespace tinyxml2;
using namespace engine;

/* Loader for individual assets, objects */
#pragma region load_element

void loadAsset(XMLElement* assetElement) {

	XMLElement* assetChildElement;
	
	assetChildElement = assetElement->FirstChildElement("file");
	const char* fileName = assetChildElement->GetText();

	// Get filename
	const char* aliasName = assetElement->Attribute("alias");
	string assetKey;
	if (aliasName) {
		assetKey = string(aliasName);
	}
	else {
		assetKey = string(fileName);
		assetKey = assetKey.substr(0,assetKey.find("."));
	}
	string texturePath = string(ASSET_PATH).append(fileName);
	
	// Call function to import the texture
	engine::importTexture(texturePath, assetKey);

}

void loadObject(XMLElement* objectElement) {

	// Access the unordered map of textures
	XMLElement* objectChildElement;

	/* World location under <grid> or <coords> */
	int worldLoc[2];

	if (objectElement->FirstChildElement("coords")) {
		
		objectChildElement = objectElement->FirstChildElement("coords");

		// Extract from document 
		parse(objectChildElement->GetText(),worldLoc,2);

	}
	else if (objectElement->FirstChildElement("grid")){
		
		objectChildElement = objectElement->FirstChildElement("grid");

		// Extract from document
		parse(objectChildElement->GetText(),worldLoc,2);

		worldLoc[0] *= BLOCK_SIZE;
		worldLoc[1] *= BLOCK_SIZE;

	}
	else {
		exception("Object: Invalid location");
	}

	/* Layer */
	objectChildElement = objectElement->FirstChildElement("layer");
	int objLayer{};

	if (objectChildElement) {

		parse(objectChildElement->GetText(), &objLayer);
		if ((objLayer >= MAX_LAYERS) || (objLayer < 0)) exception("Object: Invalid layer value");

	}
	else {
		exception("Object: Missing layer");
	}

	/* Texture */
	objectChildElement = objectElement->FirstChildElement("texture");
	sf::Texture* objectTexture{};

	if (objectChildElement) {

		// Get the asset from the map
		unordered_map<string, sf::Texture>::iterator asset = engine::textureMap.find(objectChildElement->GetText());
	
		// Check if asset is valid
		if (asset == engine::textureMap.end()) {
			exception("Object: Texture not found");
		}
		else {
			objectTexture = &asset->second;
		}
	
	}
	else {
		exception("Object: Invalid texture");
	}

	/* Block selection or rectangle */
	int spriteRect[4] = {0,0,0,0};

	if (objectElement->FirstChildElement("rect")) {

		objectChildElement = objectElement->FirstChildElement("rect");

		// Extract from document
		parse(objectChildElement->GetText(), spriteRect, 4);

	}

	else if (objectElement->FirstChildElement("blockSelect")) {
		int blockSelect;

		objectChildElement = objectElement->FirstChildElement("blockSelect");

		// Extract from document
		parse(objectChildElement->GetText(), &blockSelect);

		// Set texture rectangle
		spriteRect[0] = (32 * blockSelect);
		spriteRect[1] = 0;
		spriteRect[2] = 32;
		spriteRect[3] = 32;

	}

	else {

		// Set texture rectangle
		spriteRect[0] = 0;
		spriteRect[1] = 0;
		
		// Get size of rectangle as full size of texture
		sf::Vector2u fullSize = objectTexture->getSize(); // Dereferences null pointer but could null pointer ever get here?
		spriteRect[2] = fullSize.x;
		spriteRect[3] = fullSize.y;
	}

	sf::IntRect sRect(spriteRect[0], spriteRect[1], spriteRect[2], spriteRect[3]);

	/* Construct object */
	engine::object* newObject = new engine::object(worldLoc, objLayer, objectTexture, &sRect);

	/* 
	 * Add scripts 
	 * (these may or may not apply to a given object so they
	 * have to be added to the object already constructed) 
	 */
	if (objectElement->FirstChildElement("script")) {
		objectChildElement = objectElement->FirstChildElement("script");
		newObject->setScript(objectChildElement);
	}

	/* Emplace object */
	newObject->prevObject = engine::lastObject;
	if (engine::firstObject == NULL) {
		engine::firstObject = newObject;
	}
	else {
		newObject->prevObject->nextObject = newObject;
	}
	//newObject->prevObject = engine::lastObject;

	engine::lastObject = newObject;

}

#pragma endregion

/* Loader for assets section, environment */
#pragma region load_section

/* Loads an entire level into memory from an xml file
 *
 * Arguments:
 * filename: a char array that is the name of the file to be loaded
 *
 * Returns: 0 on success, error codes
 */

void loadAssets(XMLDocument* levelDoc) {

	/* Get first asset in document */
	XMLElement* asset =
		levelDoc->FirstChildElement("level")->
		FirstChildElement("assets")->
		FirstChildElement("asset");

	while (asset) {

		loadAsset(asset);
		asset = asset->NextSiblingElement();

	}

}

void loadEnvironment(XMLDocument* levelDoc) {

	/* Get first object in document */
	XMLElement* object =
		levelDoc->FirstChildElement("level")->
		FirstChildElement("environment")->
		FirstChildElement("object");

	while (object) {

		loadObject(object);
		object = object->NextSiblingElement();

	}
}

#pragma endregion

/* Loader for entire level */
#pragma region load_level
void loadLevel(const char filename[]) {

	XMLDocument levelDoc;

	// Open file
	XMLError loadError = levelDoc.LoadFile(filename);

	// Throw error
	if (loadError) {
		exception("Level: Cannot open document");
	}

	// Load Assets
	loadAssets(&levelDoc);

	// Load Objects
	loadEnvironment(&levelDoc);

}
#pragma endregion