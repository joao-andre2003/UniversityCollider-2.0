# Projeto EEMEPP, UFPR - Autor: João André Agustinho da Silva 
# Código de correção para arquivos dat e meta corrompidos/vazios
#
# DESCRIÇÃO: Exclui e reconstrui a database de um feed ou medidor especificado na chamada do programa ou de todos caso não especificado. Deleta períodos de dados vazios sem deixar buracos na continuidade nos índices dos arquivos.
# 
# Mode de Operação: Há 3 maneiras de executar o código
# Modo 0 -> Sem argumentos, EX: "sudo python3 CleanEmptyDats.py" | Isso irá fazer limpar todos os feeds de todos os medidores
# Modo 1 -> Um feed de argumento, EX: "sudo python3 CleanEmptyDats.py 7540" | Isso irá fazer limpar somente esse feed em específico
# Modo 0 -> Um medidor de argumento, EX: "sudo python3 CleanEmptyDats.py MultiMedidorPL" | Isso irá fazer limpar todos os feeds contidos apenas no medidor especificado

from os import rename, remove, listdir
import sys
import json

Base_Path = "/var/lib/phpfinamultiple"; # Caminho para a database dos feeds

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
    path = Base_Path + "/%s/" % FeedNumber;
    lista_DIR = listdir(path);
    dat = [i for i in lista_DIR if 'dat' in i];

    path += FeedNumber
    newIndex = 0;
    for i in range(len(dat)):
        if IsDatEmpty(path+".dat."+str(i)):
            DeleteMetaDat(path, i);
            continue;

        if newIndex != i:
            RenameMetaDat(path, i, newIndex);
        newIndex += 1;

def main():
    OperationMode = (len(sys.argv) > 1); # ==0 Alteração de todos os feeds | ==1 Alteração de um feed específico | ==2 Alteração de um grupo de feeds 
    if OperationMode: OperationMode += sys.argv[1].isnumeric() == False;
    print("OperationMode: "+str(OperationMode));

    with open('../MagicaGrande.json', 'rb') as f:
        Magica = json.load(f);

    for j,name in enumerate(Magica):
        if OperationMode==1 and int(sys.argv[1]) not in Magica[name][1]:
            continue;
        elif OperationMode==2 and str(sys.argv[1]) != name:
            continue;

        for j,feednumber in enumerate(Magica[name][1] if OperationMode!=1 else [sys.argv[1]]):
            ReadDatFromFeed(str(feednumber));
            print("Feed "+str(feednumber)+" limpado com sucesso de períodos de dados nulos.");
main();