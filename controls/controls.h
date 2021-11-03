/*
 * controls.h
 * Control Input Library
 *
 * Written by: Patrick Chizek
 * https://github.com/pchizek
 *
 */

#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <objManager2d.h>

#define MV_LEFT		0
#define MV_RIGHT	1
#define MV_JUMP		2

extern int playerState[4];
using namespace std;

namespace engine {

	// Define controlFunction as a pointer to a function of this type
	typedef void (*controlFunction)(sf::Window* gameWindow, sf::View* gameCamera, vector<int> args);	/*!< Callback type for a function controlled by user input. */
	//typedef void (*controlFunction)(sf::Window*, vector<int> args);

	// Wrap controlFunction with key and arguments defined by the keybind element in the xml file
	typedef struct {
		sf::Keyboard::Key key;
		controlFunction controlCallback;
		vector <int> args;
	} keyControlFunction;	/*!< Wrapper for a specific key function. */

	typedef vector<keyControlFunction> keybindVector;	/*!< Vector of keybindings within a specific control scheme. */

	extern unordered_map<string, sf::Keyboard::Key> convertToKey;	/*!< Map for converting from string in XML file to Key enum in SFML. */

	extern unordered_map<string, controlFunction> controlFunctionMap;	/*!< Map of control functions (for importing from XML). */

	extern keybindVector* currentKeybindVector; /*!< Pointer to vector holding current keybindings (a.k.a. a control scheme). */

	extern unordered_map<string, keybindVector> controlSchemeMap; /*!< Map of all vectors of control schemes, so one could switch to a different one without loading the XML file again. */
	
}
/* Control functions. */
void moveCamera(sf::Window* gameWindow, sf::View* gameCamera, vector<int> args);

void resolveKeyControls(sf::Window* gameWindow, sf::View* gameCamera);	/*!< Reads if any mapped keys have been pressed and calls their relevant functions. */
void setControlScheme(string controlSchemeName);						/*!< Sets the keybindings specified by the key given to those in `controlSchemeMap`.*/

void initControls();	/*!< Initialize the controls, which includes populating the `convertToKey` map and emplacing the keybind functions into `controlFunctionMap`. */
