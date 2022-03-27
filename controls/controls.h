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
#include <xmlTools.h>

#define MV_LEFT		0
#define MV_RIGHT	1
#define MV_JUMP		2

extern int playerState[4];
using namespace std;

namespace engine {

	// Define controlFunction as a pointer to a function of this type
	typedef void (*controlFunction)(sf::Window* gameWindow, sf::View* gameCamera, vector<int>* args);	/*!< Callback type for a function controlled by user input. */

	//typedef void (*argVectorHandler)()

	extern unordered_map<string, controlFunction> controlFunctionMap;	/*!< Map of control functions (for importing from XML). */
	`
	// For each instance of a control function that can be called, store which function it is, 
	// as well as the arguments for each specific control function (e.g. a pointer to setControlScheme()
	// with the arguments telling it which control scheme to switch to, stored in an vector<int>).
	typedef struct callbackFunctionNode callbackFunctionNode;
	struct callbackFunctionNode	{
		controlFunction* controlCallback;
		vector<int> args;
	};

	extern unordered_map<string, sf::Keyboard::Key> convertToKey;	/*!< Map for converting from string in XML file to Key enum in SFML. */

	// Wrap controlFunction with key and arguments defined by the keybind element in the xml file
	typedef struct {
		sf::Keyboard::Key key;
		vector<callbackFunctionNode> callbackVector;
	} keyControl;	/*!< Wrapper for a specific key function. */

	typedef vector<keyControl> keybindVector;	/*!< Vector of keybindings within a specific control scheme. */
	extern keybindVector* currentKeybindVector; /*!< Pointer to vector holding current keybindings (a.k.a. a control scheme). */
	extern map<string, keybindVector> controlSchemeMap; /*!< Map of all vectors of control schemes, so one could switch to a different one without loading the XML file again. */

	// Wrap UI element
	typedef struct {
		sf::Sprite elementSprite;
		sf::IntRect elementRect; // or origin?
		bool isBackground; // make multiple layers?
		vector<callbackFunctionNode> callbackVector;
	} uiElement;

	typedef vector<uiElement> uiSetVector;
	extern uiSetVector* currentUiSetVector;
	extern map<string, uiSetVector> uiSetMap;

}
/* Control functions. */
#pragma region program_functions

engine::controlFunction moveCamera(sf::Window* gameWindow, sf::View* gameCamera, vector<int>* args);
engine::controlFunction closeGame(Window* gameWindow, View* gameCamera, vector<int>* args);


#pragma endregion

#pragma region keyboard_gamepad_controls

engine::controlFunction setControlScheme(sf::Window* gameWindow, sf::View* gameCamera, vector<int>* args);

#pragma endregion

#pragma region ui_functions

engine::controlFunction switchUiSet(sf::Window* gameWindow, sf::View* gameCamera, vector<int>* args);

#pragma endregion




void resolveKeyControls(sf::Window* gameWindow, sf::View* gameCamera);	/*!< Reads if any mapped keys have been pressed and calls their relevant functions. */
//void setControlScheme(string controlSchemeName);						/*!< Sets the keybindings specified by the key given to those in `controlSchemeMap`.*/

void initControls();	/*!< Initialize the controls, which includes populating the `convertToKey` map and emplacing the keybind functions into `controlFunctionMap`. */
