#include "Boat.h"
#include "InstanceTransformation.h"

#include "Sailor.h"

Crew boatCrew;

void Boat::tNormalize( GLfloat vec[3] )
{
	GLfloat sum = 0;
	GLint i;

	for( i = 0; i < 3; i++ )
		sum += vec[i] * vec[i];

	GLfloat length = sqrt(sum);

	if( length == 0 )
		length = 1;

	for( i = 0; i < 3; i++ )
		vec[i] /= length;

	int x = 4;
}

void Boat::tSurfaceNormals( GLfloat p1[3], GLfloat p2[3], GLfloat p3[3], GLfloat normal[4] )
{
	GLfloat alpha[3], beta[3];
	for( GLint i = 0; i < 3; i++ )
	{
		beta[i] = p1[i] - p2[i];
		alpha[i] = p3[i] - p2[i];
	}

	normal[0] = alpha[1] * beta[2] - alpha[2] * beta[1];
	normal[1] = alpha[2] * beta[0] - alpha[0] * beta[2];
	normal[2] = alpha[0] * beta[1] - alpha[1] * beta[0];
	normal[3] = 0.0;

	tNormalize( normal );
}

void Boat::setSailProps( GLfloat ambRed, GLfloat ambGreen, GLfloat ambBlue,
					   GLfloat difRed, GLfloat difGreen, GLfloat difBlue,
					   GLfloat spcRed, GLfloat spcGreen, GLfloat spcBlue )
{
	GLfloat ambient[4] = {ambRed, ambGreen, ambBlue, 1.0};
	GLfloat diffuse[4] = {difRed, difGreen, difBlue, 1.0};
	GLfloat specular[4] = {spcRed, spcGreen, spcBlue, 1.0};

	glFrontFace(GL_CW);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient );
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular );
}

void Boat::sail()
{
	GLfloat points[3][3] = { {0.0, 0.0, 0.0},
							 {0.0, 0.0, MastHeight / 2.0},
							 {0.0, MastHeight / 2.0, 0.0}
						   };
	GLfloat norms[3][4];
	
	setSailProps( 0.5, 0.5, 0.5,
				  1.0, 1.0, 1.0, 
				  1.0, 1.0, 1.0 );

	tSurfaceNormals(points[2], points[0], points[1], norms[0] );
	tSurfaceNormals(points[0], points[1], points[2], norms[1] );
	tSurfaceNormals(points[1], points[2], points[0], norms[2] );

	GLuint jollyID = 1;

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &jollyID);
	glBindTexture( GL_TEXTURE_2D, jollyID );
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 25, 30, 0, GL_RGB, GL_UNSIGNED_BYTE, JollyRoger);

	glBegin( GL_TRIANGLES );
		glNormal3fv( norms[0] );
		glTexCoord2f(0.5, 0.0);
		glVertex3fv( points[0] );

		glNormal3fv( norms[1] );
		glTexCoord2f(0.5, 2.0);
		glVertex3fv( points[1] );

		glNormal3fv( norms[2] );
		glTexCoord2f(-1.0, 0.0);
		glVertex3fv( points[2] );
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void Boat::mast()
{
	boatParams[0] = MastR;
	boatParams[1] = MastR;
	boatParams[2] = MastHeight;
	::matWithColor( colors[5].red, colors[5].green, colors[5].blue,
					1, 1, 1, 
					0, 0, -5, 
					0, 0, 0, 0, 
					Cylinder, boatParams );

	::matWithColor( colors[5].red, colors[5].green, colors[5].blue,
					1, 1, 1, 
					0, (MastHeight / 2.0), MastHeight / 1.8, 
					90, 1, 0, 0, 
					Cylinder, boatParams );
	
	ITWithColor( colors[3].red, colors[3].green, colors[3].blue, 
				  2, 2, 2, 
				  -MastR * 1.1, 0, MastHeight / 2, 
				  0, 0, 0, 0, 
				  &Boat::sail );

	ITWithColor( colors[3].red, colors[3].green, colors[3].blue, 
				  2, -2, 2, 
				  -MastR * 1.1, 0, MastHeight / 2, 
				  0, 0, 0, 0,
				  &Boat::sail );
}

void Boat::boat()
{
	// Creating the hull of the boat
	glClipPlane( GL_CLIP_PLANE1, planeEq2 );
	glEnable( GL_CLIP_PLANE1 );
	boatParams.resize(3);
	boatParams[0] = BoatSize;
	::matWithColor( colors[5].red, colors[5].green, colors[5].blue,
					1, 0.4, 0.3, 
					0, 0, 0, 
					90, 1, 0, 0, 
					Sphere, boatParams );
	glDisable( GL_CLIP_PLANE1 );

	// Creating the mast of the boat
	ITNoColor( 1.2, 1.2, 1.5, 
			   0, 0, 0, 
			   -90, 1, 0, 0, 
			   &Boat::mast );

	// Putting some sailors in the boat
	for( int i = 0; i < 3; i++ )
	{
		boatCrew.leftArmTheta = 0;
		boatCrew.ITNoColor( 2, 2, 2, 
							-3, -2, -(6 * i), 
							-90, 0, 1, 0, 
							&Crew::Sailor );
	}
}
