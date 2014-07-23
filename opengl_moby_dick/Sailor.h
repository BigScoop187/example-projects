#ifndef _SAILOR_H_
#define _SAILOR_H_

#include <stdlib.h>
#include <gl/glut.h>
#include "InstanceTransformation.h"
#include "Shapes.h"
#include "Colors.h"
#include <vector>

using namespace std;

const GLdouble ThighUpperR = 0.5;
const GLdouble LegPieceLength = 1.5;
const GLdouble ThighLowerR = 0.45;
const GLdouble AnkleR = 0.3;
const GLdouble TorsoLength = 3.0;
const GLdouble TorsoBaseR = 1.0;
const GLdouble TorsoTopR = 1.5;
const GLdouble NeckLength = 0.9;
const GLdouble NeckR = 0.5;
const GLdouble ShoulderRadius = 0.3;
const GLdouble ElbowRadius = 0.27;
const GLdouble WristRadius = 0.216;
const GLdouble ArmPieceLength = 1.5;
const GLdouble HandLength = 0.5;
const GLdouble HandBase = 0.15;
const GLdouble HandTop = 0.17;
const GLdouble FingerLength = 0.4;
const GLdouble FingerRadius = (HandTop * 2.0) / 4.0;
const GLdouble ThumbLength = 0.3;
const GLdouble planeEq1[4] = { 0.0, 0.5, 1.0, -0.2 };	// Hair ClipPlane
const GLdouble HeadRadius = 0.6;
const GLdouble EyeRadius = 0.08;
const GLdouble NoseRadius = 0.15;
const GLdouble NoseLength = 0.15;
const GLdouble BeardRadius = 0.2;
const GLdouble BeardLength = 0.21;
const GLdouble BeardFillR = 0.1;
const GLint MaxAhabLegTheta = 45;
const GLint MinAhabLegTheta = 0;
const GLint MaxArmTheta = 15;
const GLint MinArmTheta = -45;

struct Crew
{
	vector<GLdouble> crewParams;

	GLdouble thumbPos;
	GLint ahabLegTheta;
	GLint thumbAngle;
	GLint rightArmTheta;
	GLint leftArmTheta;
	GLint ahabLegDirection;
	GLint sailorArmDirection;
	bool ahab;
	bool left;

	Crew();
	void Finger();
	void Hand();
	void LowerArm();
	void UpperArm();
	void Arm();
	void Head();
	void UpperBody();
	void Foot();
	void Leg();
	void LowerBody();
	void Sailor();

	void ITNoColor( GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ, 
					GLfloat translateX, GLfloat translateY, GLfloat translateZ, 
					GLfloat degree, GLfloat xAxis, GLfloat yAxis, GLfloat zAxis,
					void( Crew::*method )() )
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