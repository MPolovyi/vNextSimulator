#pragma once
#include "rapidjson\prettywriter.h"
#include "DataSnap.h"
class CJSONSaver
{
public:
	template<size_t spDim>
	static void SaveAll(std::string fileName, CDataSnap<spDim>& data)
	{
		rapidjson::StringBuffer s;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> wr(s);

		std::cout << std::endl << "Saving simultion " << data.simGuid << " data" << std::endl;
		wr.StartObject();
		{
			std::cout << "Saving info" << std::endl;
			WriteMiscelaneousInfo(wr, data);
			std::cout << "Saving data" << std::endl;
			WriteData(wr, data);
		}
		wr.EndObject();
		std::cout << "Writing file" << std::endl;
		SaveInFile(fileName, s);
		std::cout << "Finished saving" << std::endl;
	};
private:
	CJSONSaver() {};
	~CJSONSaver() {};
	template<size_t spDim>
	static void WriteMiscelaneousInfo(rapidjson::PrettyWriter<rapidjson::StringBuffer>& wr, CDataSnap<spDim>& data)
	{
		wr.String("Miscelaneous");
		wr.StartObject();
		{
			wr.String("GUID");
			wr.String(data.simGuid.c_str());
			wr.String("ParticleCount");
			wr.Int(data.ParticleCount);
			wr.String("SimSize");
			wr.StartArray();
			{
				for (auto& e : data.Extents)
					wr.Double(e);
			}
			wr.EndArray();
			wr.String("ParticleVelocity");
			wr.Double(data.ParticleVelocity);
			wr.String("Noise");
			wr.Double(data.Noise);
			wr.String("SimulationSteps");
			wr.Int(data.stepsFromStart);
			wr.String("BorderConditions");
			wr.String(data.BorderConditions.c_str());
			wr.String("ParticleInterractions");
			wr.String(data.ParticleInterractions.c_str());
			wr.String("NoiseFunction");
			wr.String(data.NoiseFunction.c_str());
			wr.String("SystemStabilityCheck");
			wr.String(data.StabilityChecker.c_str());

			wr.String("PreviousSimulations");
			wr.StartArray();
			{
				for (auto& prevSim : data.PreviousSimulations)
				{
					wr.StartObject();
					wr.String("Noise");
					wr.Double(prevSim.first);
					wr.String("SimulationSteps");
					wr.Int(prevSim.second);
					wr.EndObject();
				}
			}
			wr.EndArray();
		}
		wr.EndObject();
	};

	template<size_t spDim>
	static void WriteData(rapidjson::PrettyWriter<rapidjson::StringBuffer>& wr, CDataSnap<spDim>& data)
	{
		wr.String("Data");
		wr.StartObject();
		{
			WriteParticleData(wr, data);
			WriteVelocityData(wr, data);
			WriteDencityData(wr, data);
		}
		wr.EndObject();
	};

	template<size_t spDim>
	static void WriteParticleData(rapidjson::PrettyWriter<rapidjson::StringBuffer>& wr, CDataSnap<spDim>& data)
	{
		wr.String("ParticleData");
		wr.StartArray();
		{
			for (auto& particle : data.ParticleData)
			{
				wr.StartObject();
				wr.String("Coordinate");
				wr.StartArray();
				for (auto d : particle.first)
					wr.Double(d);
				wr.EndArray();
				wr.String("Velocity");
				wr.StartArray();
				for (auto d : particle.second)
					wr.Double(d);
				wr.EndArray();
				wr.EndObject();
			}
		}
		wr.EndArray();
	};

	template<size_t spDim>
	static void WriteVelocityData(rapidjson::PrettyWriter<rapidjson::StringBuffer>& wr, CDataSnap<spDim>& data)
	{
		wr.String("AverageVelocityData");
		wr.StartArray();
		{
			for (auto& vData : data.AverageVelocityData)
			{
				wr.StartObject();
				wr.String("Coordinate");
				wr.StartArray();
				for (auto d : std::get<0>(vData))
					wr.Double(d);
				wr.EndArray();

				wr.String("Extents");
				wr.StartArray();
				for (auto d : std::get<1>(vData))
					wr.Double(d);
				wr.EndArray();

				wr.String("Velocity");
				wr.StartArray();
				for (auto d : std::get<2>(vData))
					wr.Double(std::get<3>(vData) > 0.0 ? d : 0.0);
				wr.EndArray();

				wr.String("LocalParticleCount");
				wr.Int(std::get<3>(vData));
				wr.EndObject();
			}
		}
		wr.EndArray();
	};

	template<size_t spDim>
	static void WriteDencityData(rapidjson::PrettyWriter<rapidjson::StringBuffer>& wr, CDataSnap<spDim>& data)
	{
		wr.String("AverageDencityData");
		wr.StartArray();
		{
			for (auto& vData : data.AverageDencityData)
			{
				wr.StartObject();
				wr.String("Coordinate");
				wr.StartArray();
				for (auto d : std::get<0>(vData))
					wr.Double(d);
				wr.EndArray();

				wr.String("Extents");
				wr.StartArray();
				for (auto d : std::get<1>(vData))
					wr.Double(d);
				wr.EndArray();

				wr.String("Dencity");
				wr.Double(std::get<2>(vData));

				wr.String("LocalParticleCount");
				wr.Int(std::get<3>(vData));
				wr.EndObject();
			}
		}
		wr.EndArray();
	};

	static void SaveInFile(std::string fileName, rapidjson::StringBuffer& buffer);
};

