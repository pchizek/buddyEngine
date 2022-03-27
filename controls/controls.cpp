/*
 * controls.cpp
 * Control Input Library
 *
 * Written by: Patrick Chizek
 * https://github.com/pchizek
 *
 */

#include <SFML/Window.hpp>
#include <controls.h>
#include <string>
#include <objManager2d.h>

using namespace sf;
using namespace std;
#define ACCEL		2
#define MAXSPEED	10
#define DECEL_STEP	6	

int playerState[4] = { 0,0,0,0 };
int cameraBump[2] = { 0,0 };

namespace engine {

	unordered_map<string, Keyboard::Key> convertToKey;

	unordered_map<string, controlFunction> controlFunctionMap;
	keybindVector* currentKeybindVector;
	unordered_map<string, keybindVector> controlSchemeMap;

	uiSetVector* currentUiSetVector;
	unordered_map <string, uiSetVector> uiSetMap;
}


/* Control functions. */
#pragma region program_functions
engine::controlFunction moveCamera(Window* gameWindow, View* gameCamera, vector<int>* args) {

	if (args->at(0)) {
		gameCamera->move(0, args->at(1));
	}
	else {
		gameCamera->move(args->at(1), 0);
	}

}

engine::controlFunction closeGame(Window* gameWindow, View* gameCamera, vector<int>* args) {
	gameWindow->close();
}

#pragma endregion

#pragma region keyboard_gamepad_controls

engine::controlFunction setControlScheme(sf::Window* gameWindow, sf::View* gameCamera, vector<int>* args) {

	string controlSchemeName = *(string*)args;
	engine::currentKeybindVector = &engine::controlSchemeMap.at(controlSchemeName);
}

#pragma endregion

#pragma region ui_controls

engine::controlFunction switchUiSet(sf::Window* gameWindow, sf::View* gameCamera, vector<int>* args) {

	string uiSetName = *(string*)args;
	engine::currentUiSetVector = &engine::uiSetMap.at(uiSetName);

}

#pragma endregion

void resolveKeyControls(Window* gameWindow, View* gameCamera) {

	// Check all keys in map
	for (size_t i = 0; i < engine::currentKeybindVector->size();i++) {
		if (Keyboard::isKeyPressed(engine::currentKeybindVector->at(i).key)) {
			// If key is pressed, run function binded to that key
			engine::currentKeybindVector->at(i).controlCallback(gameWindow,gameCamera,engine::currentKeybindVector->at(i).args);
		}
	}

}

#pragma region ui

void drawUi(sf::RenderWindow* window) {

	// Which ui is selected


	// draw elements, first background, then foreground
	for (int i = 1; i > -1; i--) {

		if ()



	}

	window->draw();


}

#pragma endregion

void initControls() {

#pragma region import_keys
	// Alphabet
	engine::convertToKey.emplace(string("a"), Keyboard::Key::A);
	engine::convertToKey.emplace(string("b"), Keyboard::Key::B);
	engine::convertToKey.emplace(string("c"), Keyboard::Key::C);
	engine::convertToKey.emplace(string("d"), Keyboard::Key::D);
	engine::convertToKey.emplace(string("e"), Keyboard::Key::E);
	engine::convertToKey.emplace(string("f"), Keyboard::Key::F);
	engine::convertToKey.emplace(string("g"), Keyboard::Key::G);
	engine::convertToKey.emplace(string("h"), Keyboard::Key::H);
	engine::convertToKey.emplace(string("i"), Keyboard::Key::I);
	engine::convertToKey.emplace(string("j"), Keyboard::Key::J);
	engine::convertToKey.emplace(string("k"), Keyboard::Key::K);
	engine::convertToKey.emplace(string("l"), Keyboard::Key::L);
	engine::convertToKey.emplace(string("m"), Keyboard::Key::M);
	engine::convertToKey.emplace(string("n"), Keyboard::Key::N);
	engine::convertToKey.emplace(string("o"), Keyboard::Key::O);
	engine::convertToKey.emplace(string("p"), Keyboard::Key::P);
	engine::convertToKey.emplace(string("q"), Keyboard::Key::Q);
	engine::convertToKey.emplace(string("r"), Keyboard::Key::R);
	engine::convertToKey.emplace(string("s"), Keyboard::Key::S);
	engine::convertToKey.emplace(string("t"), Keyboard::Key::T);
	engine::convertToKey.emplace(string("u"), Keyboard::Key::U);
	engine::convertToKey.emplace(string("v"), Keyboard::Key::V);
	engine::convertToKey.emplace(string("w"), Keyboard::Key::W);
	engine::convertToKey.emplace(string("x"), Keyboard::Key::X);
	engine::convertToKey.emplace(string("y"), Keyboard::Key::Y);
	engine::convertToKey.emplace(string("z"), Keyboard::Key::Z);

	// Numbers
	engine::convertToKey.emplace(string("Num0"), Keyboard::Key::Num0);
	engine::convertToKey.emplace(string("Num1"), Keyboard::Key::Num1);
	engine::convertToKey.emplace(string("Num2"), Keyboard::Key::Num2);
	engine::convertToKey.emplace(string("Num3"), Keyboard::Key::Num3);
	engine::convertToKey.emplace(string("Num4"), Keyboard::Key::Num4);
	engine::convertToKey.emplace(string("Num5"), Keyboard::Key::Num5);
	engine::convertToKey.emplace(string("Num6"), Keyboard::Key::Num6);
	engine::convertToKey.emplace(string("Num7"), Keyboard::Key::Num7);
	engine::convertToKey.emplace(string("Num8"), Keyboard::Key::Num8);
	engine::convertToKey.emplace(string("Num9"), Keyboard::Key::Num9);

	// Punctuation (update if it's actually used)

	// Shift/Alt/etc.
	engine::convertToKey.emplace(string("Escape"), Keyboard::Key::Escape);
	engine::convertToKey.emplace(string("LControl"), Keyboard::Key::LControl);
	engine::convertToKey.emplace(string("RControl"), Keyboard::Key::RControl);
	engine::convertToKey.emplace(string("LShift"), Keyboard::Key::LShift);
	engine::convertToKey.emplace(string("RShift"), Keyboard::Key::RShift);
	engine::convertToKey.emplace(string("LAlt"), Keyboard::Key::LAlt);
	engine::convertToKey.emplace(string("RAlt"), Keyboard::Key::RAlt);
	engine::convertToKey.emplace(string("Tab"), Keyboard::Key::Tab);
	engine::convertToKey.emplace(string("Enter"), Keyboard::Key::Enter);
	engine::convertToKey.emplace(string("Tilde"), Keyboard::Key::Tilde);

	// Function keys (if used)

#pragma endregion

#pragma region define_functions

	// add an emplacement for each function that is directly called by a control
	engine::controlFunctionMap.emplace(string("moveCamera"), &moveCamera);
	engine::controlFunctionMap.emplace(string("closeGame"), &closeGame);

#pragma endregion
}
