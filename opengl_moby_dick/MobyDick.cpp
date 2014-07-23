#include "MobyDick.h"

void MobyDick::Jaw()
{
	GLdouble baseRadius = MobyDickLength * .05;
	GLdouble topRadius = baseRadius * MobyTipPercentage;
	GLdouble height = MobyDickLength * 0.30;

	// Creating the body of the jaw
	mobyParams[0] = baseRadius;
	mobyParams[1] = topRadius;
	mobyParams[2] = height;
	::matWithColor( colors[0].red, colors[0].green, colors[0].blue,
					1, JawScale, 1,
					0, 0, 0,
					0, 0, 0, 0,
					Cylinder, mobyParams );
	
	// Creating the tip of the jaw
	mobyParams[0] = topRadius;
	::matWithColor( colors[0].red, colors[0].green, colors[0].blue,
					1, JawScale, 1,
					0, 0, height,
					0, 0, 0, 0,
					Sphere, mobyParams );

	// Creating two rows of teeth
	GLdouble conRadius = MobyDickLength * 0.02;
	GLdouble j = baseRadius * 0.7;
	GLint numbTeeth = height;

	mobyParams[0] = conRadius;
	mobyParams[1] = MobyDickLength * 0.04;
	
	for( int i = 0; i < numbTeeth; i++ )
	{
		if( i == (numbTeeth - 1) )
			::goldTooth = true;
		::matWithColor( colors[3].red, colors[3].green, colors[3].blue,
						ToothXScale, ToothYScale, ToothZScale, 
						j, 0, i + 2,
						-90, 1, 0, 0,
						Cone, mobyParams );
		::goldTooth = false;
		j -= (conRadius * .02);
	}

	j = -( baseRadius * 0.7 );

	for( int i = 0; i < numbTeeth; i++ )
	{
		::matWithColor( colors[3].red, colors[3].green, colors[3].blue,
						ToothXScale, ToothYScale, ToothZScale, 
						j, 0, i + 2,
						-90, 1, 0, 0,
						Cone, mobyParams );
		j += (conRadius * .02);
	}
	

	// Putting a poor sailor in the whale's mouth
	victims.ITNoColor( 1, 1, 1, 
					   0,  (MobyDickLength * .05) * 0.25, (MobyDickLength * 0.3) / 1.1,
					   -90, 0.5, 0, 0.5,
					   &Crew::Sailor );
}

void MobyDick::Eye()
{
	// Creating the eye ball
	mobyParams[0] = 1.0;
	::isEye = true;
	::matWithColor( colors[1].red, colors[1].green, colors[1].blue,
					1, 1, 1,
					0, 0, 0,
					0, 0, 0, 0,
					Sphere, mobyParams );
	::isEye = false;
	// Creating the socket
	mobyParams[0] = 0.5;
	mobyParams[1] = 1.0;
	::matWithColor( colors[0].red, colors[0].green, colors[0].blue,
					1, 1, 1, 
					0, 0, 0,
					0, 0, 0, 0,
					Torus, mobyParams );
}

void MobyDick::Ropes()
{
	mobyParams[0] = RopeWidth;
	mobyParams[1] = MobyDickLength * GirthPercentage;
	GLdouble positionOffSet = MobyDickLength * .04;

	::goldChains = true;
	// Creating the ropes
	::matWithColor( colors[2].red, colors[2].green, colors[2].blue,
					AhabRopeScale, 1, 1, 
					0, 0, 0, 
					25, 1, 0, 0,
					Torus, mobyParams );

	::matWithColor( colors[2].red, colors[2].green, colors[2].blue,
					AhabRopeScale, 1, 1, 
					0, 0, 0, 
					-25, 1, 0, 0,
					Torus, mobyParams );

	::matWithColor( colors[2].red, colors[2].green, colors[2].blue,
					AhabRopeScale, 1, 1, 
					0, 0, 0, 
					10, 1, 0, 0,
					Torus, mobyParams );

	::matWithColor( colors[2].red, colors[2].green, colors[2].blue,
					1, 1, 1, 
					0, 0, positionOffSet, 
					0, 0, 0, 0,
					Torus, mobyParams );

	::matWithColor( colors[2].red, colors[2].green, colors[2].blue,
					1, 1, 1, 
					0, 0, -positionOffSet, 
					0, 0, 0, 0,
					Torus, mobyParams );

	::matWithColor( colors[2].red, colors[2].green, colors[2].blue,
					1, 1, 1, 
					0, 0, -(positionOffSet * 1.2), 
					0, 0, 0, 0,
					Torus, mobyParams );
	::goldChains = false;
}

void MobyDick::MobyBody()
{
	GLdouble baseRadius = MobyDickLength * GirthPercentage;	// Moby's middle girth

	// Creating the body
	mobyParams[0] = baseRadius;
	mobyParams[1] = baseRadius * MobyTipPercentage;
	mobyParams[2] = MobyDickLength / 1.5;
	::matWithColor( colors[0].red, colors[0].green, colors[0].blue,
					1, 1, 1, 
					0, 0, 0, 
					0, 0, 0, 0,
					Cylinder, mobyParams );

	GLdouble ropePosition = MobyDickLength * 0.30;

	// Ensnaring poor Ahab on Moby Dick
	victims.ahab = true;
	victims.ITNoColor( 1.5, 1.5, 1.5, 
					   baseRadius, 0, ropePosition, 
					   90, 0, 1, 0,
					   &Crew::Sailor );
	victims.ahab = false;
	
	// Creating the ropes
	ITNoColor( 1, 1, 1, 
			   0, 0, ropePosition, 
			   0, 0, 0, 0,
			   &MobyDick::Ropes );

	// Creating a watery ring to simulate water rising
	mobyParams[0] = 4;
	mobyParams[1] = waveOuterR;
	::matWithColor( colors[7].red, colors[7].green, colors[7].blue,
					1, 1, 1, 
					0, 0, wavePosition,
					90 + MobyAngle, 1, 0, 0,
					Torus, mobyParams );
}

void MobyDick::MobyHead()
{
	GLdouble headRadius = (MobyDickLength * GirthPercentage) * MobyTipPercentage;
	GLdouble height = MobyDickLength / 1.5;

	// Creating the head
	mobyParams[0] = headRadius;
	::matWithColor( colors[0].red, colors[0].green, colors[0].blue,
					1, 1, 1, 
					0, 0, height, 
					0, 0, 0, 0,
					Sphere, mobyParams );

	// Creating the jaw
	GLdouble jawYOffset = -headRadius;
	GLdouble jawZOffset =  (MobyDickLength / 1.5) * JawLocation;
	ITNoColor( 1, 1, 1, 
			   0, jawYOffset, jawZOffset,
		       jawTheta, 1, 0, 0,
			   &MobyDick::Jaw );

	// Creating the eyes
	GLdouble eyeX = ((MobyDickLength * GirthPercentage) * MobyTipPercentage) * 0.85;
	GLdouble eyeY = (MobyDickLength * GirthPercentage) * 0.5;
	GLdouble eyeZ = (-jawYOffset) * 1.6;
	ITNoColor( 2, 2, 1,
			   eyeX, -eyeY, eyeZ,
			   90, 0.6, 1, 0,
			   &MobyDick::Eye );

	ITNoColor( 2, 2, 1, 
			   -eyeX, -eyeY, eyeZ, 
			   90, -0.6, 1, 0,
			   &MobyDick::Eye );
}

void MobyDick::Whale()
{
	// Creating the body
	ITNoColor( MobySlimFactor, 1, 1, 
				0, 0, 0, 
				0, 0, 0, 0,
				&MobyDick::MobyBody );

	// Creating the body
	ITNoColor( MobySlimFactor, 1, 1, 
			   0, 0, 0, 
			   0, 0, 0, 0,
			   &MobyDick::MobyHead );
}

MobyDick::MobyDick()
{
	mobyParams.resize(3, 0);
	wavePosition = MobyDickLength;
	waveOuterR = 0;
	jawTheta = 0;
	jawDirection = -1;
}