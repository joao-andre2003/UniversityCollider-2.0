/* Projeto University Collider 2.0, EEMEPP, UFPR - Autor: Jo�o Andr� Agustinho da Silva */
#include "UniversityCollider_2_0.h"
#include <cstring>

extern "C" {
#include "libmodbus/modbus.h"
}

#define PORT 502
#define RGSTR_ADDR 0
#define RGSTR_N 1
using namespace std;

const unsigned short RgtrsInfo[][2] = { // [0] -> register address (0 to 65535)
    {30003 - 30001, 74},                // [1] -> number of registers to read (1 to 125)
    {30201 - 30001, 16},
};
const unsigned short RgtrsInfo_TwoBytes[][2] = {
    {33001 - 30001, 12} // Esse registrador � diferente dos demais, seus dados s�o de 2 byte apenas, n�o precisando juntar com outros bytes (O c�digo antigo tratava desse jeito, mas n�o sei se est� certo, pois lan�a valores muito discrepantes dos demais)
};
const unsigned short RgtrsInfo_Harmonico[][2] = { // [0] -> register address (0 to 65535)
    {34001 - 30001, 80}, // U1                    // [1] -> number of registers to read (1 to 125)
    {34081 - 30001, 80}, // U2
    {34161 - 30001, 80}, // U3
    {34241 - 30001, 80}, // I1
    {34321 - 30001, 80}, // I2
    {34401 - 30001, 80}  // I3
};
const unsigned short RgtrsInfo_Size = sizeof(RgtrsInfo) / sizeof(RgtrsInfo[0]);
const unsigned short* RgtrsInfo_TwoBytes_Size = &RgtrsInfo_TwoBytes[0][RGSTR_N];
const unsigned short RgtrsInfo_Harmonico_Size = sizeof(RgtrsInfo_Harmonico) / sizeof(RgtrsInfo_Harmonico[0]);
unsigned short Data_Size;
unsigned short Data_Harmonico_Size;

void SetDataSize()
{
    unsigned short size = 0;
    for (short i = 0; i < RgtrsInfo_Size; ++i)
        size += RgtrsInfo[i][RGSTR_N];
    Data_Size = size / 2.0f + .5f + *RgtrsInfo_TwoBytes_Size;

    size = 0;
    for (short i = 0; i < RgtrsInfo_Harmonico_Size; ++i)
        size += RgtrsInfo_Harmonico[i][RGSTR_N];
    Data_Harmonico_Size = size / 2.0f + .5f + Data_Size;
}

static float GetFloatFromBytes(unsigned short& TwoBytes_a, unsigned short& TwoBytes_b)
{
    char FourBytesArray[4] = { // array de bytes
        TwoBytes_a >> 8,// 8 � a quantidade de bits em um char
        TwoBytes_a & 0xFF,
        TwoBytes_b >> 8,
        TwoBytes_b & 0xFF
    };
    float f = 0.0f;
    memcpy(&f, &FourBytesArray, FLOATSIZE); // a fun��o memcpy l� o "source" como um "big-endian" caso ele for maior que 1 byte, ent�o � melhor usar o m�todo do array de bytes (buffer)
    return f;
}

static void Read4BytesRegistors(modbus_t* ctx, const unsigned short RgtrsInfoList[][2], const unsigned short RgtrsInfo_SIZE, float* data, unsigned short* dataCnt)
{
    for (unsigned short i = 0; i < RgtrsInfo_SIZE; ++i)
    {
        unsigned short* _rgstrReader = new unsigned short[RgtrsInfoList[i][RGSTR_N]];
        modbus_read_input_registers(ctx, RgtrsInfoList[i][RGSTR_ADDR], RgtrsInfoList[i][RGSTR_N], _rgstrReader);

        for (int cnt = 0; cnt < RgtrsInfoList[i][RGSTR_N] - 1; cnt += 2) {
            data[*dataCnt] = GetFloatFromBytes(_rgstrReader[cnt], _rgstrReader[cnt + 1]);
            *dataCnt += 1;
        }
        delete[] _rgstrReader;
    }
}

static void Read2BytesRegistor(modbus_t* ctx, const unsigned short RgtrsInfoList[][2], float* data, unsigned short* dataCnt)
{
    unsigned short* _rgstrReader = new unsigned short[*RgtrsInfo_TwoBytes_Size];
    modbus_read_input_registers(ctx, RgtrsInfoList[0][RGSTR_ADDR], RgtrsInfoList[0][RGSTR_N], _rgstrReader);

    for (int i = 0; i < *RgtrsInfo_TwoBytes_Size; ++i) {
        data[*dataCnt] = _rgstrReader[i];
        *dataCnt += 1;
    }
    delete[] _rgstrReader;
}

bool ReadMultiMedidor(const char* Server_IP, bool* IsHarmonico, float*& data, unsigned short* data_size)
{
    modbus_t* ctx;
    ctx = modbus_new_tcp(Server_IP, PORT);

    if (modbus_connect(ctx) == -1) {
        log(Warning, "Conexão ModBus falhou: " + (string)(modbus_strerror(errno)));
        modbus_free(ctx);
        return 1;
    }

    *data_size = (*IsHarmonico) ? Data_Harmonico_Size : Data_Size;
    data = new float[*data_size];
    unsigned short dataCnt = 0;

    Read4BytesRegistors(ctx, RgtrsInfo, RgtrsInfo_Size, data, &dataCnt);
    Read2BytesRegistor(ctx, RgtrsInfo_TwoBytes, data, &dataCnt);
    if (*IsHarmonico) Read4BytesRegistors(ctx, RgtrsInfo_Harmonico, RgtrsInfo_Harmonico_Size, data, &dataCnt);

    modbus_close(ctx);
    modbus_free(ctx);
    return 0;
}
