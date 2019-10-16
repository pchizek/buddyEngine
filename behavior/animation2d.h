#ifndef _ANIMATION_2D_H
#define _ANIMATION_2D_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// Defines for lines of each file that coorespond to standard actions
typedef enum {
	ANIM_IDLE,
	ANIM_WALK,
	ANIM_JUMP,
	ANIM_FALL,
};

class animation {

public:
	/* Construct with
	image, x,y dimensions of the frame, vector of number of frames in each row, 2d mat of times
	
	*/
	animation(char imagePath[], int spriteDims[2], int framesInRow[], int frameTicks[][]);

	// Sets the animation sequence onto a new row of sprites and cooresponding times
	int setRow(int newRow);

	// Mirrors the sprite (0 is normal, right facing, 1 is mirrored)
	int setMirrored(bool mirrored);

};


// Other stuff

#endif