#include <WTypes.h>
#include <math.h>
#include <iostream>
#include "InstanceTransformation.h"
#include "MobyDick.h"
#include "Sailor.h"
#include "Boat.h"
#include <vector>

using namespace std;

const GLdouble mobyAngle = 60.0;
const GLdouble camY = 50;
const GLdouble PI = acos(-1.0);
const GLdouble ToRads = (PI / 180.0);
const GLdouble r = 0.5;
const GLdouble MaxWaveOuterR = MobyDickLength * GirthPercentage;
const GLint MaxMobyDepth = -5;
const GLint MaxBoatDepth = MobyDickLength + MaxMobyDepth;
const GLint MaxBoatTheta = 5;
const GLint MinBoatTheta = -5;
const GLint OceanDimension = 75;
const GLint SceneSize = 600;

const GLfloat sunAmb[4] = {0.3, 0.3, 0.3, 1.0};
const GLfloat sunDif[4] = {0.5, 0.5, 0.5, 1.0};
const GLfloat sunSpc[4] = {0.5, 0.5, 0.5, 1.0};

GLdouble camX = -250;
GLdouble camZ = 0;
GLdouble focusX = 0;
GLdouble focusY = camY;
GLdouble focusZ = 0;
GLdouble theta = 0;
GLdouble phi = 0;
GLdouble movementDirection = 0;
GLdouble boatTheta = 0;
GLdouble sunSize = 20;
GLdouble sunY = 0;
GLdouble sunX = -300;
GLdouble sunArcRadius = 300;
GLdouble sunTheta = 0;
GLdouble sunSpeed = 0.5;
GLint boatDirection = 1;
GLint boatDepth = 10;
GLint MobyDepth = -160;
GLint MobyX = 50;
GLint MobyZ = -100;
GLint Screen_Size_Width = 0;
GLint Screen_Size_Height = 0;
GLint Window_Width = 750;
GLint Window_Height = 750;
bool stop = false;
bool spotOff = false;

GLfloat sunPosition[4] = {sunX, sunY, 0, 1};
GLfloat sunColor[4] = {1.0, 1.0, 0.0, 1.0};
GLfloat spotPosition[4] = {camX, camY, camZ, 1};
GLfloat spotDirection[4];
GLfloat spotColor[4] = {1.0, 1.0, 0.8, 1.0};

// Whale object
MobyDick mobyDick;

// Sailor object
Crew crew;

// Boat Object
Boat boat;

// Texture stuff from the textures project's texture.cpp
typedef struct { 
	unsigned char r, g, b; 
} RGB;

typedef struct {
	int BitCt;
	unsigned long Width, Height;
	unsigned int PalSize;
	unsigned char *map;
	RGB *palette; 
} BMP;

enum TEXS { MANY_CLOUDS, OCEAN, SKY, SUN };
const int NUM_TEXS = 4;
GLuint texIDs[NUM_TEXS];

//***********************************************************************
// Function Name:	ReadBMPFile()
// Purpose		:	Reads in a bitmap, stores it in a BMP structure,
//					used to load in textures.
//***********************************************************************

void ReadBMPFile(char *fname, BMP *r) {
	HANDLE hFile; 
	char buffer[16];
	int i;
	unsigned long ImgOffset;
	unsigned long dummy;

	wchar_t wText[100];
	::MultiByteToWideChar(CP_ACP, NULL, fname, -1, wText, strlen(fname)+1);
	for (i=0; i < 16; i++) 
		buffer[i] = 0;
	hFile=CreateFile(wText, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) { 
		r->map = NULL; 
		return; 
	}
	ReadFile(hFile, buffer, 2, &dummy, NULL);
	if (strcmp(buffer, "BM")) { 
		r->map = NULL; 
		return; 
	}
	SetFilePointer(hFile, 8, NULL, FILE_CURRENT);
	ReadFile(hFile, &ImgOffset, 4, &dummy, NULL);
	SetFilePointer(hFile, 4, NULL, FILE_CURRENT);
	ReadFile(hFile, &r->Width, 4, &dummy, NULL);
	ReadFile(hFile, &r->Height, 4, &dummy, NULL);
	SetFilePointer(hFile, 2, NULL, FILE_CURRENT);
	r->BitCt=0;
	ReadFile(hFile, &r->BitCt, 2, &dummy, NULL);
	SetFilePointer(hFile, 24, NULL, FILE_CURRENT);
	switch (r->BitCt) {
		case 1: r->PalSize = 2;  break;
		case 4: r->PalSize = 16; break;
		case 8: r->PalSize = 256;break;
		default: r->PalSize = 0; break; 
	}
	if (r->PalSize != 0) {
		r->palette=(RGB*)LocalAlloc(LPTR, r->PalSize * sizeof(RGB));
		for (i = 0; i < r->PalSize; i++) {
			ReadFile(hFile, buffer, 4, &dummy, NULL);
			r->palette[i].b = buffer[0];
        	r->palette[i].g = buffer[1];
        	r->palette[i].r = buffer[2]; 
		} 
	}
	else 
		r->palette=NULL;
	SetFilePointer(hFile,ImgOffset,NULL,FILE_BEGIN);
	r->map = (byte*)LocalAlloc(LPTR, r->Width*r->Height*r->BitCt/8);
	ReadFile(hFile, r->map, r->Width*r->Height*r->BitCt/8, &dummy,NULL);
	CloseHandle(hFile);
	
	return; 
}

void CreateTextureFromBitmap(char *fname, GLuint &id)
{
	BMP image;
	ReadBMPFile(fname, &image);

	glBindTexture(GL_TEXTURE_2D, id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.Width, image.Height, 
				0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image.map);
}

void move( int id )
{
	// Moving Moby Up
	if( MobyDepth < MaxMobyDepth )
		MobyDepth += 1;
	// Creating a wave
	if( mobyDick.waveOuterR < MaxWaveOuterR && (MobyDepth > -(MobyDickLength - MinWavePos) && MobyDepth < MaxMobyDepth) )
		mobyDick.waveOuterR += 1.0;
	if( MobyDepth > -(MobyDickLength - MinWavePos) && (mobyDick.wavePosition > MinWavePos) )
		mobyDick.wavePosition -= (mobyDick.wavePosition - MinWavePos) / -(MobyDepth - MaxMobyDepth);

	// Moving the ship
	if( MobyDepth > -(MobyDickLength - MinWavePos) && (boatDepth < MaxBoatDepth) )
		boatDepth += 1.0;
	if( boatTheta > MaxBoatTheta )
		boatDirection = -1;
	else if( boatTheta < MinBoatTheta )
		boatDirection = 1;
	boatTheta += boatDirection;

	// Moving the jaw back and forth
	if( mobyDick.jawTheta > MaxJawTheta )
		mobyDick.jawDirection = -1;
	else if( mobyDick.jawTheta < MinJawTheta )
		mobyDick.jawDirection = 1;
	mobyDick.jawTheta += mobyDick.jawDirection;

	// Moving poor ahab's legs
	if( mobyDick.victims.ahabLegTheta > MaxAhabLegTheta )
		mobyDick.victims.ahabLegDirection = -1;
	else if( mobyDick.victims.ahabLegTheta < MinAhabLegTheta )
		mobyDick.victims.ahabLegDirection = 1;
	mobyDick.victims.ahabLegTheta += mobyDick.victims.ahabLegDirection;
	
	// Moving an arm for help
	if( crew.rightArmTheta > MaxArmTheta )
		crew.sailorArmDirection = -2;
	else if( crew.rightArmTheta < MinArmTheta )
		crew.sailorArmDirection = 2;
	crew.rightArmTheta += crew.sailorArmDirection;

	// Moving the sun
	sunTheta += sunSpeed;
	sunX = -( sunArcRadius * cos(sunTheta * ToRads) );
	sunY = sunArcRadius * sin(sunTheta * ToRads);
	if( sunY - 0.00000001 < 0 )
	{
		sunX = -600;
		sunY = 0;
		sunTheta = 0;
	}
	sunPosition[0] = sunX;
	sunPosition[1] = sunY;

	if( !stop )
	{
		camX += cos( movementDirection );
		camZ += sin( movementDirection );
	}

	// FocalPoint
	focusX = camX + (r * cos(phi) * cos(theta));
	if( theta != (90.0 * ToRads) )
		focusY = camY + (r * sin(theta));
	else
		focusY = camY + (r * sin(theta - .000000001));
	
	focusZ = camZ + (r * cos(theta) * sin(phi));

	// Setting the spotlights values
	spotPosition[0] = camX;
	spotPosition[2] = camZ;
	spotDirection[0] = focusX - camX;
	spotDirection[1] = focusY - camY;
	spotDirection[2] = focusZ - camZ;
	
	::Normalize(spotDirection);

	glutTimerFunc( 50, move, 1 );
	glutPostRedisplay();
}

void boundaries()
{
	GLint i, j;
	GLint heightAdjust = -5;
	GLfloat dim = SceneSize / 2.0;
	GLfloat norms[5][4][3];
	GLfloat vert[5][4][3] = {	{{-dim, heightAdjust, -dim}, {dim, heightAdjust, -dim}, {dim, dim, -dim}, {-dim, dim, -dim}},	//Left Wall = vert[0]
								{{dim, heightAdjust, -dim}, {dim, heightAdjust, dim}, {dim, dim, dim}, {dim, dim, -dim}},		//Front Wall = vert[1]
								{{dim, heightAdjust, dim}, {-dim, heightAdjust, dim}, {-dim, dim, dim}, {dim, dim, dim}},		//Right Wall = vert[2]
								{{-dim, heightAdjust, dim}, {-dim, heightAdjust, -dim}, {-dim, dim, -dim}, {-dim, dim, dim}},	//Back Wall = vert[3]
								{{ -dim, dim, dim}, {-dim, dim, -dim}, {dim, dim, -dim}, {dim, dim, dim}}						//Ceiling = vert[4]
							};

	for( i = 0; i < 5; i++ )
		for( j = 0; j < 3; j++ )
			::GetSurfaceNormal( vert[i][j], vert[i+1][j], vert[i+1][j+1], norms[i][j] );
		
	// Walls
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, (texIDs[MANY_CLOUDS]));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	for( i = 0; i < 4; i++ )
	{
		glBegin(GL_POLYGON);
			glNormal3fv( norms[i][0] );
			glTexCoord2f((0.25 * i), 0.4);
			glVertex3fv( vert[i][0] );

			glNormal3fv( norms[i][1] );
			glTexCoord2f((0.25 * i)+.25, 0.4);
			glVertex3fv( vert[i][1] );

			glNormal3fv( norms[i][2] );
			glTexCoord2f((0.25 * i)+.25, 1.0);
			glVertex3fv( vert[i][2] );

			glNormal3fv( norms[i][3] );
			glTexCoord2f((0.25 * i), 1.0);
			glVertex3fv( vert[i][3] );
		glEnd();
	}
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, (texIDs[SKY]));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBegin(GL_POLYGON);
		glNormal3fv( norms[4][0] );
		glTexCoord2f(0.0, 1.0);
		glVertex3fv( vert[4][0] );

		glNormal3fv( norms[4][1] );
		glTexCoord2f(0.0, 0.0);
		glVertex3fv( vert[4][1] );

		glNormal3fv( norms[4][2] );
		glTexCoord2f(1.0, 0.0);
		glVertex3fv( vert[4][2] );

		glNormal3fv( norms[4][3] );
		glTexCoord2f(1.0, 1.0);
		glVertex3fv( vert[4][3] );
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

// The ocean
void ocean()
{
	GLfloat ambient[4] = {0.5, 0.5, 0.8, 1};
	GLfloat diffuse[4] = {0.5, 0.5, 0.6, 1};
	GLfloat specular[4] = {0.4, 0.4, 0.6, 1};
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular);

	GLfloat offset = SceneSize / OceanDimension;
	GLfloat vals[OceanDimension][OceanDimension][3];
	GLfloat norms[OceanDimension][OceanDimension][3];
	GLint r, c;

	for( r = 0; r < OceanDimension; r++ )
		for( c = 0; c < OceanDimension; c++ )
		{
			vals[r][c][0] = (-300 + (offset * c));
			vals[r][c][1] = 0.0;
			vals[r][c][2] = (-300 + (offset * r));
		}

	for( r = 0; r < OceanDimension - 1; r++ )
		for(c = 0; c < OceanDimension - 1; c++ )
			GetSurfaceNormal( vals[r][c], vals[r+1][c], vals[r+1][c+1], norms[r][c] );

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texIDs[OCEAN]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLfloat texOffset = ( offset / SceneSize );

	for( r = 0; r < OceanDimension - 1; r++ )
		for( c = 0; c < OceanDimension - 1; c++ )
		{
			glBegin(GL_POLYGON);
				glNormal3fv( norms[r][c] );
				glTexCoord2f(0.0,0.0);
				glVertex3fv( vals[r][c] );

				glNormal3fv( norms[r+1][c] );
				glTexCoord2f(1.0,0.0);
				glVertex3fv( vals[r+1][c] );

				glNormal3fv( norms[r+1][c+1] );
				glTexCoord2f(1.0,1.0);
				glVertex3fv( vals[r+1][c+1] );

				glNormal3fv( norms[r][c+1] );
				glTexCoord2f(0.0,1.0);
				glVertex3fv( vals[r][c+1] );
			glEnd();
		}
	glDisable(GL_TEXTURE_2D);
}

// The Sun/Source of light
void sun()
{
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texIDs[SUN]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	GLUquadric *sun = gluNewQuadric();
	gluQuadricDrawStyle(sun, GLU_FILL);
    gluQuadricTexture(sun, GL_TRUE);
    gluQuadricNormals(sun, GLU_SMOOTH);

	glMaterialfv(GL_FRONT, GL_EMISSION, sunColor );
	glTranslatef( sunPosition[0], sunPosition[1], 0 );
	gluSphere( sun, sunSize, SLICES, STACKS );
		
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopAttrib();
}

// Main light in the scene
void setSunLight()
{
	glLightfv( GL_LIGHT0, GL_POSITION, sunPosition );
	glLightfv( GL_LIGHT0, GL_AMBIENT, sunAmb );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, sunDif );
	glLightfv( GL_LIGHT0, GL_SPECULAR, sunSpc );
	glEnable( GL_LIGHT0 );
}

void setSpotLight()
{
	glLightf( GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1 );
	glLightf( GL_LIGHT1, GL_LINEAR_ATTENUATION, 0 );
	glLightf( GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0 );

	glLightfv( GL_LIGHT1, GL_POSITION, spotPosition );
	glLightfv( GL_LIGHT1, GL_AMBIENT, spotColor );
	glLightfv( GL_LIGHT1, GL_DIFFUSE, spotColor );
	glLightfv( GL_LIGHT1, GL_SPECULAR, spotColor );
	glLightfv( GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection );
	glLightf( GL_LIGHT1, GL_SPOT_EXPONENT, 100 );
	glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, 15 );
	if( !spotOff )
		glEnable( GL_LIGHT1 );
}

void display( void )
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	gluPerspective( 60, GLdouble(Window_Width) / Window_Width, 0.1, 700 );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	gluLookAt( camX, camY, camZ, focusX, focusY, focusZ, 0, 1, 0 );
	
	// Setting the sunlight
	setSunLight();

	// Setting the spotlight
	setSpotLight();

	// Drawing the ocean
	ocean();

	// Drawing the scenery
	boundaries();

	// Drawing the sun
	sun();
	
	// Drawing the boat
	boat.ITNoColor( 1, 1, 1, 
					MobyX, boatDepth, MobyZ + (BoatSize / 2.0), 
					boatTheta, 1, 0, 1,
					&Boat::boat );

	// Drawing a sailor
	crew.ITNoColor( 1, 1, 1, 
					0, -TorsoLength * 0.8, 0, 
					-90, 0, 1, 0,
					&Crew::Sailor );
	
	// Drawing the white whale
	mobyDick.ITNoColor( -1, 1, 1, 
					    MobyX, MobyDepth, MobyZ, 
					    -mobyAngle, 1, 0, 0,
					    &MobyDick::Whale );
	
	glFlush();
	glutSwapBuffers();
}

// Function to put the window in the middle of the screen
void setScreenSize()
{
	RECT screen;
	const HWND tempScreen = GetDesktopWindow();
	GetWindowRect( tempScreen, &screen );

	Screen_Size_Width = screen.right;
	Screen_Size_Height = screen.bottom;

	int x = (Screen_Size_Width / 2) - ( Window_Width / 2 );
	int y = (Screen_Size_Height / 2) - ( Window_Height / 2 );
	glutInitWindowPosition( x, y );
}

void keyboard( unsigned char key, int x, int y )
{
	if( key == 'c' || key == 'C' )
	{
		movementDirection = phi;
	}
	else if( key == 'r' || key == 'R' )
	{
		theta = 0;
		phi = movementDirection;
	}
	else if( key =='w' || key == 'W' )
	{
		stop = !stop;
	}
	else if( key == 's' || key == 'S' )
	{
		spotOff = !spotOff;
		if( !spotOff )
			glEnable( GL_LIGHT1 );
		else
			glDisable( GL_LIGHT1 );
	}
}

void special( int key, int x, int y )
{
	if( key == GLUT_KEY_RIGHT )
	{
		phi += (10 * ToRads);
		glutPostRedisplay();
	}
	else if( key == GLUT_KEY_LEFT )
	{
		phi -= (10 * ToRads);
		glutPostRedisplay();
	}
	else if( key == GLUT_KEY_UP )
	{
		if( (theta * 1 / ToRads) < 91.0 && (theta * 1 / ToRads) < 89.0 )
			theta += (10 * ToRads);
		glutPostRedisplay();
	}
	else if( key == GLUT_KEY_DOWN )
	{
		if( (theta * (1 / ToRads)) > -60.0 && (theta * (1 / ToRads)) > -59 )
			theta -= (10 * ToRads);
		glutPostRedisplay();
	}	

	// To show the proper angles
	cout << "Phi = " << (phi * (1 / ToRads)) << endl;
	cout << "Theta = " << (theta * (1 / ToRads)) << endl;
}

void initLighting()
{
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glShadeModel( GL_SMOOTH );
	glEnable( GL_LIGHTING );
	glEnable( GL_DEPTH_TEST );
}

void initTextures()
{
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(NUM_TEXS, texIDs);
	CreateTextureFromBitmap("sky.bmp", texIDs[SKY]);
	CreateTextureFromBitmap("oceanManyClouds.bmp", texIDs[MANY_CLOUDS]);
	CreateTextureFromBitmap("ocean.bmp", texIDs[OCEAN]);
	CreateTextureFromBitmap("sun.bmp", texIDs[SUN]);
	glDisable(GL_TEXTURE_2D);
}

void main( int argc, char **argv )
{
	glutInit( &argc, argv );

	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( Window_Width, Window_Height );
	setScreenSize();
	glutCreateWindow( "hawortbt Moby Dick" );
	initLighting();
	initTextures();
	glutDisplayFunc( display );
	glutTimerFunc( 50, move, 1 );
	glutKeyboardFunc( keyboard );
	glutSpecialFunc( special );

	glutMainLoop();
}