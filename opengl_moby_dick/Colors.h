#ifndef _COLORS_H_
#define _COLORS_H_

struct Color
{
	float red, green, blue;
};


const Color colors[] = {
						{0.5f, 0.5f, 0.5f},			// Gray
						{0.0f, 0.0f, 0.0f},			// Black
						{0.824f, 0.412f, 0.117f},	// Rope
						{1.0f, 1.0f, 1.0f},			// Teeth
						{0.957f, 0.643f, 0.376f},	// Skin
						{0.545f, 0.27f, 0.074f},	// Brown
						{0.098f, 0.098f, 0.439f},	// Blue - eyes
						{0.255f, 0.412f, 0.882f},	// Royal Blue
						{1.0f, 1.0f, 0.0f}			// Sun
};

#endif