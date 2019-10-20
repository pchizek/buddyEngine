/*
 * controls.cpp
 * Control Input Library
 *
 * Written by: Patrick Chizek
 * https://github.com/pchizek
 *
 */

#include <SFML/Window.hpp>
using namespace sf;

#define ACCEL		2
#define MAXSPEED	10
#define DECEL_STEP	6	

int playerState[4] = { 0,0,0,0 };

void resolveState() {

	playerState[0] += playerState[2];
	playerState[1] += playerState[3];

}

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

void controlInput() {

	int dir = (
		(Keyboard::isKeyPressed(Keyboard::A))
		- (Keyboard::isKeyPressed(Keyboard::D)));

	// Get speed
	speedCeil(dir);

	/*
	if (Keyboard::isKeyPressed(Keyboard::Space)) {

		jump();

	}
	*/
}


/*
void jump() {

}
*/

/*
bool isOnFloor() {

}


void doubleTap(int key, bool state) {

}
*/