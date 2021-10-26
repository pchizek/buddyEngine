/*
 * gameWindow.cpp
 * For managing the display of the game
 *
 *	Written by: Patrick Chizek
 */

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <gameWindow.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
 
 //#include <WinBase.h>

#define FAUX_RES_HEIGHT 180.0
#define FAUX_RES_WIDTH 320.0
#define WIDE_RATIO 1.7777

using namespace std;
using namespace tinyxml2;
using namespace engine;
namespace po = boost::program_options;
//using namespace po;


// Get window modes

void setGameOptions() {

	// Platform specific options TODO: implement OSX, Linux
	//#ifdef _WIN32 || _WIN64
	
	// Read from configuration file
	
	// Open the config file
	ifstream configFile("Config.ini", ifstream::in);

	// Store things from config file into a map
	po::options_description gameConfig("Game Options");
	po::variables_map configMap;
	
	store(parse_config_file("Config.ini", gameConfig), configMap);

	// Evaluate options in .ini file from the config map
	//setWindowOptions(configMap);

}

/*
void setWindowOptions(po::variables_map configMap) {

	variable_value resolutionSetting = configMap.find()

}
*/



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