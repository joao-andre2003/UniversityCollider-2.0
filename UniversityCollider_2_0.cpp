/* Projeto University Collider 2.0, EEMEPP, UFPR - Autor: João André Agustinho da Silva */
#include <ctime>
#include "UniversityCollider_2_0.h"

#define MagicaGrande_PATH "MagicaGrande.json"
using namespace std;

void NewDatMeta(FeedInfo* feedInfo, const unsigned* Timestamp)
{
    feedInfo->lastDatMetaIndex = AddNewMeta(&feedInfo->feedId, GetLastMetaDatIndex(&feedInfo->feedId), Timestamp);
    AddNewDat(&feedInfo->feedId, &feedInfo->lastDatMetaIndex);
}

int main()
{
    setlocale(LC_ALL, "portuguese");

    SetDataSize();
    const int MedidoresInfo_size = GetJsonLength(MagicaGrande_PATH);
    Medidor* medidoresInfo = new Medidor[MedidoresInfo_size];
    GetInfoMedidores(medidoresInfo, MagicaGrande_PATH);

    const unsigned CurrentTimestamp = time(NULL);
    cout << "Timestamp atual é: " << CurrentTimestamp << endl;

    for (int i = 0; i < MedidoresInfo_size; ++i)
    {
        //const clock_t begin_time = clock();
        // do something
        //cout << float(clock() - begin_time) / CLOCKS_PER_SEC;
        return 0;
        cout << medidoresInfo[i].name << endl;

        float* data = 0x0;
        unsigned short data_size = 0;
        if (ReadMultiMedidor(&medidoresInfo[i].ipAdress[0], &medidoresInfo[i].isHarmonico, data, &data_size)) {
            medidoresInfo[i].isInactive = true;
            cout << " - " << medidoresInfo[i].name << endl;
            continue;
        }

        if (medidoresInfo[i].isInactive) {
            medidoresInfo[i].isInactive = false;

            for (unsigned short j = 0; j < data_size; ++j)
            {
                if ((medidoresInfo[i].feedsInfo[j].nextTimestamp - NextMetaInterval) < InactiveTimeLimit)
                    NewDatMeta(&medidoresInfo[i].feedsInfo[j], &CurrentTimestamp);

            }
        }

        for (unsigned short j = 0; j < data_size; ++j)
        {
            cout << medidoresInfo[i].feedsInfo[j].feedId << " -> " << data[j] << endl;
            
            if (medidoresInfo[i].feedsInfo[j].nextTimestamp < CurrentTimestamp)
                NewDatMeta(&medidoresInfo[i].feedsInfo[j], &CurrentTimestamp);

            AddNPoint(&medidoresInfo[i].feedsInfo[j].feedId, &medidoresInfo[i].feedsInfo[j].lastDatMetaIndex, data[j]);
        }

        delete[] data;
    }
    return 0;
}