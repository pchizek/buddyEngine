/*
 * Main.cpp
 * buddyEngine main
 *
 * Written by: Patrick Chizek
 * https://github.com/pchizek
 * 
 * tinyxml2 by: Lee Thomason
 * https://github.com/leethomason
 * 
 * Uses SFML (Simple Fast Multimedia Library)
 * https://github.com/SFML/SFML
 * https://www.sfml-dev.org/
 * 
 */

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "managers/objectManager2d.h"
#include "loader/loader.h"

using namespace sf;
using namespace std;

// TODO: Make this not global, as issues arise in Debug configuration and it is bad practice
RenderWindow window(VideoMode(800, 600), "SFML works!");

/* Get struct for all objects to be rendered in the level */
int main()
{
	/* Bind keys */

	/* Create Window */

	/* Load File */
	loadLevel("resources/levels/testLevel.xml");

	//sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);

	/* Main event loop */
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
			}
		}

		window.clear();
		
		int rc = renderBlocks();
	
		//window.draw(levelObjects.firstBlock->sprite);
		window.display();
	}

	return 0;
}