/*
 * objectManager.h
 * 3D Object Manager
 *
 * Written by: Patrick Chizek
 * https://github.com/pchizek
 *
 */

#ifndef _OBJECTMANAGER_2D_H

#define _OBJECTMANAGER_2D_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>

#define BLOCK_SIZE 32
#define MAX_LAYERS 5

using namespace std;

/* Get access to window */
extern sf::RenderWindow window;

/* Texture Assets */
namespace engine {

	/* A graphical asset */
	void importTexture(string textureFile, string textureKey);

	/* An object that exists in the world */
	class object {

	public:

		object(	int worldLoc[2],
				int objLayer,
				sf::Texture* newTexture = NULL,
				int spriteRect[4] = NULL);

		void setTexture(sf::Texture* newTexture);
		//void setScript(void* script, ...); 

		void draw(sf::RenderWindow* window);

		int worldCoords[2];

		// Pointer to next object in linked list
		object* nextObject;
		object* prevObject;
		uint8_t layer;

	private:

		sf::Sprite objectSprite;
		void *objectScript;

	};

	/* Data structure for all objects in the level */
	//static int numObjects;
	extern object* firstObject;
	extern object* lastObject;

	void renderAll(sf::RenderWindow* window);

	// Assets
	extern unordered_map<string, sf::Texture> textureMap;

}
#endif