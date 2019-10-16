// Anim 2d .cpp

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "animation2d.h"

// Class construction
animation::animation(char imagePath[], int spriteDims[2], int framesInRow[], int frameTicks [][] ) {
	sf::Texture texture;

	// Load file and check for error
	if (!texture.loadFromFile(imagePath)) {
		
	}

	// Construct sprite


}
