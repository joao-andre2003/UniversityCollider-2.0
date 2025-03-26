# Projeto EEMEPP, UFPR - Autor: João André Agustinho da Silva
# Código de debug de metadados, reutilizando e modificando os códigos de Yuri Poledna, da aplicação UniversityCollider
#
# DESCRIÇÃO: Debuga os últimos dados dats de todos os feed. Se chamar o código sem argumentos, imprime todos. Se chamar com o valor de um feed específico, imprime todos os dats desse feed

from struct import calcsize, unpack_from
from os import stat, system, listdir
import sys
import pickle

INT_SIZE = calcsize('i')
CHAR_SIZE = calcsize('c')
DOUBLE_SIZE = calcsize('Q')
FLOAT_SIZE = calcsize('f')

def ReadDat(Path_to_dat):

    with open(Path_to_dat, 'rb') as dat:
        datBytes = dat.read()
        print("Dat bytes = "+str(datBytes));
        if datBytes == b'':
            print("ERRO: Dat dado vazio em "+Path_to_dat);
            return;

def ReadDatFromFeed(FeedNumber, ReadAllMetas):
    path = "/media/canopus/phpfinamultiple/%s/" % FeedNumber;
    lista_DIR = listdir(path);
    dat = [i for i in lista_DIR if 'dat' in i];

    last_dat = len(dat) - 1;
    print("O feed "+FeedNumber+" tem "+str(last_dat+1)+" dats.");

    if ReadAllMetas:
        print("Lendo todos os dats de "+FeedNumber);
        for i in range(last_dat):
            _path = path+FeedNumber+".dat."+str(i);
            print("Lendo "+_path);
            ReadDat(_path);
    else:
        print("Lendo o ultimo meta: ");
        path = path+FeedNumber+".dat."+str(last_dat);
        ReadDat(path);

def main():
    customMedidorList = len(sys.argv) > 1;

    with open('../MagicaGrande.pickle', 'rb') as handle:
        Magica=pickle.load(handle)

    for j,name in enumerate(Magica):
        if customMedidorList and int(sys.argv[1]) not in Magica[name][1]:
            continue;
        _feednumber =  str(Magica[name][1][0]) if not customMedidorList else sys.argv[1];
        print(_feednumber);
        ReadDatFromFeed(_feednumber, customMedidorList);

main();