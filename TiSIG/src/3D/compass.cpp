#include "compass.h"

#include <cmath>


QVector3D makePtForCompas(float dist, float angle, float e, float angle2) {
	return (dist + e*cos(angle2)) * QVector3D(cos(angle), sin(angle), e*sin(angle2));
};

QVector3D makePtNormForCompas(float angle, float angle2) {
	return makePtForCompas(1, angle, 1, angle2) - makePtForCompas(1, angle, 0, angle2);
};

CompassFactory::CompassFactory()
{
	float a=.5, b=.12, c=.1, d = .8, e = 0.05;
	int nA1=48, nA2=12; // resolution circle

	this->position = {
		// arrow north
		{0, 0, c}, {-b, b, 0}, {0, a, 0},
		{0, 0, c}, {b, b, 0}, {0, a, 0},
		// arrow east
		{0, 0, c}, {b, b, 0}, {a, 0, 0},
		{0, 0, c}, {b, -b, 0}, {a, 0, 0},
		// arrow south
		{0, 0, c}, {b, -b, 0}, {0, -a, 0},
		{0, 0, c}, {-b, -b, 0}, {0, -a, 0},
		// arrow west
		{0, 0, c}, {-b, -b, 0}, {-a, 0, 0},
		{0, 0, c}, {-b, b, 0}, {-a, 0, 0},

		{0,0,0}, {0,0,0}, {0,0,0},
		{d,0,0}, {d,0,0}, {d,0,0},
	};
	this->normal = {};
	for (int i=0; i<(int)this->position.size()/3; i++) {
		QVector3D v = this->position[3*i+1];
		this->normal.push_back({v.x(), v.y(), b});
		this->normal.push_back(v);
		this->normal.push_back(v);
	}
	this->textPath = ":/compas.png";
	QVector2D colorNorth[3]={{0,1}, {0.5, 1}, {0.5, 0.75}};
	QVector2D colorEast[3]={{0.5,0.75}, {0.5, 1}, {1, 1}};
	QVector2D colorSouth[3]={{0.5,0.75}, {0.5, 0.5}, {1, 0.5}};
	QVector2D colorWest[3]={{0,0.75}, {0, 0.5}, {0.5, 0.5}};
	QVector2D colorCircle[3]={{0.5,0.5}, {1, 0.5}, {0.5, 0.25}};

	this->textCoord = {
		colorNorth[0], colorNorth[1], colorNorth[2],
		colorNorth[0], colorNorth[1], colorNorth[2],

		colorEast[0], colorEast[1], colorEast[2],
		colorEast[0], colorEast[1], colorEast[2],

		colorSouth[0], colorSouth[1], colorSouth[2],
		colorSouth[0], colorSouth[1], colorSouth[2],

		colorWest[0], colorWest[1], colorWest[2],
		colorWest[0], colorWest[1], colorWest[2],

		{0,0}, {0,0}, {0,0},
		{0,0}, {0,0}, {0,0},
	};

	for (int i=0; i<nA1; i++) {
		for (int k=0; k<nA2; k++) {
			QVector3D p1 = makePtForCompas(d, 2.0*M_PI*  i  /nA1, e, 2.*  k  /nA2*M_PI);
			QVector3D p2 = makePtForCompas(d, 2.0*M_PI*  i  /nA1, e, 2.*(k+1)/nA2*M_PI);
			QVector3D p5 = makePtForCompas(d, 2.0*M_PI*(i+1)/nA1, e, 2.*  k  /nA2*M_PI);
			QVector3D p6 = makePtForCompas(d, 2.0*M_PI*(i+1)/nA1, e, 2.*(k+1)/nA2*M_PI);

			QVector3D n1 = makePtNormForCompas(2.0*M_PI*  i  /nA1, 2.*  k  /nA2*M_PI);
			QVector3D n2 = makePtNormForCompas(2.0*M_PI*  i  /nA1, 2.*(k+1)/nA2*M_PI);
			QVector3D n5 = makePtNormForCompas(2.0*M_PI*(i+1)/nA1, 2.*  k  /nA2*M_PI);
			QVector3D n6 = makePtNormForCompas(2.0*M_PI*(i+1)/nA1, 2.*(k+1)/nA2*M_PI);

			this->position.push_back(p1);	this->normal.push_back(n1);
			this->position.push_back(p6);	this->normal.push_back(n6);
			this->position.push_back(p2);	this->normal.push_back(n2);
			this->position.push_back(p1);	this->normal.push_back(n1);
			this->position.push_back(p5);	this->normal.push_back(n5);
			this->position.push_back(p6);	this->normal.push_back(n6);

			this->textCoord.push_back(colorCircle[0]);
			this->textCoord.push_back(colorCircle[1]);
			this->textCoord.push_back(colorCircle[2]);
			this->textCoord.push_back(colorCircle[0]);
			this->textCoord.push_back(colorCircle[1]);
			this->textCoord.push_back(colorCircle[2]);
		}
	}
}

Compass *CompassFactory::getCompass() const
{
	return new Compass(this->position, this->normal, this->textCoord, this->textPath);
}

