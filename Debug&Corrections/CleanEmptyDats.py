# Projeto EEMEPP, UFPR - Autor: João André Agustinho da Silva
# Código de correção para arquivos dat corrompidos/vazios
#
# DESCRIÇÃO: Debuga os últimos dados dats de todos os feed. Se chamar o código sem argumentos, imprime todos. Se chamar com o valor de um feed específico, imprime todos os dats desse feed

from os import rename, remove, listdir
import sys
import pickle

def RenameMetaDat(FeedPath, Index, NewIndex):
    rename(FeedPath+".dat."+str(Index), FeedPath+".dat."+str(NewIndex));
    rename(FeedPath+".meta."+str(Index), FeedPath+".meta."+str(NewIndex));

def DeleteMetaDat(FeedPath, Index):
    remove(FeedPath+".dat."+str(Index));
    remove(FeedPath+".meta."+str(Index));

def IsDatEmpty(Path_to_dat):
    with open(Path_to_dat, 'rb') as dat:
        datBytes = dat.read()
        if datBytes == b'':
            return True;
        return False;

def ReadDatFromFeed(FeedNumber):
    path = "/media/canopus/phpfinamultiple/%s/" % FeedNumber;
    lista_DIR = listdir(path);
    dat = [i for i in lista_DIR if 'dat' in i];

    newIndex = 0;
    for i in range(len(dat) - 1):
        _feedPath = path+FeedNumber;
        if IsDatEmpty(_feedPath+".dat."+str(i)):
            DeleteMetaDat(_feedPath, i);
            continue;

        newIndex += 1;
        if newIndex != i:
            RenameMetaDat(_feedPath, i, newIndex);

def main():
    customMedidorList = len(sys.argv) > 1;

    with open('../MagicaGrande.pickle', 'rb') as handle:
        Magica=pickle.load(handle)

    for j,name in enumerate(Magica):
        if customMedidorList and int(sys.argv[1]) not in Magica[name][1]:
            continue;
        _feednumber =  str(Magica[name][1][0]) if not customMedidorList else sys.argv[1];
        ReadDatFromFeed(_feednumber);

main();