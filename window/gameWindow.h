/*
 * gameWindow.h
 * For managing the display of the game
 *
 *	Written by: Patrick Chizek
 */

#ifndef _GAMEWINDOW_H

#define _GAMEWINDOW_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <objManager2d.h>

using namespace std;
using namespace tinyxml2;

extern sf::Vector2f engine::scaleFactor;
extern sf::RenderWindow window;

void updateScaleFactor(sf::RenderWindow* window);

#endif