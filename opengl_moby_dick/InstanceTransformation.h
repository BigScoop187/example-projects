#ifndef _INSTANCETRANSFORMATION_H_
#define _INSTANCETRANSFORMATION_H_

#include <stdlib.h>
#include <gl/glut.h>
#include <vector>

using namespace std;

static bool isEye = false;
static bool goldTooth = false;
static bool goldChains = false;

static void Normalize( GLfloat v[3] )
{
	GLfloat sumSqrs = 0;
	GLint i;

	for( i = 0; i < 3; i++ )
		sumSqrs += v[i] * v[i];

	GLfloat length = sqrt(sumSqrs);

	if( length == 0 )
		length = 1;

	for( i = 0; i < 3; i++ )
		v[i] /= length;
}

static void GetSurfaceNormal( GLfloat point1[3], GLfloat point2[3], GLfloat point3[3], GLfloat normal[4] )
{
	GLfloat alpha[3], beta[3];
	for( GLint i = 0; i < 3; i++ )
	{
		beta[i] = point1[i] - point2[i];
		alpha[i] = point3[i] - point2[i];
	}

	normal[0] = alpha[1] * beta[2] - alpha[2] * beta[1];
	normal[1] = alpha[2] * beta[0] - alpha[0] * beta[2];
	normal[2] = alpha[0] * beta[1] - alpha[1] * beta[0];
	normal[3] = 0.0;

	Normalize(normal);
}

static void SetMaterial( GLfloat red, GLfloat green, GLfloat blue, GLfloat MULT, GLint MODE )
{
	GLfloat vec[4];
	vec[0] = red * MULT;
	vec[1] = green * MULT;
	vec[2] = blue * MULT;
	vec[3] = 1.0;
	glMaterialfv( GL_FRONT_AND_BACK, MODE, vec );
}

static void matWithColor( GLfloat red, GLfloat green, GLfloat blue,
						  GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ, 
						  GLfloat translateX, GLfloat translateY, GLfloat translateZ, 
						  GLfloat degree, GLfloat xAxis, GLfloat yAxis, GLfloat zAxis, 
						  void(*func)(vector<GLdouble>), vector<GLdouble> params )
{
	GLfloat shapeColor[4] = {red, green, blue, 1};
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glPushMatrix();

	const GLfloat AMBIENT_MULT = 0.8;
	const GLfloat DIFFUSE_MULT = 0.8;
	const GLfloat SPECULAR_MULT = 0.2;
	
	if( isEye )  // Using obsidian standard material values for Moby's eyes
	{
		SetMaterial( 0.05475, 0.05, 0.06625, 1.0, GL_AMBIENT );
		SetMaterial(0.18275, 0.17, 0.22525, 1.0, GL_DIFFUSE);
		SetMaterial(0.332741, 0.328634, 0.346435, 1.0, GL_SPECULAR);
		glMaterialf(GL_FRONT, GL_SHININESS, 0.3);
	}
	else if( goldTooth || goldChains )
	{
		// Using gold standard material values to give Moby 1 gold tooth
		// and also to make the ropes gold chains, to give Moby a Hip-Hop vibe
		SetMaterial( 0.24725, 0.1995, 0.0745, 1.0, GL_AMBIENT );
		SetMaterial(0.75164, 0.60648, 0.22648, 1.0, GL_DIFFUSE);
		SetMaterial(0.628281, 0.555802, 0.366065, 1.0, GL_SPECULAR);
		glMaterialf(GL_FRONT, GL_SHININESS, 0.4);
	}
	else
	{
		SetMaterial(red, green, blue, AMBIENT_MULT, GL_AMBIENT);
		SetMaterial(red, green, blue, DIFFUSE_MULT, GL_DIFFUSE);
		SetMaterial(red, green, blue, SPECULAR_MULT, GL_SPECULAR);
	}

	glTranslatef( translateX, translateY, translateZ );
	glRotatef( degree, xAxis, yAxis, zAxis );
	glScalef( scaleX, scaleY, scaleZ );

	(*func)(params);
	glPopMatrix();
	glPopAttrib();
}

#endif