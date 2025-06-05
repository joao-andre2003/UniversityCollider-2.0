/* Projeto University Collider 2.0, EEMEPP, UFPR - Autor: Jo�o Andr� Agustinho da Silva */
#include <fstream>
#include "UniversityCollider_2_0.h"
using namespace std;

string AddNewMeta(const string* FeedId, const string LastMetaIndex, unsigned Timestamp)
{
    /*
    const string Base_Path = FeedsDB_PATH + *FeedId + "/" + *FeedId + ".meta.";
    const string NewIndex = to_string(stoi(LastMetaIndex) + 1);

    log(Debug, ("Criando novo Meta - " + Base_Path + NewIndex));
    ifstream fileSrc(Base_Path + LastMetaIndex, ios::binary | ios::in);
    fstream newMeta(Base_Path + NewIndex, ios::binary | ios::out);

    newMeta << fileSrc.rdbuf();
    newMeta.seekg(3 * INTSIZE, ifstream::beg);
    newMeta.write(reinterpret_cast<char*>(&Timestamp), INTSIZE);

    fileSrc.close();
    newMeta.close();
    return NewIndex;
    */
    return LastMetaIndex;
}

void AddNPoint(const string* FeedId, const string* LastDatIndex, float* Value, unsigned short Value_size)
{
    /*
    const string Path = FeedsDB_PATH + *FeedId + "/" + *FeedId + ".dat." + *LastDatIndex;

    //cout << "New NPoint: " << *Value << endl;
    fstream file(Path, ios::binary | ios::app);
    file.write(reinterpret_cast<char*>(&Value[0]), FLOATSIZE * Value_size);
    file.close();
    */
}

void AddNewDat(const string* FeedId, const string* LastDatIndex)
{
    /*
    const string Path = FeedsDB_PATH + *FeedId + "/" + *FeedId + ".dat." + *LastDatIndex;

    log(Debug, ("Criando novo Dat - " + Path));
    fstream file(Path, ios::binary | ios::out);
    file.close();
    */
}

void NewDatMeta(Medidor::FeedInfo* feedInfo, unsigned* Timestamp)
{
    /*
    if (GetLastTimestamp(&feedInfo->feedId, &feedInfo->lastDatMetaIndex) != (feedInfo->nextTimestamp - NextMetaInterval)) {
        log(Error, ("Meta Corrompido: " + feedInfo->feedId + ".meta." + feedInfo->lastDatMetaIndex));
        AddNewMeta(&feedInfo->feedId, feedInfo->lastDatMetaIndex, (feedInfo->nextTimestamp - NextMetaInterval));
    }
    else if (feedInfo->nextTimestamp == NextMetaInterval) {
        log(Error, ("Timestamp lido Incorretamente. Meta Pode Estar Errado - " + feedInfo->feedId + ".meta." + feedInfo->lastDatMetaIndex));
        return;
    }

    feedInfo->lastDatMetaIndex = AddNewMeta(&feedInfo->feedId, GetLastMetaDatIndex(&feedInfo->feedId), *Timestamp);
    feedInfo->nextTimestamp = *Timestamp + NextMetaInterval;
    AddNewDat(&feedInfo->feedId, &feedInfo->lastDatMetaIndex);
    */
}

void UpdateLast(const string* FeedId, float* Value, unsigned* Timestamp)
{
    /*
    const string Path = FeedsDB_PATH + *FeedId + "/" + *FeedId + ".last";

    fstream file(Path, ios::binary | ios::out);
    file.write(reinterpret_cast<char*>(&Value[0]), FLOATSIZE);
    file.seekg(FLOATSIZE, ifstream::beg);
    file.write(reinterpret_cast<char*>(&Timestamp[0]), INTSIZE);
    file.close();
    */
}
