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
#include <objManager2d.h>
#include <xmlTools.h>
#include <controls.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define ASSET_PATH "resources\\textures\\"
//#define SCALEFACTOR 1920.0/320.0

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

	// Get asset scale
	float assetScaleArray[2];
	assetChildElement = assetChildElement->NextSiblingElement("scale");
	if (assetChildElement){
		parse(assetChildElement->GetText(), assetScaleArray, 2);
	}
	else {
		assetScaleArray[0] = 1.0f;
		assetScaleArray[1] = 1.0f;
	}
	sf::Vector2f scaleFactor;
	scaleFactor.x = assetScaleArray[0];
	scaleFactor.y = assetScaleArray[1];


	// Call function to import the texture
	engine::importTexture(texturePath, assetKey, scaleFactor);

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
	textureInfo* objectTextureInfo = NULL;

	if (objectChildElement) {

		// Get the asset from the map
		unordered_map<string, textureInfo>::iterator asset = engine::textureInfoMap.find(objectChildElement->GetText());
	
		// Check if asset is valid
		if (asset == engine::textureInfoMap.end()) {
			exception("Object: Texture not found");
		}
		else {
			objectTextureInfo = &asset->second;
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
		sf::Vector2u fullSize = objectTextureInfo->assetTexture.getSize(); // Dereferences null pointer but could null pointer ever get here?
		spriteRect[2] = fullSize.x;
		spriteRect[3] = fullSize.y;
	}



	sf::IntRect sRect(spriteRect[0], spriteRect[1], spriteRect[2], spriteRect[3]);


	/* Construct object */
	engine::object* newObject = new engine::object(worldLoc, objLayer, objectTextureInfo, &sRect);

	/* Set object scale factor */
	float localScaleFactorArr[2];
	sf::Vector2f localScaleFactor;
	if (objectElement->FirstChildElement("scale")) {
		objectChildElement = objectElement->FirstChildElement("scale");
		parse(objectChildElement->GetText(), localScaleFactorArr, 2);
		localScaleFactor.x = localScaleFactorArr[0];
		localScaleFactor.y = localScaleFactorArr[1];
	}
	else {
		localScaleFactor.x = 1.0f;
		localScaleFactor.y = 1.0f;
	}

	newObject->setLocalScaleFactor(&localScaleFactor);
	newObject->setObjectScaleFactor();

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

#pragma region load_controls_and_ui

void loadControlScheme() {
	
	XMLDocument controlSchemesDoc;
    XMLError loadError = controlSchemesDoc.LoadFile("resources/ui/controlSchemes.xml");

	// Throw error
	if (loadError) {
		exception("Controls: Cannot open document");
	}

	XMLElement* schemeElement = controlSchemesDoc.FirstChildElement("scheme");
	XMLElement* schemeChildElement;

	while (schemeElement) {

		const char* schemeName = schemeElement->Attribute("name");
		string schemeKey;
		if (schemeName) {
			schemeKey = string(schemeName);
		}
		else { exception("Error reading control scheme"); }

		// Get keybinds
		schemeChildElement = schemeElement->FirstChildElement("keybind");

		keybindVector thisKeybindVector;

		while (schemeChildElement) {

			keyControlFunction thisKeyFunction;

			// Get key on keyboard
			thisKeyFunction.key = convertToKey.at(string(schemeChildElement->FirstChildElement("key")->GetText()));

			// Get function callback by name
			string functionName = string(schemeChildElement->FirstChildElement("function")->GetText());
			thisKeyFunction.controlCallback = engine::controlFunctionMap.at(functionName);

			// Get args
			parse(string(schemeChildElement->FirstChildElement("args")->GetText()), thisKeyFunction.args);
			
			// Put this key bind in the vector
			thisKeybindVector.push_back(thisKeyFunction);

			// Next keybind in scheme
			schemeChildElement = schemeChildElement->NextSiblingElement("keybind");
		}

		// Put this keybind vector into map of all keybind schemes
		controlSchemeMap.emplace(schemeKey, thisKeybindVector);

		// Next scheme
		schemeElement = schemeElement->NextSiblingElement("scheme");
	}
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

	// Open level file
	XMLDocument levelDoc;
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

/*Loader for game */
#pragma region load_game
void loadGame() {

	// Load control schemas

	// Load ui

	// Load level (TODO: load start screen)

}



