#pragma once
#include <thread>

#include <regex>
#include "boost\lexical_cast.hpp"
#include "SimulationManager.h"

#include "DataSnap.h"
#include "JSONSaver.h"

#include "Simulator.h"

#include "VicsecPPInterractor.h"
#include "UniformNoiseRotation.h"
#include "RectangularTransitionalBorders.h"

template<size_t spDim>
class CSimulationManager
{
	template<size_t spDim>
	struct SSimulatorParams
	{
		double minNoise; double maxNoise; double noiseStep; double stepsWithCurrentNoise;
		int particleCount; blaze::StaticVector<double, spDim> size; double particleVelocity;
		std::function<void(Simulator::CParticle<spDim>&, Simulator::CParticle<spDim>&)> ppInterract;
		std::function<void(Simulator::CParticle<spDim>&)> pbInterract;
		std::function<void(Simulator::CParticle<spDim>&, double)> noiseFunc;

		SSimulatorParams()
		{
			minNoise = 0; maxNoise = 360; noiseStep = 1;
			particleCount = 1024; size = 32;
			ppInterract = Simulator::CVicsecPPInterractor();
			pbInterract = Simulator::CRectangularTransitionalBorders(size[0], size[1]);
			noiseFunc = Simulator::CUniformNoiseRotation();
		}
	};
public:
	std::string saveFileName = "AllDataFile.txt";

	CSimulationManager() {};
	CSimulationManager(int argc, const char **argv)
	{
		if (argc > 1)
			CreateParams(PrepareParams(argc, argv));
		Init();
	};
	~CSimulationManager() {};

	void RunSimulations()
	{
		int notExit = 1;

		while (notExit)
		{
			notExit = 0;

			for (int i = 0; i < m_Simulators.size(); i++)
			{
				if (m_Simulators[i].GetNoise() >= m_SimulatorParams[i].minNoise &&
					m_Simulators[i].GetNoise() <= m_SimulatorParams[i].maxNoise)
				{
					notExit++;
					std::cout << " S." << i << " St." << m_Simulators[i].Steps <<
						" N" << m_SimulatorParams[i].minNoise << "<" << m_Simulators[i].GetNoise() << "<" << m_SimulatorParams[i].maxNoise <<
						" | ";
					if (m_Simulators[i].IsStable())
					{
						CJSONSaver::SaveAll(saveFileName, CDataSnap<spDim>(m_Simulators[i]));
						m_Simulators[i].ChangeNoise(m_SimulatorParams[i].noiseStep);
					}
					else
					{
						m_Simulators[i].Interract();
					}
				}
			}
			std::cout << std::endl << std::endl;
		}
	};
private:
	std::vector<SSimulatorParams<spDim>> m_SimulatorParams;
	std::vector<Simulator::CSimulator<spDim>> m_Simulators;

	std::vector<std::string> PrepareParams(int argc, const char **argv)
	{
		std::vector<std::string> ret;
		for (int i = 1; i < argc; i++)
		{
			std::string str(argv[i]);
			str += " ";
			if (str.find("--") != std::string::npos)
				ret.push_back(str);
			else
				ret[ret.size() - 1] += str;
		}
		return ret;
	};

	void CreateParams(std::vector<std::string> argv)
	{
		std::cout << "Reading parameters" << std::endl;
		int numOfSimulators = 1;

		std::string str(argv[0]);
		std::smatch m;
		if (str.find("--numOfSimulators") != std::string::npos)
		{
			std::regex_search(str, m, std::regex("--numOfSimulators=(\\d{1,})"));
			numOfSimulators = boost::lexical_cast<int>(m[1].str());
		}

		for (int i = 0; i < numOfSimulators; i++)
		{
			m_SimulatorParams.push_back(SSimulatorParams<spDim>());
		}

		for (int i = 1; i < argv.size(); i++)
		{
			std::string str(argv[i]);
			std::smatch m;

			if (std::regex_search(str, m, std::regex("--numOfParticles=(.*)")))
			{
				auto group = m[1].str();
				std::smatch subM;
				int i = 0;
				while (std::regex_search(group, subM, std::regex("(\\d{1,}) ")))
				{
					m_SimulatorParams[i++].particleCount = boost::lexical_cast<int>(subM[1].str());
					group = subM.suffix().str();
				}
				continue;
			}
			if (std::regex_search(str, m, std::regex("--maxNoise=(.*)")))
			{
				auto group = m[1].str();
				std::smatch subM;
				int i = 0;
				while (std::regex_search(group, subM, std::regex("(\\d{1,}[,.]?\\d{0,}) ")))
				{
					m_SimulatorParams[i++].maxNoise = boost::lexical_cast<double>(subM[1].str());
					group = subM.suffix().str();
				}
				continue;
			}
			if (std::regex_search(str, m, std::regex("--minNoise=(.*)")))
			{
				auto group = m[1].str();
				std::smatch subM;
				int i = 0;
				while (std::regex_search(group, subM, std::regex("(\\d{1,}[,.]?\\d{0,}) ")))
				{
					m_SimulatorParams[i++].minNoise = boost::lexical_cast<double>(subM[1].str());
					group = subM.suffix().str();
				}
				continue;
			}
			if (std::regex_search(str, m, std::regex("--noiseStep=(.*)")))
			{
				auto group = m[1].str();
				std::smatch subM;
				int i = 0;
				while (std::regex_search(group, subM, std::regex("(\\d{1,}[,.]?\\d{0,}) ")))
				{
					m_SimulatorParams[i++].noiseStep = boost::lexical_cast<double>(subM[1].str());
					group = subM.suffix().str();
				}
				continue;
			}
			if (std::regex_search(str, m, std::regex("--particleVelocity=(.*)")))
			{
				auto group = m[1].str();
				std::smatch subM;
				int i = 0;
				while (std::regex_search(group, subM, std::regex("(\\d{1,}[,.]?\\d{0,}) ")))
				{
					m_SimulatorParams[i++].particleVelocity = boost::lexical_cast<double>(subM[1].str());
					group = subM.suffix().str();
				}
				continue;
			}
			if (std::regex_search(str, m, std::regex("--sizeX=(.*)")))
			{
				auto group = m[1].str();
				std::smatch subM;
				int i = 0;
				while (std::regex_search(group, subM, std::regex("(\\d{1,}[,.]?\\d{0,}) ")))
				{
					m_SimulatorParams[i++].size[0] = boost::lexical_cast<double>(subM[1].str());
					group = subM.suffix().str();
				}
				continue;
			}
			if (std::regex_search(str, m, std::regex("--sizeY=(.*)")))
			{
				auto group = m[1].str();
				std::smatch subM;
				int i = 0;
				while (std::regex_search(group, subM, std::regex("(\\d{1,}[,.]?\\d{0,}) ")))
				{
					m_SimulatorParams[i++].size[1] = boost::lexical_cast<double>(subM[1].str());
					group = subM.suffix().str();
				}
				continue;
			}
			if (std::regex_search(str, m, std::regex("--sizeZ=(.*)")))
			{
				auto group = m[1].str();
				std::smatch subM;
				int i = 0;
				while (std::regex_search(group, subM, std::regex("(\\d{1,}[,.]?\\d{0,}) ")))
				{
					m_SimulatorParams[i++].size[2] = boost::lexical_cast<double>(subM[1].str());
					group = subM.suffix().str();
				}
				continue;
			}
		}
	};

	void Init()
	{
		std::cout << "Creating simulators" << std::endl;
		for (int i = 0; i < m_SimulatorParams.size(); i++)
		{
			m_Simulators.push_back(Simulator::CSimulator<spDim>(
				m_SimulatorParams[i].particleCount,
				m_SimulatorParams[i].size,
				m_SimulatorParams[i].ppInterract,
				m_SimulatorParams[i].pbInterract,
				m_SimulatorParams[i].noiseFunc,
				m_SimulatorParams[i].noiseStep > 0 ? m_SimulatorParams[i].minNoise : m_SimulatorParams[i].maxNoise
				));
		}
	};
};

