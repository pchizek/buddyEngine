/*
 * objectManager.cpp
 * 3D Object Manager
 *
 * Written by: Patrick Chizek
 * https://github.com/pchizek
 *
 */

#include "objectManager2d.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

struct objects levelObjects;
extern sf::RenderWindow window;
int cameraCoords[2] = {0,0};

/* Initialize threading */

/* Render all sets of objects according to type */
int renderBlocks() {
	
	/* Get objects struct */
	//extern struct objects levelObjects;

	/* Render all blocks */
	block* nextBlock;

	nextBlock = levelObjects.firstBlock;

	for (int i = 0; i < levelObjects.numBlocks; i++) {

		nextBlock->draw();

		/* Move pointer down the linked list */
		nextBlock = nextBlock->nextBlock;

	}

	return 0;

}

/*
int renderStdObjs() {

}

int renderAll() {

}
*/

/* Standard Objects */
object::object(sf::Texture* objTexture, int gridCoords[2], bool alwaysDraw) {

	/* Create sprite */
	sprite.setTexture(*objTexture);

	/* Convert into actual world coordinates */
	worldCoords[0] = gridCoords[0]*BLOCK_SZ;
	worldCoords[1] = gridCoords[1]*BLOCK_SZ;

	/* Add flag for whether object is always drawn */
	drawAlways = alwaysDraw;

}/*
int object::checkDraw() {
	
	extern int apron[2];
	extern int cameraCoords[2];

	int rc;

	if (this->drawAlways) {
		
		rc = this->draw();

	}

	else if (
		((this->worldCoords[0] - apron[0]) > 0) &
		((this->worldCoords[0] - apron[2]) < 0) &
		((this->worldCoords[1] - apron[1]) > 0) &
		((this->worldCoords[1] - apron[3]) < 0)) {

		rc = this->draw();

	}

}
*/
int object::draw() {

	// Get external to the program's window
	//extern sf::RenderWindow window;
	extern int cameraCoords[2];

	// Set the position of the sprite in the window
	sprite.setPosition(sf::Vector2f(
		(float)(this->worldCoords[0] - cameraCoords[0]),
		(float)(this->worldCoords[1] - cameraCoords[1])));

	// Set it to be drawn in the next window
	window.draw(sprite);

	return 0;

}

/* Blocks */
block::block(sf::Texture *objTexture, int gridCoords[2], int selection, bool alwaysDraw)
	:object(objTexture,gridCoords,alwaysDraw)
{

	/* Select block from blockset */
	this->setSelection(selection);

}

int block::setSelection(int selection) {
	
	sprite.setTextureRect(sf::IntRect(selection * BLOCK_SZ, 0, BLOCK_SZ, BLOCK_SZ));

	return 0;

}




