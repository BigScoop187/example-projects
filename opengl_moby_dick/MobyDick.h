#ifndef _MOBYDICK_H_
#define _MOBYDICK_H_

#include "InstanceTransformation.h"
#include "Shapes.h"
#include "Colors.h"
#include "Sailor.h"
#include <vector>

using namespace std;

const GLdouble MobySlimFactor = 0.75;
const GLdouble GirthPercentage = .30;
const GLdouble MobyTipPercentage = 0.9;
const GLdouble JawLocation = 0.6;
const GLdouble JawScale = 0.2;
const GLdouble ToothXScale = 0.3;
const GLdouble ToothYScale = 0.6;
const GLdouble ToothZScale = 0.6;
const GLdouble AhabRopeScale = 1.1;
const GLdouble RopeWidth = 0.4;
const GLint MobyDickLength = 80;
const GLint MobyAngle = 60;
const GLint MaxJawTheta = 40;
const GLint MinJawTheta = -1;
const GLint MinWavePos = 8;

struct MobyDick
{	
	vector<GLdouble> mobyParams;

	// Sailors
	Crew victims;

	MobyDick();
	void Whale();
	void Tooth();
	void Jaw();
	void Eye();
	void Ropes();
	void MobyBody();
	void MobyHead();

	GLdouble waveOuterR;
	GLdouble wavePosition;
	GLint jawTheta;
	GLint jawDirection;

	void ITNoColor( GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ, 
					GLfloat translateX, GLfloat translateY, GLfloat translateZ, 
					GLfloat degree, GLfloat xAxis, GLfloat yAxis, GLfloat zAxis,
					void( MobyDick::*method )() )
	{
		glPushMatrix();
		glTranslatef( translateX, translateY, translateZ );
		glRotatef( degree, xAxis, yAxis, zAxis );
		glScalef( scaleX, scaleY, scaleZ );
		(this->*method)();
		glPopMatrix();
	}
};

#endif