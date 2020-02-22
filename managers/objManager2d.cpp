/*
 * objectManager.cpp
 * 3D Object Manager
 *
 * Written by: Patrick Chizek
 * https://github.com/pchizek
 *
 */

#include "objManager2d.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

//using namespace engine;
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
	object::object(int worldLoc[2],
		int objLayer,
		sf::Texture* newTexture,
		int spriteRect[4]) {

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
		
		/* Set object script */
		//objectScript = script;

	}

	/* Set Texture */
	void object::setTexture(sf::Texture* newTexture) {
		objectSprite.setTexture(*newTexture);
	}

	/* Set Script */
	//void setScript(void* script) {
	//}

	/* Draw */
	void object::draw(sf::RenderWindow* window) {

		// Get external to the program's window
		extern int cameraCoords[2];

		// Set the position of the sprite in the window
		objectSprite.setPosition(sf::Vector2f(
			(float)(this->worldCoords[0] - cameraCoords[0]),
			(float)(this->worldCoords[1] - cameraCoords[1])));

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