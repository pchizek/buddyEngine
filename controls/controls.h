/*
 * controls.h
 * Control Input Library
 *
 * Written by: Patrick Chizek
 * https://github.com/pchizek
 *
 */

#ifndef _CONTROLS_H
#define _CONTROLS_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

#define MV_LEFT		0
#define MV_RIGHT	1
#define MV_JUMP		2

extern int playerState[4];
using namespace std;

namespace engine {

	// Define controlFunction as a pointer to a function of this type
	typedef void (*controlFunction)(sf::Window* gameWindow, sf::View* gameCamera, vector<int> args);
	//typedef void (*controlFunction)(sf::Window*, vector<int> args);

	// Wrap controlFunction with key and arguments defined by the keybind element in the xml file
	typedef struct {
		sf::Keyboard::Key key;
		controlFunction controlCallback;
		vector <int> args;
	} keyControlFunction;

	// Vector of keybindings within a specific control scheme
	typedef vector<keyControlFunction> keybindVector;

	// Map for converting from string in xml file to Key enum in SFML
	extern unordered_map<string, sf::Keyboard::Key> convertToKey;

	// Map of control functions (for importing from xml)
	extern unordered_map<string, controlFunction> controlFunctionMap;

	// Pointer to vector holding current keybindings (aka a control scheme)
	extern keybindVector* currentKeybindVector;

	// Map of all vectors of control schemes
	extern unordered_map<string, keybindVector> controlSchemeMap;
	

}

/*
void controlInput(sf::View* gameCamera);
void noControl(sf::View* gameCamera);
void flyingCameraControl(sf::View* gameCamera);
void playerCameraControl(sf::View* gameCamera);
*/

void moveCamera(sf::Window* gameWindow, sf::View* gameCamera, vector<int> args);

void resolveKeyControls(sf::Window* gameWindow, sf::View* gameCamera);
void setControlScheme(string controlSchemeName);

//void speedCeil(float bump);

void initControls();


#endif // !_CONTROLS_H
