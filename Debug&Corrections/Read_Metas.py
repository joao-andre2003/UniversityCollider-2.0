# Projeto EEMEPP, UFPR - Autor: João André Agustinho da Silva
# Código de debug de metadados, reutilizando e modificando os códigos de Yuri Poledna, da aplicação UniversityCollider
#
# DESCRIÇÃO: Debuga os últimos dados metas de todos os feed. Se chamar o código sem argumentos, imprime todos. Se chamar com o valor de um feed específico, imprime todos os metas desse feed 

from struct import calcsize, unpack_from
from os import stat, system, listdir
import sys
import pickle

INT_SIZE = calcsize('i')
CHAR_SIZE = calcsize('c')
DOUBLE_SIZE = calcsize('Q')
FLOAT_SIZE = calcsize('f')

def ReadMeta(Path_to_meta):

    with open(Path_to_meta, 'rb') as meta:
        metaBytes = meta.read()
        print("Meta bytes = "+str(metaBytes));
        if metaBytes == b'':
            print("ERRO: Meta dado vazio em "+Path_to_meta);
            return;
        
        if(len(metaBytes) == 25):

            Modo = unpack_from('c', metaBytes, offset=INT_SIZE * 3 + DOUBLE_SIZE)[0]  # depois do starttime
            Modo = int.from_bytes(Modo, byteorder='big')
            print("Modo = "+str(Modo));
            if(Modo == 1):
                interval = unpack_from('f', metaBytes, offset=FLOAT_SIZE * 2)
                StartTime = unpack_from('Q', metaBytes, offset=INT_SIZE * 3)
                if(StartTime[0] < 1500000000000):
                    raise Exception(
                        "StartTime Incorrect if using MiliSeconds! Check StartTime of the feed")
        elif(len(metaBytes) == 16):
            Modo = 0
            interval = unpack_from('I', metaBytes, offset=2 * INT_SIZE)
            StartTime = unpack_from('I', metaBytes, offset=3 * INT_SIZE)
    try:
        StartTime = StartTime[0]
        interval = interval[0]
        print("Start time = "+str(StartTime));
        print("Interval = "+str(interval));
    except UnboundLocalError:
        raise TypeError(Path_to_meta)
    
def ReadMetaFromFeed(FeedNumber, ReadAllMetas):
    path = "/media/canopus/phpfinamultiple/%s/" % FeedNumber;
    lista_DIR = listdir(path);
    meta = [i for i in lista_DIR if 'meta' in i];

    last_meta = len(meta) - 1;
    print("O feed "+FeedNumber+" tem "+str(last_meta+1)+" metas.");

    if ReadAllMetas:
        print("Lendo todos os metas de "+FeedNumber);
        for i in range(last_meta):
            _path = path+FeedNumber+".meta."+str(i);
            print("Lendo "+_path);
            ReadMeta(_path);
    else:    
        print("Lendo o ultimo meta: ");
        path = path+FeedNumber+".meta."+str(last_meta);
        ReadMeta(path);

def main():
    customMedidorList = len(sys.argv) > 1;

    with open('../MagicaGrande.pickle', 'rb') as handle:
        Magica=pickle.load(handle)

    for j,name in enumerate(Magica):
        if customMedidorList and int(sys.argv[1]) not in Magica[name][1]:
            continue;
        _feednumber =  str(Magica[name][1][0]) if not customMedidorList else sys.argv[1];
        print(_feednumber);
        ReadMetaFromFeed(_feednumber, customMedidorList);

main();