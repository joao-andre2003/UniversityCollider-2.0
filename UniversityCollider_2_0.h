/* Projeto University Collider 2.0, EEMEPP, UFPR - Autor: João André Agustinho da Silva */
#pragma once

#ifndef UNIVERSITYCOLLIDER_2_0
#define UNIVERSITYCOLLIDER_2_0
#define FeedsDB_PATH "C:\\Users\\joaoa\\OneDrive\\Área de Trabalho\\EEMEPP\\UniversityCollider_2_0-VS\\"
#define INTSIZE sizeof(int)
#define FLOATSIZE sizeof(float)
#define NextMetaInterval 604800 // -> 1 semana em segundos
#include <iostream>
#include<string>
#include <fstream>

struct FeedInfo {
    std::string feedId = "";
    std::string lastDatMetaIndex = "";
    unsigned int nextTimestamp = 0;
};
struct Medidor {
    std::string name = "";
    std::string ipAdress = "";
    FeedInfo* feedsInfo = 0x0;
    unsigned short* feedsLength = 0x0;
    bool isHarmonico = 0;
    bool isInactive = 0;
};

void SetDataSize();
unsigned short $Data_Size;
unsigned short $Data_Harmonico_Size;

unsigned GetJsonLength(std::string file);
void GetInfoMedidores(Medidor* medidoresInfo, std::string file);
bool ReadMultiMedidor(const char* Server_IP, bool* IsHarmonico, float*& data, unsigned short* Data_size);
std::string GetLastMetaDatIndex(const std::string* FeedId);
unsigned int GetLastTimestamp(const std::string* FeedId, const std::string* LastMetaNumber);
//unsigned int GetDatNPoints(const std::string* FeedId, const std::string* LastDatIndex);

std::string AddNewMeta(const std::string* FeedId, const std::string LastMetaIndex, unsigned* Timestamp);
void AddNewDat(const std::string* FeedId, const std::string* LastDatIndex);
void AddNPoint(const std::string* FeedId, const std::string* LastDatIndex, float Value);
#endif