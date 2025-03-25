/* Projeto University Collider 2.0, EEMEPP, UFPR - Autor: João André Agustinho da Silva */
#include <ctime>
#include "UniversityCollider_2_0.h"

#define MagicaGrande_PATH "MagicaGrande.json"
using namespace std;

void NewDatMeta(FeedInfo* feedInfo, const unsigned* Timestamp)
{
    feedInfo->lastDatMetaIndex = AddNewMeta(&feedInfo->feedId, GetLastMetaDatIndex(&feedInfo->feedId), *Timestamp);
    feedInfo->nextTimestamp = *Timestamp + NextMetaInterval;
    AddNewDat(&feedInfo->feedId, &feedInfo->lastDatMetaIndex);
}

int main()
{
    SetDataSize();

    const unsigned short MedidoresInfo_size = GetJsonLength(MagicaGrande_PATH);
    Medidor* medidoresInfo = new Medidor[MedidoresInfo_size];
    GetInfoMedidores(medidoresInfo, MagicaGrande_PATH);

    int teste = 0;

    while (true)
    {
        clock_t lastCycle_time = clock();
        for (int i = 0; i < MedidoresInfo_size; ++i)
        {
            if (medidoresInfo[i].name != "QDAC-FARMACO-NOVO-3PAV")
                continue;
            
            float* data = 0x0;
            unsigned short data_size = 0;
            unsigned CurrentTimestamp = time(NULL);

            if (ReadMultiMedidor(&medidoresInfo[i].ipAdress[0], &medidoresInfo[i].isHarmonico, data, &data_size) || teste++ == 0) {
                medidoresInfo[i].isInactive = true;
                cout << " - " << medidoresInfo[i].name << endl;
                continue;
            }

            if (medidoresInfo[i].isInactive) {
                medidoresInfo[i].isInactive = false;

                for (unsigned short j = 0; j < data_size; ++j)
                {
                    int npoints_missing = (CurrentTimestamp - medidoresInfo[i].feedsInfo[j].nextTimestamp + NextMetaInterval) / CycleInterval - GetDatNPoints(&medidoresInfo[i].feedsInfo[j].feedId, &medidoresInfo[i].feedsInfo[j].lastDatMetaIndex) - 1;
                    
                    if (npoints_missing > InactiveNPointLimit) {
                        NewDatMeta(&medidoresInfo[i].feedsInfo[j], &CurrentTimestamp);
                        continue;
                    }

                    float* missing_data = new float[npoints_missing];
                    for (unsigned short cnt = 0; cnt < npoints_missing; ++cnt)
                        missing_data[cnt] = NAN;

                    AddNPoint(&medidoresInfo[i].feedsInfo[j].feedId, &medidoresInfo[i].feedsInfo[j].lastDatMetaIndex, missing_data, npoints_missing);
                    delete[] missing_data;
                }
            }

            for (unsigned short j = 0; j < data_size; ++j)
            {
                if (medidoresInfo[i].feedsInfo[j].nextTimestamp < CurrentTimestamp)
                    NewDatMeta(&medidoresInfo[i].feedsInfo[j], &CurrentTimestamp);

                AddNPoint(&medidoresInfo[i].feedsInfo[j].feedId, &medidoresInfo[i].feedsInfo[j].lastDatMetaIndex, &data[j]);
            }

            delete[] data;
        }

        while (float(clock() - lastCycle_time) / CLOCKS_PER_SEC - CycleInterval < 0)
            continue;
    }
    return 0;
}