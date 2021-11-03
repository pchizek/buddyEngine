/*
 * gameWindow.h
 * For managing the display of the game
 *
 *	Written by: Patrick Chizek
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <boost/program_options.hpp>
#include <objManager2d.h>

using namespace std;
using namespace tinyxml2;
namespace po = boost::program_options;

void setGameOptions(sf::RenderWindow *window, sf::View *gameCamera);
void setWindowOptions(sf::RenderWindow* window, sf::View* gameCamera, string resolution, bool fullscreen);

//void updateScaleFactor(sf::RenderWindow* window);
