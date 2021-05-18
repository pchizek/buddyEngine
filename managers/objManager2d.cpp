/*
 * objectManager.cpp
 * 3D Object Manager
 *
 * Written by: Patrick Chizek
 * https://github.com/pchizek
 *
 */

#include "objManager2d.h"
#include "..//loader/xmlTools.h"
#include "..//loader/tinyxml2.h"
#include <array>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace tinyxml2;
using namespace engine;
using namespace std;

int cameraCoords[2] = { 0,0 };

namespace engine {

	object* firstObject;
	object* lastObject;
	unordered_map<string, sf::Texture> textureMap;

	/* Texture */
#pragma region texture

	void engine::importTexture(string textureFile, string textureKey) {

		sf::Texture assetTexture;

		if (!assetTexture.loadFromFile(textureFile)) {
			// Error
			//const char* errTextureFile = string("File not found: ").append(textureFile).c_str();
			//exception(errTextureFile);
			exception("File not found");
		}
		else {
			// Copy into map
			engine::textureMap.emplace(textureKey, assetTexture);
		}

	}
#pragma endregion

	/* Object */
#pragma region object

	/* Constructor */
	object::object(int worldLoc[2], uint8_t objLayer, sf::Texture* newTexture, int spriteRect[4]) {

		/* Set world location */
		worldCoords[0] = worldLoc[0];
		worldCoords[1] = worldLoc[1];

		/* Set sprite texture */
		if (newTexture) {
			objectSprite.setTexture(*newTexture);
		}

		/* Set texture rectangle/hitbox */
		if (spriteRect[3]) {
			objectSprite.setTextureRect(sf::IntRect(spriteRect[0], spriteRect[1], spriteRect[2], spriteRect[3]));
		}

		layer = objLayer;

	}

	/* Set Texture */
	void object::setTexture(sf::Texture* newTexture) {
		objectSprite.setTexture(*newTexture);
	}

	/* Set Script */
	void object::setScript(XMLElement* scriptElement) {

		bool scriptsToProcess = true;
		XMLElement* nodeElement;
		int numNodes{ 0 };
		int numFrames{ 0 };

		// Get type of script
		if (scriptElement->Attribute("type", "move")) {

			motionInfoCache motionInfo;
			array<int,3> motionNodesArray{ 0,0,0 };
			
			nodeElement = scriptElement->FirstChildElement("node");
			while (nodeElement) {

				// Increase number of nodes
				numNodes++;

				// Extract node into array
				parse(nodeElement->GetText(), motionNodesArray);

				// put contents of array into vector
				motionInfo.motionNodes.push_back(motionNodesArray);

				// Get next node
				nodeElement = nodeElement->NextSiblingElement("node");
				
			}
			motionInfo.numNodes = numNodes;
		}
		else if (scriptElement->Attribute("type", "animation")) {
			
			animInfoCache animInfo;
			array<int, 5> animFramesArray{ 0,0,0,0,0 };
			
			nodeElement = scriptElement->FirstChildElement("node");
			while (nodeElement) {

				// Increase number of nodes
				numFrames++;

				// Extract node into array
				parse(nodeElement->GetText(), animFramesArray);
				
				// put contents of array into vector
				animInfo.animFrames.push_back(animFramesArray);
				
				// Get next node
				nodeElement = nodeElement->NextSiblingElement("frame");
			}
			animInfo.numFrames = numFrames;
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

			if (animInfo.frameCounter == animInfo.animFrames.at(animInfo.currentFrame)[4] ){

				// Reset frame counter
				animInfo.frameCounter = 0;

				// Loop the current frame in the row
				if (animInfo.currentFrame + 1 == animInfo.numFrames) {
					animInfo.currentFrame = 0;
				}
				else {
					animInfo.currentFrame++;
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

		// Resolve motion
		if (motionInfo.active) {

			if (motionInfo.nodeCounter == motionInfo.motionNodes.at(motionInfo.currentNode)[2] ) {

				// Reset counter
				motionInfo.nodeCounter = 0;

				// Loop the motion
				if (motionInfo.currentNode + 1 == motionInfo.numNodes) {
					motionInfo.currentNode = 0;
				}
				else {
					motionInfo.currentNode++;
				}

				// Set offset to that of the node
				motionInfo.scriptOffset[0] = motionInfo.motionNodes.at(motionInfo.currentNode)[0];
				motionInfo.scriptOffset[1] = motionInfo.motionNodes.at(motionInfo.currentNode)[1];

			}

			else {

				// Interpolate position on path
				motionInfo.scriptOffset[0] = 
					(round((motionInfo.motionNodes.at(motionInfo.currentNode+1)[0] - motionInfo.motionNodes.at(motionInfo.currentNode)[0])
						* (motionInfo.nodeCounter / motionInfo.motionNodes.at(motionInfo.currentNode)[2]) ) );

				motionInfo.scriptOffset[1] =
					(round((motionInfo.motionNodes.at(motionInfo.currentNode + 1)[1] - motionInfo.motionNodes.at(motionInfo.currentNode)[1])
						* (motionInfo.nodeCounter / motionInfo.motionNodes.at(motionInfo.currentNode)[2])));

				motionInfo.nodeCounter++;

			}


		}

		// Set the position of the sprite in the window
		objectSprite.setPosition(sf::Vector2f(
			(float)(this->worldCoords[0] - cameraCoords[0] + motionInfo.scriptOffset[0]),
			(float)(this->worldCoords[1] - cameraCoords[1] + motionInfo.scriptOffset[1])));

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

}