/* Projeto University Collider 2.0, EEMEPP, UFPR - Autor: Jo�o Andr� Agustinho da Silva */
#include <fstream>
#include <thread>
#include <vector>
#include <dirent.h>
#include "nlohmann/json.hpp"
#include "UniversityCollider_2_0.h"

using json = nlohmann::json;
using namespace std;

unsigned GetJsonLength(const string file)
{
    ifstream f(file);
    unsigned length = json::parse(f).size();
    f.close();
    return length;
}

void SetUpMedidor(Medidor* medidor, json::value_type& elementValue, string name)
{
    log(Debug, ("Lendo medidor do Magica Grande - " + name));
    medidor->name = name;
    medidor->ipAdress = elementValue[0];
    medidor->isHarmonico = elementValue.size() > 2;

    const int _FeedsId_size = elementValue[1].size();
    medidor->feedsInfo = new Medidor::FeedInfo[_FeedsId_size];
    for (int i = 0; i < _FeedsId_size; i++) {
        medidor->feedsInfo[i].feedId = to_string(elementValue[1][i]);
        medidor->feedsInfo[i].lastDatMetaIndex = GetLastMetaDatIndex(&medidor->feedsInfo[i].feedId);
        medidor->feedsInfo[i].nextTimestamp = GetLastTimestamp(&medidor->feedsInfo[i].feedId, &medidor->feedsInfo[i].lastDatMetaIndex) + NextMetaInterval;
    }
}

void GetInfoMedidores(Medidor* medidoresInfo, const string file)
{
    ifstream f(file);
    json data = json::parse(f);

    vector<thread> threads;
    int cnt = 0;
    for (json::iterator element = data.begin(); element != data.end(); ++element)
        threads.emplace_back(SetUpMedidor, &medidoresInfo[cnt++], element.value(), element.key());

    for (thread& t : threads)
        if (t.joinable())
            t.join();

    log(Debug, ("Leitura Magica Grande concluída."));
    f.close();
}

string GetLastMetaDatIndex(const string* FeedId)
{
    const string Path = FeedsDB_PATH + *FeedId;
    DIR* dir;
    dir = opendir(&Path[0]);

    struct dirent* entry;
    unsigned int lastIndex = 0;

    while ((entry = readdir(dir)) != NULL)
        ++lastIndex;

    closedir(dir);
    lastIndex = (lastIndex - 5) / 2;
    return to_string(lastIndex);
}

unsigned long GetFileSize(ifstream* file)
{
    unsigned long fileSize = 0;

    file->seekg(0, ios::beg);
    fileSize = file->tellg();
    file->seekg(0, ios::end);

    fileSize = (unsigned long)file->tellg() - fileSize;
    return fileSize;
}

unsigned int GetLastTimestamp(const string* FeedId, const string* LastMetaIndex)
{
    const string Path = FeedsDB_PATH + *FeedId + "/" + *FeedId + ".meta." + *LastMetaIndex;
    ifstream readmeta(Path, ifstream::binary);

    unsigned long long lastTimestamp = 0;
    readmeta.seekg(3 * INTSIZE, ifstream::beg);
    readmeta.read(reinterpret_cast<char*>(&lastTimestamp), INTSIZE);

    readmeta.close();
    return lastTimestamp;
}

unsigned int GetDatNPoints(const string* FeedId, const string* DatIndex)
{
    const string Path = FeedsDB_PATH + *FeedId + "/" + *FeedId + ".dat." + *DatIndex;
    ifstream readdat(Path, ifstream::binary);
    unsigned npoints = GetFileSize(&readdat) / FLOATSIZE;

    readdat.close();
    return npoints;
}
/*
bool GetMetaModo(const string* Path, unsigned long MetaFileSize, unsigned char* getTimestampSize, unsigned short* getTimestampUnit)
{
    *getTimestampSize = INTSIZE;
    *getTimestampUnit = 1;

    if (MetaFileSize == 25) {
        log(Warning, ("Timestamp em milissegundos - " + *Path));
        *getTimestampSize = LONGLONGSIZE;
        *getTimestampUnit = 1000;
    }
    else if (MetaFileSize != 16) {
        log(Error, ("Arquivo Meta de tamanho Inválido - " + *Path));
        return 1;
    }
    return 0;
}

void ReadDat(const string* FeedId, const string* DatIndex)
{
    const string Path = FeedsDB_PATH + *FeedId + "/" + *FeedId + ".dat." + *DatIndex;
    ifstream readmeta(Path, ifstream::binary);

    for (unsigned int i = 1; i < ( GetDatNPoints(FeedId, DatIndex) + 1 ); ++i)
    {
        float value = 0;
        readmeta.seekg(i * FLOATSIZE, ifstream::beg);
        readmeta.read(reinterpret_cast<char*>(&value), FLOATSIZE);
        cout << value << endl;
    }
}
*/
