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
	controlFunction controlCallback;
}

void controlInput(sf::View* gameCamera) {
	engine::controlCallback(gameCamera);
}

void noControl(View* gameCamera) {
	int x = 1;
}

void flyingCameraControl(View* gameCamera) {

	/* TODO: make camera center around a player */
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

void setControlMode(engine::controlMode mode) {

	// set callback to whichever control mode is correct, that way
	// it's only evaluated once
	if (mode == engine::controlMode::none) {
		engine::controlCallback = &noControl;
	}
	else if (mode == engine::controlMode::flyingCamera) {
		engine::controlCallback = &flyingCameraControl;
	}
	else if (mode == engine::controlMode::playerCamera) {
		engine::controlCallback = &playerCameraControl;
	}
	else {
		exception("Invalid Camera Mode");
	}

}

void speedCeil(float bump) {
	bump *= 12;
}