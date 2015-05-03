#pragma once
#include "Simulator.h"

namespace Simulator
{
	template<size_t spDim>
	class CMaxStepsStabilityChecker
	{
	public:
		CMaxStepsStabilityChecker(int maxSteps) { m_MaxSteps = maxSteps; };
		virtual bool operator()(std::vector<CParticle<spDim>>& particles)
		{
			if (++m_CycleSteps > m_MaxSteps)
			{
				m_CycleSteps = 0;
				return true;
			}
			else
				return false;
		}
		virtual ~CMaxStepsStabilityChecker() {};
	protected:
		int m_CycleSteps = 0;
		int m_MaxSteps;
	};
}