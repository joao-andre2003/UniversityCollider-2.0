# Projeto EEMEPP, UFPR - Autor: João André Agustinho da Silva 

# Descrição: < - - EM DESENVOLVIMENTO - - >

import json
from pyModbusTCP.client import ModbusClient
import mysql.connector
from mysql.connector import Error
from getpass import getpass

MagicaGrandePATH = "../MagicaGrande.json";

PORT = 502;
FEED_SIZES = [57, 297]; #Tamanhos fixos de feeds
FEED_MODEL = {};
FEED_MODEL_ENGINE = "11 " * max(FEED_SIZES);
FEED_MODEL_INTERVAL = "60 " * max(FEED_SIZES);
FEED_MODEL_OP_TYPE = "0 " * max(FEED_SIZES);
FEED_MODEL_PUBLIC = "0 " * max(FEED_SIZES);
FEED_MODEL_ENABLED = "1 " * max(FEED_SIZES);

def GetFeedModel(filePath):
    _FEED_MODEL = {}
    with open(filePath, 'r+', encoding='utf-8') as file:
        _content = file.read().replace(", \n", ', ""\n').split("\n");
        for i, element in enumerate(_content):
            print(element);
        _content2 = (print(n) for i,n in enumerate(_content))
        print(_content2)
    return {}

def main():
    FEED_MODEL = GetFeedModel("./Feed_Model.txt");
    return
    """
    print("< - - Inserir Novo Medidor ao UniversityCollider - - >")
    name = input("Nome do Medidor: ");
    
    ipAdress = "";
    while(True):
        ipAdress = input("Endereço IP do Medidor: ");
        _modbus = ModbusClient(host=ipAdress, port=PORT, unit_id=255)
        if not _modbus.open():
            print("ERRO: Conexão ModBus Recusada, IP pode estar errado.");
            continue;
        print("-> Conexão ModBus Aprovada. Medidor conectado.");
        _modbus.close();
        break;
    
    feed_list = [];
    isHarmonico = False;
    while(True):
        _feed_list_str = input("Feeds desse Medidor (Números separados por espaço ou vírgula e espaço. Use ... para completar sequencia de números. Ex: 1 ... 10 para valores entre 1 e 10):\n").split();
        for i in range(len(_feed_list_str)):
            _feed_list_str[i] = _feed_list_str[i].replace(",", "");
            if _feed_list_str[i] == "...":
                feed_list += (n for n in list(range(int(_feed_list_str[i-1])+1, int(_feed_list_str[i+1]))));
            elif _feed_list_str[i].isnumeric():
                feed_list.append(int(_feed_list_str[i]));

        if len(feed_list) not in FEED_SIZES:
            print("ERRO: Tamanho de Feed incorreto.");
            feed_list = [];
            continue;
        if len(feed_list) == max(FEED_SIZES):
            print("AVISO: O Medidor foi configurado como um harmônico.");
            isHarmonico = True;
        break;
    
    new_medidor = {name: [ipAdress, feed_list, isHarmonico]} if isHarmonico else {name: [ipAdress, feed_list]};

    updated_file = "";
    with open(MagicaGrandePATH,'r+', encoding='utf-8') as file:
        json.load(file)
        json.dump(new_medidor, file, indent=None);
    with open(MagicaGrandePATH, 'r', encoding='utf-8') as file:
        updated_file = file.read().replace('\n}{', ',\n').replace('}{', ',\n');
    with open(MagicaGrandePATH, 'w', encoding='utf-8') as file:
        file.write(updated_file);

    print("-> Arquivo "+MagicaGrandePATH+" Atualizado com Sucesso.");
    """
    with open("./Feed_Model.txt", 'w', encoding='utf-8') as f:
        f.write("name, unit");
        _aux = (" " * (FEED_SIZES[1] - FEED_SIZES[0]))
        fEED_MODEL_UNIT = "V V V V V V V A A A A A Hz Hz Hz Hz W W W W VAr VAr VAr VAr VA VA VA VA          KWh KVArh KWh KWh KW KW KVA KVA            " + _aux;
        fEED_MODEL_UNIT = fEED_MODEL_UNIT.split(" ");
        fEED_MODEL_NAME = "U0 U12 U23 U31 U1 U2 U3 I0 IN I1 I2 I3 F1 F2 F3 F1IEC P0 P1 P2 P3 Q0 Q1 Q2 Q3 S0 S1 S2 S3 FP0 FP1 FP2 FP3 FP0D FP1D FP2D FP3D FATORK EApositivo ERpositivo EAnegativo ERnegativo MDA DA MDS DS UANTHD UBNTHD UCNTHD IATHD IBTHD ICTHD U1THDagrup U2THDagrup U3THDagrup I1THDagrup I2THDagrup I3THDagrup U1AgrupH1 U1AgrupH2 U1AgrupH3 U1AgrupH4 U1AgrupH5 U1AgrupH6 U1AgrupH7 U1AgrupH8 U1AgrupH9 U1AgrupH10 U1AgrupH11 U1AgrupH12 U1AgrupH13 U1AgrupH14 U1AgrupH15 U1AgrupH16 U1AgrupH17 U1AgrupH18 U1AgrupH19 U1AgrupH20 U1AgrupH21 U1AgrupH22 U1AgrupH23 U1AgrupH24 U1AgrupH25 U1AgrupH26 U1AgrupH27 U1AgrupH28 U1AgrupH29 U1AgrupH30 U1AgrupH31 U1AgrupH32 U1AgrupH33 U1AgrupH34 U1AgrupH35 U1AgrupH36 U1AgrupH37 U1AgrupH38 U1AgrupH39 U1AgrupH40 U2AgrupH1 U2AgrupH2 U2AgrupH3 U2AgrupH4 U2AgrupH5 U2AgrupH6 U2AgrupH7 U2AgrupH8 U2AgrupH9 U2AgrupH10 U2AgrupH11 U2AgrupH12 U2AgrupH13 U2AgrupH14 U2AgrupH15 U2AgrupH16 U2AgrupH17 U2AgrupH18 U2AgrupH19 U2AgrupH20 U2AgrupH21 U2AgrupH22 U2AgrupH23 U2AgrupH24 U2AgrupH25 U2AgrupH26 U2AgrupH27 U2AgrupH28 U2AgrupH29 U2AgrupH30 U2AgrupH31 U2AgrupH32 U2AgrupH33 U2AgrupH34 U2AgrupH35 U2AgrupH36 U2AgrupH37 U2AgrupH38 U2AgrupH39 U2AgrupH40 U3AgrupH1 U3AgrupH2 U3AgrupH3 U3AgrupH4 U3AgrupH5 U3AgrupH6 U3AgrupH7 U3AgrupH8 U3AgrupH9 U3AgrupH10 U3AgrupH11 U3AgrupH12 U3AgrupH13 U3AgrupH14 U3AgrupH15 U3AgrupH16 U3AgrupH17 U3AgrupH18 U3AgrupH19 U3AgrupH20 U3AgrupH21 U3AgrupH22 U3AgrupH23 U3AgrupH24 U3AgrupH25 U3AgrupH26 U3AgrupH27 U3AgrupH28 U3AgrupH29 U3AgrupH30 U3AgrupH31 U3AgrupH32 U3AgrupH33 U3AgrupH34 U3AgrupH35 U3AgrupH36 U3AgrupH37 U3AgrupH38 U3AgrupH39 U3AgrupH40 I1AgrupH1 I1AgrupH2 I1AgrupH3 I1AgrupH4 I1AgrupH5 I1AgrupH6 I1AgrupH7 I1AgrupH8 I1AgrupH9 I1AgrupH10 I1AgrupH11 I1AgrupH12 I1AgrupH13 I1AgrupH14 I1AgrupH15 I1AgrupH16 I1AgrupH17 I1AgrupH18 I1AgrupH19 I1AgrupH20 I1AgrupH21 I1AgrupH22 I1AgrupH23 I1AgrupH24 I1AgrupH25 I1AgrupH26 I1AgrupH27 I1AgrupH28 I1AgrupH29 I1AgrupH30 I1AgrupH31 I1AgrupH32 I1AgrupH33 I1AgrupH34 I1AgrupH35 I1AgrupH36 I1AgrupH37 I1AgrupH38 I1AgrupH39 I1AgrupH40 I2AgrupH1 I2AgrupH2 I2AgrupH3 I2AgrupH4 I2AgrupH5 I2AgrupH6 I2AgrupH7 I2AgrupH8 I2AgrupH9 I2AgrupH10 I2AgrupH11 I2AgrupH12 I2AgrupH13 I2AgrupH14 I2AgrupH15 I2AgrupH16 I2AgrupH17 I2AgrupH18 I2AgrupH19 I2AgrupH20 I2AgrupH21 I2AgrupH22 I2AgrupH23 I2AgrupH24 I2AgrupH25 I2AgrupH26 I2AgrupH27 I2AgrupH28 I2AgrupH29 I2AgrupH30 I2AgrupH31 I2AgrupH32 I2AgrupH33 I2AgrupH34 I2AgrupH35 I2AgrupH36 I2AgrupH37 I2AgrupH38 I2AgrupH39 I2AgrupH40 I3AgrupH1 I3AgrupH2 I3AgrupH3 I3AgrupH4 I3AgrupH5 I3AgrupH6 I3AgrupH7 I3AgrupH8 I3AgrupH9 I3AgrupH10 I3AgrupH11 I3AgrupH12 I3AgrupH13 I3AgrupH14 I3AgrupH15 I3AgrupH16 I3AgrupH17 I3AgrupH18 I3AgrupH19 I3AgrupH20 I3AgrupH21 I3AgrupH22 I3AgrupH23 I3AgrupH24 I3AgrupH25 I3AgrupH26 I3AgrupH27 I3AgrupH28 I3AgrupH29 I3AgrupH30 I3AgrupH31 I3AgrupH32 I3AgrupH33 I3AgrupH34 I3AgrupH35 I3AgrupH36 I3AgrupH37 I3AgrupH38 I3AgrupH39 I3AgrupH40".split(" ");
        for n in range(297):
            f.write("\n"+fEED_MODEL_NAME[n]+", "+fEED_MODEL_UNIT[n])


    return;
    print("\n< - - Iniciando Atualização do SQL Emoncms - - >");

    db = None;
    while(True):
        _host = input("Host SQL (=localhost se deixado vazio): ");
        _host = "localhost" if _host=="" else _host;
        _database = input("Database SQL (=emoncms se deixado vazio): ");
        _database = "localhost" if _database=="" else _database;
        _user = input("Usuário SQL: ");
        _password = getpass("Senha SQL: ");

        try:
            db = mysql.connector.connect(host=_host, user=_user, password=_password, database=_database);
            if not db.is_connected():
                print("ERRO: Conexão SQL não estabelecida. Cheque os dados novamente.");
                continue;
        except Error as e:
            print(f"ERRO: {e}")
            continue;
        break;
    print("-> Conexão SQL Estabelecida.")
    cursor = db.cursor()



    db.commit();
    db.close();
main();
