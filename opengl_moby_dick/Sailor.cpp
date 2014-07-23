#include "Sailor.h"

void Crew::Finger()
{
	// Finger
	crewParams[0] = FingerRadius;
	crewParams[1] = FingerRadius * 0.9;
	crewParams[2] = FingerLength;
	::matWithColor( colors[4].red, colors[4].green, colors[4].blue,
					1, 1, 1, 
					0, 0, 0, 
					0, 0, 0, 0,
					Cylinder, crewParams );

	// Finger tip
	::matWithColor( colors[4].red, colors[4].green, colors[4].blue,
					1, 1, 1, 
					0, 0, FingerLength, 
					0, 0, 0, 0,
					Sphere, crewParams );
}
void Crew::Hand()
{
	// Palm
	crewParams[0] = HandBase;
	crewParams[1] = HandTop;
	crewParams[2] = HandLength;
	::matWithColor( colors[4].red, colors[4].green, colors[4].blue,
					1.2, 0.7, 1, 
					0, 0, 0, 
					0, 0, 0, 0,
					Cylinder, crewParams );

	// Knuckles
	for( int i = 0; i < 4; i++ )
	{
		crewParams[0] = FingerRadius;
		::matWithColor( colors[4].red, colors[4].green, colors[4].blue,
						1, 1, 1, 
						(HandTop * 0.7) - (FingerRadius * i), 0, HandLength, 
						0, 0, 0, 0,
						Sphere, crewParams);
	}

	// Fingers
	for( int i = 0; i < 4; i++ )
	{
		ITNoColor( 1, 0.7, 1, 
				   (HandTop* 0.7) - (FingerRadius * i), 0, HandLength, 
				   (10 * -i), 0, 1, 0,
				   &Crew::Finger );
	}

	// Creating a thumb
	thumbPos = -thumbPos;
	thumbAngle = -thumbAngle;
	ITNoColor( 1, 0.6, 1, 
			   thumbPos, 0, ThumbLength, 
			   thumbAngle, 0, 1, 0,
			   &Crew::Finger );
}

void Crew::LowerArm()
{
	// Elbow
	crewParams[0] = ElbowRadius;
	::matWithColor( colors[4].red, colors[4].green, colors[4].blue, 
					1, 1, 1, 
					0, 0, 0, 
					0, 0, 0, 0,
					Sphere, crewParams );
	
	// Forearm
	crewParams[1] = WristRadius;
	crewParams[2] = ArmPieceLength / 1.2;
	::matWithColor( colors[4].red, colors[4].green, colors[4].blue,
					1, 1, 1, 
					0, 0, 0, 
					0, 0, 0, 0,
					Cylinder, crewParams );

	// Wrist
	crewParams[0] = WristRadius;
	::matWithColor( colors[4].red, colors[4].green, colors[4].blue,
					1, 1, 1, 
					0, 0, ArmPieceLength / 1.2, 
					0, 0, 0, 0,
					Sphere, crewParams );

	// Hand
	ITNoColor( 1, 1, 1, 
			   0, 0, ArmPieceLength / 1.2,
			   0, 0, 0, 0,
			   &Crew::Hand );
}

void Crew::UpperArm()
{
	// Shoulder
	crewParams[0] = ShoulderRadius;
	::matWithColor( colors[3].red, colors[3].green, colors[3].blue,
					1, 1, 1, 
					0, 0, 0, 
					0, 0, 0, 0,
					Sphere, crewParams );

	// Arm
	crewParams[1] = ElbowRadius;
	crewParams[2] = ArmPieceLength;
	::matWithColor( colors[3].red, colors[3].green, colors[3].blue,
					1, 1, 1, 
					0, 0, 0, 
					0, 0, 0, 0,
					Cylinder, crewParams );
}

void Crew::Arm()
{
	ITNoColor( 1, 1, 1, 
			   0, 0, 0,
			   0, 0, 0, 0,
			   &Crew::UpperArm );

	ITNoColor( 1, 1, 1, 
			   0, 0, ArmPieceLength,
			   0, 0, 0, 0,
			   &Crew::LowerArm );
}

void Crew::Head()
{
	// Creating a little hair
	glClipPlane( GL_CLIP_PLANE0, planeEq1 );
	glEnable( GL_CLIP_PLANE0 );
		crewParams[0] = HeadRadius;
		::matWithColor( colors[1].red, colors[1].green, colors[1].blue,
						1, 1, 1, 
						0, 0, 0, 
						0, 0, 0, 0,
						Sphere, crewParams );
	glDisable( GL_CLIP_PLANE0 );

	// The head
	::matWithColor( colors[4].red, colors[4].green, colors[4].blue,
					1, 1, 1, 
					0, 0, 0, 
					0, 0, 0, 0,
					Sphere, crewParams );

	// Eyes
	crewParams[0] = EyeRadius;
	::matWithColor( colors[6].red, colors[6].green, colors[6].blue,
					1, 1, 1, 
					HeadRadius * 0.4, -HeadRadius / 1.3, HeadRadius * 0.2, 
					0, 0, 0, 0,
					Sphere, crewParams );
	
	::matWithColor( colors[6].red, colors[6].green, colors[6].blue,
					1, 1, 1, 
					-HeadRadius * 0.4, -HeadRadius / 1.3, HeadRadius * 0.2, 
					0, 0, 0, 0,
					Sphere, crewParams );

	// Nose
	crewParams[0] = NoseRadius;
	crewParams[1] = NoseLength;
	::matWithColor( colors[4].red, colors[4].green, colors[4].blue,
					1, 1, 1, 
					0, -HeadRadius, 0,
					0, 0, 0, 0,
					Cone, crewParams );

	// Beard
	crewParams[0] = BeardRadius;
	crewParams[1] = BeardLength;
	::matWithColor( colors[1].red, colors[1].green, colors[1].blue,
					2, 0.3, -2, 
					0, -HeadRadius, -HeadRadius * 0.2,
					0, 0, 0, 0,
					Cone, crewParams );

	crewParams[0] = crewParams[1] = BeardFillR;
	::matWithColor( colors[1].red, colors[1].green, colors[1].blue,
					1.5, 0.4, 1.5, 
					0, -HeadRadius, -HeadRadius * 0.3,
					0, 0, 0, 0,
					Torus, crewParams );
}

void Crew::UpperBody()
{
	// Torso
	crewParams[0] = TorsoBaseR;
	crewParams[1] = TorsoTopR;
	crewParams[2] = TorsoLength;
	::matWithColor( colors[3].red, colors[3].green, colors[3].blue,
					1, 0.6, 1, 
					0, 0, 0, 
					0, 0, 0, 0,
					Cylinder, crewParams );

	// Torso Top
	crewParams[0] = TorsoTopR;
	::matWithColor( colors[3].red, colors[3].green, colors[3].blue,
					1, 0.6, 0.25, 
					0, 0, TorsoLength, 
					0, 0, 0, 0,
					Sphere, crewParams);
	
	// Neck
	crewParams[0] = crewParams[1] = NeckR;
	crewParams[2] = NeckLength;
	::matWithColor( colors[4].red, colors[4].green, colors[4].blue,
					1, 1, 1, 
					0, 0, TorsoLength, 
					0, 0, 0, 0,
					Cylinder, crewParams );

	// Head
	ITNoColor( 1, 1.2, 1.4, 
			   0, -0.1, NeckLength / 0.21,
			   0, 0, 0, 0,
			   &Crew::Head );

	int xAxis = 1;
	int yAxis = 0;
	if( ahab )
	{
		leftArmTheta = 45;
		rightArmTheta = -45;
		xAxis = 0;
		yAxis = 1;
	}
	
	// Left Arm
	left = true;
	ITNoColor( 1, 1, 1, 
			   TorsoTopR, 0, TorsoLength - ShoulderRadius, 
			   leftArmTheta, xAxis, yAxis, 0,
			   &Crew::Arm );

	// Right Arm
	left = false;
	xAxis = 0;
	yAxis = 1;
	ITNoColor( 1, 1, 1, 
			   -TorsoTopR, 0, TorsoLength - ShoulderRadius, 
			   rightArmTheta, xAxis, yAxis, 0,
			   &Crew::Arm );
}

void Crew::Foot()
{
	// Body of the foot
	crewParams[0] = crewParams[1] = AnkleR;
	crewParams[2] = LegPieceLength / 1.8;
	::matWithColor( colors[5].red, colors[5].green, colors[5].blue,
					1, 1, 1, 
					0, 0, 0, 
					0, 0, 0, 0,
					Cylinder, crewParams );

	// Tip of the foot
	::matWithColor( colors[5].red, colors[5].green, colors[5].blue,
					1, 1, 1, 
					0, 0, LegPieceLength / 1.8, 
					0, 0, 0, 0,
					Sphere, crewParams );
}

void Crew::Leg()
{
	// Hip
	crewParams[0] = ThighUpperR;
	::matWithColor( colors[5].red, colors[5].green, colors[5].blue,
					1, 1, 1, 
					0, 0, 0, 
					0, 0, 0, 0,
					Sphere, crewParams );

	// Thigh
	crewParams[1] = ThighLowerR;
	crewParams[2] = LegPieceLength;
	::matWithColor( colors[5].red, colors[5].green, colors[5].blue,
					1, 1, 1, 
					0, 0, 0, 
					0, 0, 0, 0,
					Cylinder, crewParams );

	if( ahab && left )
	{
		crewParams[0] = ThighUpperR * 1.1;
		crewParams[1] = ThighLowerR * 0.8;
		crewParams[2] = LegPieceLength * 0.3;

		// Peg Leg Base
		::matWithColor( colors[3].red, colors[3].green, colors[3].blue,
						1, 1, 1, 
						0, 0, LegPieceLength - 0.2, 
						0, 0, 0, 0,
						Cylinder, crewParams );

		crewParams[0] = ThighUpperR * 0.5;
		crewParams[1] = ThighLowerR * 0.3;
		crewParams[2] = LegPieceLength + AnkleR;

		// Peg Leg Shaft
		::matWithColor( colors[3].red, colors[3].green, colors[3].blue,
						1, 1, 1, 
						0, 0, LegPieceLength, 
						0, 0, 0, 0,
						Cylinder, crewParams );
	}
	else
	{
		GLdouble footZ = (LegPieceLength * 2) + (ThighUpperR / 2.0);
		GLdouble ankleZ = LegPieceLength * 2.0;

		// Knee
		crewParams[0] = ThighLowerR;
		::matWithColor( colors[5].red, colors[5].green, colors[5].blue,
						1, 1, 1, 
						0, 0, LegPieceLength, 
						0, 0, 0, 0,
						Sphere, crewParams );

		// Shin
		crewParams[1] = AnkleR;
		crewParams[2] = LegPieceLength;
		::matWithColor( colors[4].red, colors[4].green, colors[4].blue,
						1, 1, 1, 
						0, 0, LegPieceLength, 
						0, 0, 0, 0, 
						Cylinder, crewParams );

		// Ankle
		crewParams[0] = AnkleR;
		::matWithColor( colors[5].red, colors[5].green, colors[5].blue,
						1, 1, 1, 
						0, 0, ankleZ, 
						0, 0, 0, 0, 
						Sphere, crewParams );

		// Foot
		ITNoColor( 1, 0.5, 1, 
				   0, 0, footZ,
				   -90, 1, 0, 0,
				   &Crew::Foot );
	}
}

void Crew::LowerBody()
{
	GLint rotation = 0;
	GLint yAxis = 0;
	GLint axis = 0;

	if( ahab )
	{
		rotation = ahabLegTheta;
		yAxis = 1;
	}
	else
	{
		rotation = 0;
		yAxis = 0;
	}

	// Left Leg
	left = false;
	ITNoColor( 1, 1, 1, 
			   ThighUpperR, 0, 0,
			   rotation, axis, yAxis, axis,
			   &Crew::Leg );

	// Right leg
	left = true;
	ITNoColor( 1, 1, 1, 
			   -ThighUpperR, 0, 0,
			   -rotation, axis, yAxis, axis,
			   &Crew::Leg );	
}

void Crew::Sailor()
{
	ITNoColor( 1, 1, 1, 
			   0, 0, 0, 
			   -90, 1, 0, 0, 
			   &Crew::UpperBody );
	
	ITNoColor( 1, 1, 1, 
			   0, 0, 0, 
			   90, 1, 0, 0,
			   &Crew::LowerBody );
}

Crew::Crew()
{
	crewParams.resize(3, 0);
	ahab = false;
	ahabLegTheta = 0;
	ahabLegDirection = 1;
	sailorArmDirection = 1;
	left = false;
	thumbAngle = 30;
	thumbPos = WristRadius * 0.8;
	rightArmTheta = 0;
	leftArmTheta = 90;
}