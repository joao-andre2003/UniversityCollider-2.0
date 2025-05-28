/* Projeto University Collider 2.0, EEMEPP, UFPR - Autor: Jo�o Andr� Agustinho da Silva */
#include "dirent.h"
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

void GetInfoMedidores(Medidor* medidoresInfo, const string file)
{
    cout << "GetInfoMedidores" << endl;
    ifstream f(file);
    json data = json::parse(f);

    int cnt = 0;
    for (json::iterator element = data.begin(); element != data.end(); ++element)
    {
        json::value_type& _element_value = element.value();
        medidoresInfo[cnt].name = element.key();
        cout << "Medidor:" << cnt << endl;
        if ((medidoresInfo[cnt].name != "MultiMedidorPL") && (medidoresInfo[cnt].name != "MultiMedidorPK"))
            continue;
        cout << "PL ou PK" << endl;
        medidoresInfo[cnt].ipAdress = _element_value[0];
        medidoresInfo[cnt].isHarmonico = _element_value.size() > 2;

        const int _FeedsId_size = _element_value[1].size();
        medidoresInfo[cnt].feedsInfo = new Medidor::FeedInfo[_FeedsId_size];
        cout << "Medidor:" << medidoresInfo[cnt].name << " Size:" << _FeedsId_size << endl;
        for (int i = 0; i < _FeedsId_size; i++) {
            cout << i << " " << flush;
            medidoresInfo[cnt].feedsInfo[i].feedId = to_string(_element_value[1][i]);
            medidoresInfo[cnt].feedsInfo[i].lastDatMetaIndex = GetLastMetaDatIndex(&medidoresInfo[cnt].feedsInfo[i].feedId);
            medidoresInfo[cnt].feedsInfo[i].nextTimestamp = GetLastTimestamp(&medidoresInfo[cnt].feedsInfo[i].feedId, &medidoresInfo[cnt].feedsInfo[i].lastDatMetaIndex) + NextMetaInterval;
        }
        ++cnt;
    }
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

    if (GetFileSize(&readmeta) != 16) {
        cout << "ERRO - Timestamp: Arquivo de tamanho invalido. Sem apoio para valores em milisegundos";
        return 0;
    }

    unsigned lastTimestamp = 0;
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