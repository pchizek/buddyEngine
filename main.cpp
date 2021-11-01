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
//#include <SFML/System.hpp>

#include <objManager2d.h>
#include <loader.h>
#include <gameOptions.h>
#include <controls.h>

using namespace sf;
using namespace std;
using namespace engine;

int main(){

	/* Initialize some things */
	initControls();
	loadControlScheme();
	setControlScheme("flyingCameraKeyboard");

	/* Create a render window, the view variable*/

	// NOTE: SFML objects have to be passed into functions
	// if they are global it messes with mutexes 
	RenderWindow window(VideoMode(), string("Window"));
	sf::View gameCamera;
	
	//setControlMode(controlMode::flyingCamera);
	setGameOptions(&window,&gameCamera);

	/* TODO: Package game assets into some sort of library */

	/* Load File */
	loadLevel("resources/levels/testLevel.xml");
	extern int cameraCoords[2];


	/* Main event loop */
	while (window.isOpen())
	{

		// TODO: Make a better update procedure that isn't 
		// locked to a specific frame rate
		using clock = chrono::steady_clock;

		clock::time_point nextTick = clock::now() + 17ms;

		/* Process events */
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				window.close();
				return 0;
			}

		}

		/* Controls */
		resolveKeyControls(&window, &gameCamera);
		//controlInput(&gameCamera);
		//resolveState();

		/* Clear window for next draw */
		window.clear();

		/*Ensure window size is the same, or lock window size*/
		window.setView(gameCamera);

		/* If not, reset scale factor accordingly*/

		engine::renderAll(&window);
	
		window.display();

		// Wait until next time
		this_thread::sleep_until(nextTick);

	}

	return 0;
}