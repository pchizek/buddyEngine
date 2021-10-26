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

#define MV_LEFT		0
#define MV_RIGHT	1
#define MV_JUMP		2

extern int playerState[4];


namespace engine {
	typedef enum {
		none,
		flyingCamera,
		playerCamera
	} controlMode;

	typedef void (*controlFunction)(sf::View* gameCamera);
	extern controlFunction controlCallback;

} 

void controlInput(sf::View* gameCamera);
void noControl(sf::View* gameCamera);
void flyingCameraControl(sf::View* gameCamera);
void playerCameraControl(sf::View* gameCamera);

void setControlMode(engine::controlMode);

void speedCeil(float bump);


#endif // !_CONTROLS_H
