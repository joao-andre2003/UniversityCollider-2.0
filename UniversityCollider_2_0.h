/* Projeto University Collider 2.0, EEMEPP, UFPR - Autor: Jo�o Andr� Agustinho da Silva */
#pragma once

#ifndef UNIVERSITYCOLLIDER_2_0
#define UNIVERSITYCOLLIDER_2_0

#include <string>
#define FeedsDB_PATH "/media/canopus4/phpfinamultiple/"
#define INTSIZE sizeof(int)
#define FLOATSIZE sizeof(float)
#define LONGLONGSIZE sizeof(unsigned long long)
#define NextMetaInterval 604800 // -> 1 semana em segundos
#define CycleInterval 60 // -> intervalo de amostragem em segundos
#define InactiveNPointLimit (3600 / CycleInterval) // -> 1 hora em segundos

class Medidor {
public:
	struct FeedInfo {
		std::string feedId = "";
		std::string lastDatMetaIndex = "";
		unsigned int nextTimestamp = 0;
	};
	std::string name = "";
	std::string ipAdress = "";
	FeedInfo* feedsInfo = nullptr;
	bool isHarmonico = 0;
	bool isInactive = 1;

	void Update(unsigned* CurrentTimestamp);
};

enum LogType {
	Debug,
	Warning,
	Error
};
const std::string LogTypeString[] = { 
	"DEBUG", 
	"\033[1;33mWARNING\033[0m",
	"\033[1;31mERROR\033[0m"
};

void SetDataSize();
void log(LogType logtype, std::string message);
unsigned GetJsonLength(std::string file);
void GetInfoMedidores(Medidor* medidoresInfo, std::string file);
bool ReadMultiMedidor(const char* Server_IP, bool* IsHarmonico, float*& data, unsigned short* Data_size);

std::string GetLastMetaDatIndex(const std::string* FeedId);
unsigned int GetLastTimestamp(const std::string* FeedId, const std::string* LastMetaNumber);
unsigned int GetDatNPoints(const std::string* FeedId, const std::string* LastDatIndex);
//void ReadDat(const std::string* FeedId, const std::string* DatIndex);

void NewDatMeta(Medidor::FeedInfo* feedInfo, unsigned* Timestamp);
void AddNPoint(const std::string* FeedId, const std::string* LastDatIndex, float* Value, unsigned short Value_size = 1);
void UpdateLast(const std::string* FeedId, float* Value, unsigned* Timestamp);
#endif
