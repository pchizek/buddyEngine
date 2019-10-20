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

#define MV_LEFT		0
#define MV_RIGHT	1
#define MV_JUMP		2

extern int playerState[4];

void resolveState();

void controlInput();

void speedCeil(int dir);


#endif // !_CONTROLS_H
