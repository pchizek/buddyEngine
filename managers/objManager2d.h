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
#include <xmlTools.h>
#include <tinyxml2.h>
#include <array>

#define BLOCK_SIZE 32
#define MAX_LAYERS 5

using namespace std;
using namespace tinyxml2;

/* Get access to window */
extern sf::RenderWindow window;

/* Texture Assets */
namespace engine {

	/* A graphical asset */
	void importTexture(string textureFile, string textureKey);

	/* Structs for script specific info */
	typedef struct {

		bool active;

		uint16_t nodeCounter;
		uint8_t numNodes, currentNode;
		vector<array<int, 3>> motionNodes;
		float scriptOffset[2];
		int dx, dy;
		float dt;

	} motionInfoCache;

	typedef struct {

		bool active;

		// Current frame counter
		uint16_t frameCounter;
		// Frames 
		uint8_t numFrames, currentFrame;
		vector<array<int,5>> animFrames;

	} animInfoCache;

	/* An object that exists in the world */
	class object {

	public:

		object(	int worldLoc[2],
				uint8_t objLayer,
				sf::Texture* newTexture,
				sf::IntRect* spriteRect);

		void setTexture(sf::Texture* newTexture);
		void setScript(XMLElement* scriptElement);

		void draw(sf::RenderWindow* window);

		int worldCoords[2];

		motionInfoCache motionInfo;
		animInfoCache animInfo;

		// Pointer to next object in linked list
		object* nextObject{ NULL };
		object* prevObject{ NULL };
		uint8_t layer{ 0 };

	private:

		sf::Sprite objectSprite;

	};

	/* Data structure for all objects in the level */
	extern object* firstObject;
	extern object* lastObject;

	void renderAll(sf::RenderWindow* window);

	// Assets
	extern unordered_map<string, sf::Texture> textureMap;

}
#endif