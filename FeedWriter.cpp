/* Projeto University Collider 2.0, EEMEPP, UFPR - Autor: João André Agustinho da Silva */
#include "UniversityCollider_2_0.h"
using namespace std;

string AddNewMeta(const string* FeedId, const string LastMetaIndex, unsigned Timestamp)
{
    const string Base_Path = FeedsDB_PATH + *FeedId + "\\" + *FeedId + ".meta.";
    const string NewIndex = to_string(stoi(LastMetaIndex) + 1);

    ifstream fileSrc(Base_Path + LastMetaIndex, ios::binary | ios::in);
    fstream newMeta(Base_Path + NewIndex, ios::binary | ios::out);

    cout << "NEW META" << endl;

    newMeta << fileSrc.rdbuf();

    newMeta.seekg(3 * INTSIZE, ifstream::beg);
    newMeta.write(reinterpret_cast<char*>(&Timestamp), INTSIZE);
    newMeta.close();
    return NewIndex;
}

void AddNPoint(const string* FeedId, const string* LastDatIndex, float* Value, unsigned short Value_size)
{
    const string Path = FeedsDB_PATH + *FeedId + "\\" + *FeedId + ".dat." + *LastDatIndex;

    fstream file(Path, ios::binary | ios::app);
    file.write(reinterpret_cast<char*>(&Value[0]), FLOATSIZE * Value_size);
    file.close();
}

void AddNewDat(const string* FeedId, const string* LastDatIndex)
{
    const string Path = FeedsDB_PATH + *FeedId + "\\" + *FeedId + ".dat." + *LastDatIndex;

    fstream file(Path, ios::binary | ios::out);
    file.close();
}