/* Projeto University Collider 2.0, EEMEPP, UFPR - Autor: Jo�o Andr� Agustinho da Silva */
#include <cmath>
#include "UniversityCollider_2_0.h"
using namespace std;

void Medidor::Update(unsigned CurrentTimestamp)
{
    if ((name != "MultiMedidorPL") && (name != "MultiMedidorPK"))
        return;

    cout << "Nova Iteracao - " << name << endl;

    float* data = nullptr;
    unsigned short data_size = 0;

    if (ReadMultiMedidor(&ipAdress[0], &isHarmonico, data, &data_size)) {
        isInactive = true;
        cout << " - " << name << endl;
        delete[] data;
        return;
    }

    if (isInactive) {
        isInactive = false;

        for (unsigned short j = 0; j < data_size; ++j)
        {
            int npoints_missing = (CurrentTimestamp - feedsInfo[j].nextTimestamp + NextMetaInterval) / CycleInterval - GetDatNPoints(&feedsInfo[j].feedId, &feedsInfo[j].lastDatMetaIndex);
            cout << "NPoints faltando: " << npoints_missing << endl;

            if (npoints_missing > InactiveNPointLimit) {
                NewDatMeta(&feedsInfo[j], &CurrentTimestamp);
                continue;
            }
            if (npoints_missing < 1) {
                delete[] data;
                return;
            }

            float* missing_data = new float[npoints_missing];
            for (unsigned short cnt = 0; cnt < npoints_missing; ++cnt)
                missing_data[cnt] = NAN;

            AddNPoint(&feedsInfo[j].feedId, &feedsInfo[j].lastDatMetaIndex, missing_data, npoints_missing);
            delete[] missing_data;
        }
    }

    for (unsigned short j = 0; j < data_size; ++j)
    {
        if (feedsInfo[j].nextTimestamp < CurrentTimestamp)
            NewDatMeta(&feedsInfo[j], &CurrentTimestamp);

        AddNPoint(&feedsInfo[j].feedId, &feedsInfo[j].lastDatMetaIndex, &data[j]);
        UpdateLast(&feedsInfo[j].feedId, &data[j], &CurrentTimestamp);
    }
    delete[] data;
}