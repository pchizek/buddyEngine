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

/*
void speedCeil(int dir) {

	if (dir) {
		playerState[2] += (dir * ACCEL);

		if (playerState[2] > MAXSPEED) {
			playerState[2] = MAXSPEED;
		}

		else if (playerState[2] < (-MAXSPEED)) {
			playerState[2] = -MAXSPEED;
		}

	}

	else {

		if (playerState[2] > DECEL_STEP) {
			playerState[2] = DECEL_STEP;
		}

		else if (playerState[2] < -DECEL_STEP) {
			playerState[2] = -DECEL_STEP;
		}

		else {
			playerState[2] = 0;
		}

	}

}
*/

namespace engine {

	unordered_map<string, Keyboard::Key> convertToKey;
	unordered_map<string, controlFunction> controlFunctionMap;
	keybindVector* currentKeybindVector;
	unordered_map<string, keybindVector>controlSchemeMap;
}


#pragma region control_functions

void moveCamera(Window* gameWindow, View* gameCamera, vector<int> args) {

	if (args.at(0)) {
		gameCamera->move(0, args.at(1));
	}
	else {
		gameCamera->move(args.at(1), 0);
	}

}

void closeGame(Window* gameWindow, View* gameCamera, vector<int> args) {
	gameWindow->close();
}

#pragma endregion 

/*
void controlInput(sf::View* gameCamera, ) {

	// Check location of mouse
	Vector2i currentMouseLocation = Mouse::getPosition();
	if (currentMouseLocation != engine::mouseLocation) {
		currentMouseLocation = engine::mouseLocation;
		engine::mouseEnabled = true;
	}

	// Check state of mouse button
	if (Mouse::isButtonPressed(Mouse::Left) {
		// Handle mouse
	}

	engine::controlCallback(gameCamera);
}


void controlInput(sf::View* gameCamera) {

	// Check location of mouse
	Vector2i currentMouseLocation = Mouse::getPosition();
	if (currentMouseLocation != engine::mouseLocation) {
		currentMouseLocation = engine::mouseLocation;
		engine::mouseEnabled = true;
	}

	// Check state of mouse button
	if (Mouse::isButtonPressed(Mouse::Left) {
		// Handle mouse
	}

	engine::controlCallback(gameCamera);
}

void noControl(View* gameCamera) {
	int x = 1;
}

void flyingCameraControl(View* gameCamera) {

	/* TODO: make camera center around a player *
	float bumpX = (
		(Keyboard::isKeyPressed(Keyboard::D))
		- (Keyboard::isKeyPressed(Keyboard::A)));

	float bumpY = (
		(Keyboard::isKeyPressed(Keyboard::S))
		- (Keyboard::isKeyPressed(Keyboard::W)));

	// 
	speedCeil(bumpX);
	speedCeil(bumpY);

	gameCamera->move(bumpX, bumpY);

}

void playerCameraControl(View* gameCamera) {
	int x = 1;
}
*/

void setControlScheme(string controlSchemeName) {
	engine::currentKeybindVector = &engine::controlSchemeMap.at(controlSchemeName); //What's causing the problem? prolly not
}

void resolveKeyControls(Window* gameWindow, View* gameCamera) {

	// Check all keys in map
	for (size_t i = 0; i < engine::currentKeybindVector->size();i++) {
		if (Keyboard::isKeyPressed(engine::currentKeybindVector->at(i).key)) {
			// If key is pressed, run function binded to that key
			engine::currentKeybindVector->at(i).controlCallback(gameWindow,gameCamera,engine::currentKeybindVector->at(i).args);
		}
	}

}

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
