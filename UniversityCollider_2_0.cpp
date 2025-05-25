/* Projeto University Collider 2.0, EEMEPP, UFPR - Autor: Jo�o Andr� Agustinho da Silva */
#include <ctime>
#include <chrono>
#include <thread>
#include <cmath>
#include <vector>
//#include <cppconn/driver.h>
//#include <cppconn/statement.h>
//#include <cppconn/connection.h>
#include "UniversityCollider_2_0.h"

#define MagicaGrande_PATH "MagicaGrande.json"
using namespace std;

void teste() {}

int main()
{
    SetDataSize();
    const unsigned short MedidoresInfo_size = GetJsonLength(MagicaGrande_PATH);
    Medidor* medidoresInfo = new Medidor[MedidoresInfo_size];
    GetInfoMedidores(medidoresInfo, MagicaGrande_PATH);

    const chrono::milliseconds CycleInterval_s(CycleInterval);
    chrono::duration<double> cycleInterval_offset = 0s;

    while (true)
    {
        const auto StartTime = chrono::high_resolution_clock::now();
        unsigned CurrentTimestamp = time(NULL);
        vector<thread> threads;
        
        for (int i = 0; i < MedidoresInfo_size; ++i)
            threads.emplace_back([&medidor = medidoresInfo[i], ts = CurrentTimestamp]() {
                medidor.Update(ts);
            });

        for (thread& t : threads)
            if (t.joinable())
                t.join();

        const auto NextWakeUpTime = StartTime + CycleInterval_s - cycleInterval_offset;
        this_thread::sleep_until(NextWakeUpTime);

        cycleInterval_offset = chrono::high_resolution_clock::now() - StartTime - CycleInterval_s;
    }
    return 0;
}