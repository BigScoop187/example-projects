#include "Shapes.h"

void Sphere( vector<GLdouble> params )
{
	glutSolidSphere( params[0], SLICES, STACKS );
}

void Cylinder( vector<GLdouble> params )
{
	GLUquadric *q = gluNewQuadric();
	gluCylinder( q, params[0], params[1], params[2], SLICES, STACKS );
}

void Torus( vector<GLdouble> params )
{
	glutSolidTorus( params[0], params[1], SIDES, RINGS );
}

void Cone( vector<GLdouble> params )
{
	glutSolidCone( params[0], params[1], SLICES, STACKS );
}

void Cube( vector<GLdouble> params )
{
	glutSolidCube( params[0] );
}
