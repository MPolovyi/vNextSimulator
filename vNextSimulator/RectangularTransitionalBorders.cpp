#include "stdafx.h"
#include "RectangularTransitionalBorders.h"

namespace Simulator
{
	CRectangularTransitionalBorders::CRectangularTransitionalBorders(double x, double y, double z)
	{
		m_X = x; m_Y = y; m_Z = 0;
	}

	void CRectangularTransitionalBorders::operator()(CParticle<2UL>& particle)
	{
		auto xCoord = particle.Coords[0];
		auto yCoord = particle.Coords[1];
		if (xCoord < 0)
			particle.Coords[0] += m_X;
		if (xCoord > m_X)
			particle.Coords[0] -= m_X;
		if (yCoord < 0)
			particle.Coords[1] += m_Y;
		if (yCoord > m_Y)
			particle.Coords[1] -= m_Y;
	}

	void CRectangularTransitionalBorders::operator()(CParticle<3UL>& particle)
	{
		auto xCoord = particle.Coords[0];
		auto yCoord = particle.Coords[1];
		auto zCoord = particle.Coords[2];
		if (xCoord < 0)
			particle.Coords[0] += m_X;
		if (xCoord > m_X)
			particle.Coords[0] -= m_X;
		if (yCoord < 0)
			particle.Coords[1] += m_Y;
		if (yCoord > m_Y)
			particle.Coords[1] -= m_Y;
		if (zCoord < 0)
			particle.Coords[2] += m_Z;
		if (zCoord > m_Z)
			particle.Coords[2] -= m_Z;
	}

	CRectangularTransitionalBorders::~CRectangularTransitionalBorders()
	{}
}