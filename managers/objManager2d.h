/*
 * objectManager.h
 * 3D Object Manager
 *
 * Written by: Patrick Chizek
 * https://github.com/pchizek
 *
 */
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <xmlTools.h>
#include <tinyxml2.h>
#include <array>

#define BLOCK_SIZE 32
#define MAX_LAYERS 5

using namespace std;
using namespace tinyxml2;

/*! \brief  All funtctionality from handling the way objects are stored and displayed
 *			to how control inputs are handled are part of the engine namespace.	
 */
namespace engine {

	/* A graphical asset */
	void importTexture(string textureFile, string textureKey, sf::Vector2f scaleFactor);
	
	/*! \brief  Wrapper for texture related data. */
	typedef struct {
		sf::Texture assetTexture;
		sf::Vector2f assetScaleFactor;
	} textureInfo;

	/*! \brief  Struct for storing data related to movement.
	*
	*			Motion info is stored as waypoints (nodes) and number of screen updates it takes to get to the next node.
	*			At each of those paths, the position is interpolated at each screen update.
	*/
	typedef struct {

		bool active;						/*!< Whether this object has movement to be resolved. */
		uint16_t nodeCounter;				/*!< Number of game updates since this node was started. */
		uint8_t numNodes;					/*!< Number of movement nodes in this. */
		uint8_t currentNode;				/*!< The node that the object is heading for on it's movement path. */
		vector<array<int, 3>> motionNodes;	/*!< Movement data is stored in "nodes" i.e. waypoints that the object moves between.
												 The array is formatted as follows: `[ offset X , offset Y, travel time in frames]`.
												 The object always starts at it's home position given during construction and stored in `int worldCoords[2]`*/
		float scriptOffset[2];				/*!< Offset from the object's "home" position. The `int worldCoords[2]` variable doesn't 
												 change as this moves the object around the worldspace */
		int dx;								/*!< Number of pixels to move in x direction on next update. */
		int dy;								/*!< Number of pixels to move in y direction on next update. */
		float dt;							/*!< Timestep for updating position of moving object. */

	} motionInfoCache;

	/*! \brief  Struct for storing data related to animation.
	*
	*			Animation info is stored as keyframes, which are rectangles for subsections of the source image/sprite sheet 
	*			plus a number of screen updates to hold on that image.
	*/
	typedef struct {

		bool active;						/*!< Whether this object has an animation to process. */
		uint16_t frameCounter;				/*!< Number of game updates since this keyframe was shown. */
		uint8_t numFrames;					/*!< Total Number of keyframes in animFrames. */
		uint8_t currentFrame;				/*!< Current keyframe. */
		vector<array<int, 5>> animFrames;	/*!< Data for each keyframe is stored as a subsection of the image 
												 and number of update loops to show that image. */

	} animInfoCache;

	/*! \brief  All things in the worldspace, visible or not are defined as "objects".
	 *          
	 *			All objects have member variables that hold motion and animation info,
	 *			even if they are not animated or moving.
	 */
	class object {

	public:

		object(int worldLoc[2],
			uint8_t objLayer,
			textureInfo* newTexture,
			sf::IntRect* spriteRect); /*!< Constructor. */

		void setTexture(sf::Texture* newTexture);	/*!< Change texture after construction. */

		void setScript(XMLElement* scriptElement);	/*!< Set movement or animation script. */

		void draw(sf::RenderWindow* window);		/*!< Draw the object on the screen to be shown at the next window update. */

		void setObjectScaleFactor();				/*!< Set the scale of the sprite based on the local, object, and global scale factors. */
		void setLocalScaleFactor(sf::Vector2f* scaleFactor); /*!< Changes the local scale factor, but does not alter the sprite. */

		int worldCoords[2];							/*!< The "home" location of the object. */
		uint8_t layer{ 0 };							/*!< The pass on which to render the object. */

		motionInfoCache motionInfo;					/*!< Container for info pertaining to motion of the object (if applicable) */
		animInfoCache animInfo;						/*!< Container for info pertaining to the object's animation (if applicable)*/

		object* nextObject{ NULL };					/*!< Next object in linked list. */
		object* prevObject{ NULL };					/*!< Previous object in linked list. */

		sf::Sprite objectSprite;					/*!< Data for object's sprite. */
		textureInfo* objectTextureInfo;				/*!< The texture of the object and it's scale factor. */

	private:
		sf::Vector2f localScaleFactor;
		sf::Vector2f objectScaleFactor;

	};

	/* Data structure for all objects in the level */
	extern object* firstObject;  /*!< First object in linked list. */
	extern object* lastObject;  /*!< Last object in linked list. */
	extern unordered_map<string, textureInfo> textureInfoMap; /*!< Map for storing all the texture assets. */

	void setGlobalScaleFactor(sf::View* gameCamera, float fauxWidth, float fauxHeight, bool stayPosition);

	void renderAll(sf::RenderWindow* window); /*! Iterate through the linked list and render all objects. */
}