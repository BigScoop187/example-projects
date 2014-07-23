#ifndef _SHAPES_H_
#define _SHAPES_H_

#include <stdlib.h>
#include <gl/glut.h>
#include <vector>

using namespace std;

const GLint SLICES = 20;
const GLint STACKS = 20;
const GLint SIDES = 20;
const GLint RINGS = 20;

void Sphere( vector<GLdouble> params );
void Cylinder( vector<GLdouble> params );
void Torus( vector<GLdouble> params );
void Cone( vector<GLdouble> params );
void Cube( vector<GLdouble> params );

#endif