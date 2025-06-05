/* Projeto University Collider 2.0, EEMEPP, UFPR - Autor: Jo�o Andr� Agustinho da Silva */
#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>
//#include <cppconn/driver.h>
//#include <cppconn/statement.h>
//#include <cppconn/connection.h>
#include "UniversityCollider_2_0.h"

#define MagicaGrande_PATH "MagicaGrande.json"
#define LOG_TYPE Debug // -> [0] para Debug e demais, [1] para Warning e Error, [2] somente pra Error, [>2] para desativar log
using namespace std;

void log(LogType logtype, string message) {
    if (logtype >= LOG_TYPE)
        cout << "[" << LogTypeString[logtype] << "] " << message << endl;
}

int main()
{
    SetDataSize();
    const unsigned short MedidoresInfo_size = GetJsonLength(MagicaGrande_PATH);
    Medidor* medidoresInfo = new Medidor[MedidoresInfo_size];
    GetInfoMedidores(medidoresInfo, MagicaGrande_PATH);

    const chrono::seconds CycleInterval_s(CycleInterval);
    chrono::duration<double> cycleInterval_offset = 0s;

    while (true)
    {
        const auto StartTime = chrono::high_resolution_clock::now();
        unsigned CurrentTimestamp = time(NULL);
        vector<thread> threads;
        
        for (int i = 0; i < MedidoresInfo_size; ++i)
            threads.emplace_back([&medidor = medidoresInfo[i], &ts = CurrentTimestamp]() {
                medidor.Update(&ts);
            });

        for (thread& t : threads)
            if (t.joinable())
                t.join();

        chrono::duration<double> duration = chrono::high_resolution_clock::now() - StartTime;
        log(Debug, ("Tempo da Multi-Thread: - " + to_string(duration.count())));

        const auto NextWakeUpTime = StartTime + CycleInterval_s - cycleInterval_offset;
        this_thread::sleep_until(NextWakeUpTime);

        cycleInterval_offset = chrono::high_resolution_clock::now() - StartTime - CycleInterval_s;
    }
    return 0;
}
