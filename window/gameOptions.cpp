/*
 * gameWindow.cpp
 * For managing the display of the game
 *
 *	Written by: Patrick Chizek
 */

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <boost/program_options.hpp>
#include <tinyxml2.h>
#include <objManager2d.h>
#include <gameOptions.h>
#include <string>
#include <iostream>
 
 //#include <WinBase.h>

#define FAUX_RES_HEIGHT 180.0
#define FAUX_RES_WIDTH 320.0
#define WIDE_RATIO 1.7777

using namespace std;
using namespace sf;
using namespace tinyxml2;
using namespace engine;
namespace po = boost::program_options;
//using namespace po;


// Get window modes

void setGameOptions(RenderWindow *window, View* gameCamera) {

	// Platform specific options TODO: implement OSX, Linux
	//#ifdef _WIN32 || _WIN64

	// Store things from config file into a map
	po::options_description gameConfig;
	po::variables_map configMap;
	
	// Add options
	gameConfig.add_options()
		("Settings.Resolution", po::value<string>(), "Resolution of game window")
		("Settings.Fullscreen", po::value<bool>(), "Whether window is fullscreen")
		;
	
	store(parse_config_file("options.cfg", gameConfig), configMap);

	// Evaluate options in .ini file from the config map
	setWindowOptions(window,gameCamera,
		configMap["Settings.Resolution"].as<string>(),
		configMap["Settings.Fullscreen"].as<bool>());

}


void setWindowOptions(RenderWindow* window, View* gameCamera, string resolution, bool fullscreen) {

	// split string
	string width = resolution.substr(0, resolution.find("_"));
	string height = resolution.substr(resolution.find("_") + 1);
	Vector2u windowResolution(stoi(width), stoi(height));

	// check window setting

	// Get fullscreen modes
	vector<VideoMode> screenModes = VideoMode::getFullscreenModes();
	bool validScreenMode = false;
	for (uint8_t i = 1; i < screenModes.size(); i++) {

		// Check size of allowable screen modes
		if (screenModes[i].width == windowResolution.x
			&& screenModes[i].height == windowResolution.y) {
			validScreenMode = true;
			break;
		}

	}

	// Error TODO: Set Default screen mode if none is found, regenerate config file
	if (!validScreenMode) {
		exception("Invalid Screen Setting");
	}


	// Recreate window
	if (fullscreen){
		window->create(VideoMode(
			windowResolution.x,
			windowResolution.y),
			"Window (Fullscreen)", 
			Style::Fullscreen);
	}
	else {
		window->create(VideoMode(
			windowResolution.x,
			windowResolution.y),
			"Window (-ed)");
	}

	/* Set VSync TODO: Make a game option */
	window->setVerticalSyncEnabled(true);

	engine::setGlobalScaleFactor(gameCamera, FAUX_RES_WIDTH, FAUX_RES_HEIGHT, false);

	window->setView(*gameCamera);
	//gameCamera->setSize(FAUX_RES_WIDTH, FAUX_RES_HEIGHT);

}




/*
void updateScaleFactor(sf::RenderWindow* window) {
	
	float scaleX;
	float scaleY;
	float displayRatio;
	float unstretchX = 1;
	float unstretchY = 1;

	// Get new size of window
	sf::Vector2u newWindowSize = window->getSize();
	
	// Get ratio of window size to display resolution
	scaleX = float(newWindowSize.x) / (FAUX_RES_WIDTH);
	scaleY = float(newWindowSize.y) / (FAUX_RES_HEIGHT);

	// Get new display ratio
	displayRatio = float(newWindowSize.x) / float(newWindowSize.y);
	if (displayRatio > WIDE_RATIO) {

		unstretchX = (scaleY*WIDE_RATIO)/(scaleX*displayRatio);

	}
	else {

		unstretchY = (scaleX*displayRatio) / (scaleY*WIDE_RATIO);

	}

	// Get offset of view from left side, top

	// Set scale Factor
	if (scaleX < scaleY) {
		scaleFactor.x = scaleX;
		scaleFactor.y = scaleX/scaleY;// (scaleX * scaleX) / scaleY;
	}
	else
	{
		scaleFactor.x = scaleY/scaleX; // (scaleY * scaleY) / scaleX;
		scaleFactor.y = scaleY;
	}
}
*/