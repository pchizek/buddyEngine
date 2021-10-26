/*
 * objectManager.cpp
 * 3D Object Manager
 *
 * Written by: Patrick Chizek
 * https://github.com/pchizek
 *
 */

#include <objManager2d.h>
#include <xmlTools.h>
#include <tinyxml2.h>
#include <array>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#define FAUXWIDTH 320.0
#define FAUXHEIGHT 180.0

using namespace tinyxml2;
using namespace engine;
using namespace std;

int cameraCoords[2] = { 0,0 };

namespace engine {
	
	object* firstObject;
	object* lastObject;
	unordered_map<string, textureInfo> textureInfoMap;

	/* Texture */
#pragma region texture

	void engine::importTexture(string textureFile, string textureKey, sf::Vector2f scaleFactor) {

		textureInfo assetTextureInfo;

		// Put in scale factor
		assetTextureInfo.assetScaleFactor = scaleFactor;

		// Import actual texture data
		if (!assetTextureInfo.assetTexture.loadFromFile(textureFile)) {
			// Error
			exception("File not found");
		}
		else {
			// Copy into map
			engine::textureInfoMap.emplace(textureKey, assetTextureInfo);
		}

	}
#pragma endregion

	/* Object */
#pragma region object

	/* Constructor */
	object::object(int worldLoc[2], uint8_t objLayer, textureInfo* newTexture, sf::IntRect *spriteRect) {

		/* Set world location */
		worldCoords[0] = worldLoc[0];
		worldCoords[1] = worldLoc[1];

		/* Set sprite texture */
		if (newTexture) {
			objectSprite.setTexture(newTexture->assetTexture);
			
		}

		/* Set texture rectangle/hitbox */
		objectSprite.setTextureRect(*spriteRect);

		objectTextureInfo = newTexture;

		layer = objLayer;

	}

	/* Set Texture */
	void object::setTexture(sf::Texture* newTexture) {
		objectSprite.setTexture(*newTexture);
	}

	/* Set local scale factor*/
	void object::setLocalScaleFactor(sf::Vector2f* scaleFactor) {
		localScaleFactor.x = scaleFactor->x;
		localScaleFactor.y = scaleFactor->y;
	}

	/* Set and update object scale factor*/
	void object::setObjectScaleFactor() {
		objectScaleFactor.x = objectTextureInfo->assetScaleFactor.x * 
			localScaleFactor.x;
		objectScaleFactor.y = objectTextureInfo->assetScaleFactor.y * 
			localScaleFactor.y;
		objectSprite.setScale(objectScaleFactor);
	}

	/* Set Script */
	void object::setScript(XMLElement* scriptElement) {

		bool scriptsToProcess = true;
		XMLElement* nodeElement;
		int numNodes{ 0 };
		int numFrames{ 0 };

		// Get type of script
		if (scriptElement->Attribute("type", "move")) {

			//motionInfoCache motionInfo;
			array<int,3> motionNodesArray{ 0,0,0 };
			
			nodeElement = scriptElement->FirstChildElement("node");
			while (nodeElement) {

				// Increase number of nodes
				numNodes++;

				// Extract node into array
				parse(nodeElement->GetText(), motionNodesArray);

				// put contents of array into vector
				this->motionInfo.motionNodes.push_back(motionNodesArray);

				// Get next node
				nodeElement = nodeElement->NextSiblingElement("node");
				
			}
			
			// Enable movement
			this->motionInfo.numNodes = numNodes;
			this->motionInfo.active = true;
			this->motionInfo.currentNode = 0;

			//Preload first (0) node
			this->motionInfo.dx = motionInfo.motionNodes.at(motionInfo.currentNode)[0];
			this->motionInfo.dy = motionInfo.motionNodes.at(motionInfo.currentNode)[1];
			this->motionInfo.dt = 1 / float(motionInfo.motionNodes.at(motionInfo.currentNode)[2]);

		}
		else if (scriptElement->Attribute("type", "animation")) {
			
			//animInfoCache animInfo;
			array<int, 5> animFramesArray{ 0,0,0,0,0 };
			
			nodeElement = scriptElement->FirstChildElement("frame");
			while (nodeElement) {

				// Increase number of nodes
				numFrames++;

				// Extract node into array
				parse(nodeElement->GetText(), animFramesArray);
				
				// put contents of array into vector
				this->animInfo.animFrames.push_back(animFramesArray);
				
				// Get next node
				nodeElement = nodeElement->NextSiblingElement("frame");
			}
			
			// Enable animation
			this->animInfo.numFrames = numFrames;
			this->animInfo.active = true;
			this->animInfo.currentFrame = 0;

			//Preload first (0) frame
			this->objectSprite.setTextureRect(sf::IntRect(
				animInfo.animFrames.at(animInfo.currentFrame)[0],
				animInfo.animFrames.at(animInfo.currentFrame)[1],
				animInfo.animFrames.at(animInfo.currentFrame)[2],
				animInfo.animFrames.at(animInfo.currentFrame)[3]));

		}
		else {
			exception("Script: Invalid type");
		}

		scriptElement = scriptElement->NextSiblingElement("script");
		if (!scriptElement) {
			scriptsToProcess = false;
		}

	}

	/* Draw */
	void object::draw(sf::RenderWindow* window) {

		// Get external to the program's window
		extern int cameraCoords[2];

		// Resolve animation
		if (animInfo.active) {
			
			// If we've reached the next frame
			if (animInfo.frameCounter == animInfo.animFrames.at(animInfo.currentFrame)[4] ){

				// Reset frame counter
				animInfo.frameCounter = 0;
				animInfo.currentFrame++;

				// Loop the current frame in the row
				if (animInfo.currentFrame == animInfo.numFrames) {
					animInfo.currentFrame = 0;
				}

				// Move actual sprite rectangle to next frame
				objectSprite.setTextureRect(sf::IntRect(
				animInfo.animFrames.at(animInfo.currentFrame)[0],
				animInfo.animFrames.at(animInfo.currentFrame)[1],
				animInfo.animFrames.at(animInfo.currentFrame)[2],
				animInfo.animFrames.at(animInfo.currentFrame)[3]));

			}

			else {
				animInfo.frameCounter++;
			}

		}

		if (motionInfo.active) {

			// If has reached the next node
			if (motionInfo.nodeCounter == motionInfo.motionNodes.at(motionInfo.currentNode)[2]) {

				// Reset counter, move to next node
				motionInfo.nodeCounter = 0;
				motionInfo.currentNode++;

				// Loop the motion, Calculate steps in X and Y, and the dt
				
				if (motionInfo.currentNode == motionInfo.numNodes){ // Reset loop

					motionInfo.currentNode = 0;

					motionInfo.dx = motionInfo.motionNodes.at(motionInfo.currentNode)[0];
					motionInfo.dy = motionInfo.motionNodes.at(motionInfo.currentNode)[1];
					
				}

				else { // Next node

					// Find dx, dy as path to travel to next node
					motionInfo.dx = motionInfo.motionNodes.at(motionInfo.currentNode)[0] -
						motionInfo.motionNodes.at(motionInfo.currentNode - 1)[0];
					motionInfo.dy = motionInfo.motionNodes.at(motionInfo.currentNode)[1] -
						motionInfo.motionNodes.at(motionInfo.currentNode - 1)[1];

				}
			
				motionInfo.dt = 1 / float(motionInfo.motionNodes.at(motionInfo.currentNode)[2]);

			}
			

			else {

				// Interpolate position on path
				motionInfo.scriptOffset[0] += motionInfo.dx * motionInfo.dt;
				motionInfo.scriptOffset[1] += motionInfo.dy * motionInfo.dt;

				motionInfo.nodeCounter++;

			}

		}

		// Set the position of the sprite in the window
		objectSprite.setPosition(sf::Vector2f(
			(float)(this->worldCoords[0] - cameraCoords[0] + round(motionInfo.scriptOffset[0])),
			(float)(this->worldCoords[1] - cameraCoords[1] + round(motionInfo.scriptOffset[1]))));

		// Set it to be drawn in the next window
		window->draw(objectSprite);

	}

#pragma endregion

	/* Render all */
	void engine::renderAll(sf::RenderWindow* window) {

		//object* firstObject;

		for (int i = 0; i < MAX_LAYERS; i++) {

			object* currentObject = firstObject;

			while (currentObject) {
				if (currentObject->layer == i) {
					currentObject->draw(window);
				}
				currentObject = currentObject->nextObject;
			}

		}

	}
	
	/*
	 * Set a new scale factor globally via the sf::View Class, while retaining position in the world
	 */
	void engine::setGlobalScaleFactor(sf::View* gameCamera, float fauxWidth, float fauxHeight, bool stayPosition) {

		sf::Vector2f cameraCenter = gameCamera->getCenter();

		gameCamera->reset(sf::FloatRect(0.0f, 0.0f, fauxWidth, fauxHeight));
		
		if (stayPosition) {
			gameCamera->setCenter(cameraCenter);
		}

	}
}