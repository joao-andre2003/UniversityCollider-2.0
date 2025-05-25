/* Projeto University Collider 2.0, EEMEPP, UFPR - Autor: Jo�o Andr� Agustinho da Silva */
#pragma once

#ifndef UNIVERSITYCOLLIDER_2_0
#define UNIVERSITYCOLLIDER_2_0
#define _FeedsDB_PATH "C:\\Users\\joaoa\\OneDrive\\�rea de Trabalho\\EEMEPP\\UniversityCollider_2_0-VS\\"
#define __FeedsDB_PATH "C:\\Users\\DELT\\Desktop\\labgd\\UniversityCollider_2_0-VS\\"
#define FeedsDB_PATH "/var/lib/phpfinamultiple/"
#define INTSIZE sizeof(int)
#define FLOATSIZE sizeof(float)
#define NextMetaInterval 604800 // -> 1 semana em segundos
#define CycleInterval 60 // -> intervalo de amostragem em segundos
#define InactiveNPointLimit (3600 / CycleInterval) // -> 1 hora em segundos
#include <iostream>
#include <string>
#include <fstream>

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

	void Update(unsigned CurrentTimestamp);
};

void SetDataSize();
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