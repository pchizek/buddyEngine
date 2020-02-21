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

#include <thread>
#include <chrono>

#define FPS 60
#define INTERVAL 17

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "managers/objManager2d.h"
#include "loader/loader.h"
#include "controls.h"

using namespace sf;
using namespace std;
using namespace engine;

int main(){

	/* Create a render window TODO: Add */
	RenderWindow window(VideoMode(800, 600), "SFML works!");

	/* Load File */
	loadLevel("resources/levels/testLevel.xml");
	extern int cameraCoords[2];

	/* Main event loop */
	while (window.isOpen())
	{

		using clock = chrono::steady_clock;

		clock::time_point nextTick = clock::now() + 17ms;

		/* Process events */
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
			}

		}

		/* Controls */
		controlInput();
		resolveState();

		cameraCoords[0] = playerState[0];
		cameraCoords[1] = playerState[1];

		/* Render objects */

		window.clear();
		engine::renderAll(&window);
	
		window.display();

		// Wait until next time
		this_thread::sleep_until(nextTick);

	}

	return 0;
}