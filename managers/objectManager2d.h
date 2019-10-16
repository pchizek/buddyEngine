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

#define BLOCK_SZ 32


/* Render all sets of objects according to type */
int renderBlocks();
//int renderStdObjs();
//int renderAll();

/* A basic graphical object */
class object {

	
public:
	
	object(sf::Texture* objTexture, int gridCoords[2], bool alwaysDraw = false);
	//int checkDraw();
	int draw();
	
	sf::Sprite sprite;

	bool drawAlways;
	//bool solid;
	int worldCoords[2];

	// Pointer to next object in linked list
	object* nextObject;

};


/* A block object */
class block : public object {
	
	
public:

	block(sf::Texture *objectTexture, int gridCoords[2], int selection, bool alwaysDraw = false);
	int setSelection(int selection);
	
	// Pointer to next in linked list
	block* nextBlock;
};

/* TODO: ADD An animated entity */

/* TODO: ADD A scripted entity */

/* TODO: ADD A player object (inherits from animated, scripted)*/

/* Data structure for all objects in the level */
struct objects {

	int numStdObjects;
	object* firstObject;

	int numBlocks;
	block* firstBlock;

};

extern struct objects levelObjects;
extern int cameraCoords[2];

#endif