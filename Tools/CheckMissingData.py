# Projeto EEMEPP, UFPR - Autor: João André Agustinho da Silva 

# Descrição: Código para ler os arquivos do diretório do SharePoint onde está todos os arquivos de dados de P0 e EApositivo extraídos da Canopus, verificar a data que está nos
# nomes dos arquivos e identificar quais datas estão faltando para completar os dados até o dia de hoje. Logo após a identificação, usa uma versão modificada do código do 
# discente Vitor Samuel Ruppel Ferreira, "data_extractor.ipynb", onde a extração de dados via API da Canopus é realizada e armazenada num arquivo CSV local, o qual é exportado 
# para o Teams em seguida.

import datetime
import numpy as np
import pandas as pd
import requests
from tqdm import tqdm
import json
import os
from office365.runtime.auth.user_credential import UserCredential
from office365.sharepoint.client_context import ClientContext
from getpass import getpass

SERVER = ["https://canopus.eletrica.ufpr.br", "API_KEY"];

def main():
    #region - - - - - - - - - Login Sharepoint e Configuração - - - - - - - - - 

    SP_username = "joao.andre@ufpr.br"; # Usuário do Sharepoint/Teams
    SP_password = ""; # Senha do Sharepoint/Teams - DEIXE VAZIO SE QUISER FAZER LOGIN MANUAL NA EXECUÇÃO DO CÓDIGO

    Export_file = True; # True se deseja a exportação do arquivo ao Sharepoint
    Delete_local_file = True; # True se quer deletar o CSV local após a exportação

    Sharepoint_url = "https://ufprbr0.sharepoint.com/sites/Projeto_CANOPUS2"; # URL com Sites incluso
    Export_path = f'Documentos%20Compartilhados/General/PowerBI/CanopusDB_EAP_P0%20-%20Power%20BI/'; # Caminho onde vai ser enviado no SP

    ctx = LoginSharepoint(Sharepoint_url, SP_username, SP_password);

    #region - - - - - - - - - Importar e organizar formato de data - - - - - - - - - 

    all_dates = [];
    all_dates = GetDates(ctx, Export_path);

    #region - - - - - - - - - Verificar se há datas de dados faltando e preencher - - - - - - - - - 

    i = -1;
    while i < len(all_dates) - 2:
        i += 1;
        _start_date = DatesFromFormat(all_dates[i])[1];
        _final_date = DatesFromFormat(all_dates[i+1])[0];

        _start_date_DT = datetime.datetime.strptime(_start_date, '%Y-%m-%d') + datetime.timedelta(1);
        _final_date_DT = datetime.datetime.strptime(_final_date, '%Y-%m-%d') - datetime.timedelta(1);

        if (_final_date_DT - _start_date_DT).days < 0:
            continue;
        
        _start_date = datetime.datetime.strftime(_start_date_DT, '%d-%m-%Y');
        _final_date = datetime.datetime.strftime(_final_date_DT, '%d-%m-%Y');
        print("inicio: "+_start_date+" final: "+_final_date);

        if _start_date_DT.year != _final_date_DT.year:
            _final_date_DT = datetime.datetime(_start_date_DT.year, 12, 31);
            all_dates = GetDates(ctx, Export_path);
            i -= 1;

        _file_name = f'CanopusDB_{_start_date if _start_date==_final_date else _start_date+"_"+_final_date}.csv'; # Nome do arquivo pra arquivo diários e em arquivos de datas maiores - Nome padronizado para o funcionamento desse código e do Power BI
        _local_path = f'./{_file_name}'; # Onde o arquivo vai ser salvo na máquina local + nome
        Export_path += str(_start_date_DT.year);
        ImportDataFromDate(_start_date, _final_date, _local_path);   

        if Export_file:
            ExportData(_local_path, Export_path, _file_name, ctx, Delete_local_file);
    
        all_dates.sort();
    return None;

def GetDates(ctx, Export_path):
    all_dates = [];
    for folder in ctx.web.get_folder_by_server_relative_url(Export_path).folders.get_all().execute_query():
        for file in ctx.web.get_folder_by_server_relative_url(Export_path + str(folder)).files.get_all().execute_query():
            _date = str(file);
            _date = _date[_date.find("_")+1 : _date.rfind(".")];

            _date_split = _date.split(sep="-");
            if len(_date_split) <= 3: # Reorganizar formato de data para YYYY-MM-DD
                _date = _date_split[2]+"-"+_date_split[1]+"-"+_date_split[0]; # Se for uma faixa diária
            else:
                _date = _date_split[2][:4]+"-"+_date_split[1]+"-"+_date_split[0]+"_"+_date_split[4]+"-"+_date_split[3]+"-"+_date_split[2][5:]; # Se for uma faixa de tempo maior YYYY-MM-DD_YYYY-MM-DD
            all_dates.append(_date);

    all_dates.sort() # O formato YYYY-MM-DD ajuda a organizar de maneira correta
    all_dates.append(datetime.datetime.today().strftime("%Y-%m-%d")); # Incluir o dia anterior (msm se o dia anterior já estiver incluso essa linha não resulta em erro)
    return all_dates;

def DatesFromFormat(date): # Verifica o formato do arquivo (data diária ou de período) e retorna o início e o fim da data
    date = str(date);
    start_final_date = [];
    if len(date) <= 10:
        start_final_date.append(date);
        start_final_date.append(date);
    else:
        start_final_date = date.split(sep="_");
    return start_final_date;

def Get_Feeds_IDs_Tag_Name(): # Extrai ids, medidores e grandeza da Canopus para consulta
    serverpath, apikey = SERVER;
    urlFeeds = serverpath + "/feed/list.json&userid=1&apikey=" + apikey;

    feeds = requests.get(urlFeeds);
    lista_feeds = np.array(feeds.json());

    lista_imt = [];
    lista_params = ["EApositivo","P0"]; # Grandezas que vão ser extraídas
    for item in lista_feeds:
        if item["name"] in lista_params and item["tag"] not in lista_imt:
            lista_imt.append((item["id"], item["name"], item["tag"]));
    return lista_imt;

def Get_Feed_Data_From_Server(feed_id, startUNIX, endUNIX, interval): # Extrai dados da Canopus
    serverUrl, apikeyRead = SERVER;
    url = serverUrl + "/feed/data.json?id=" + str(feed_id) + "&start=" + str(startUNIX) + "000&end=" + str(endUNIX) + "000&interval=" + str(interval) + "&apikey=" + apikeyRead;
    dataRecuperada = requests.get(url);
    json_data = None;

    try:
        json_data = dataRecuperada.json();
    except:
        print("ERRO PHP: "+str(dataRecuperada.text[: dataRecuperada.text.find("[")]));
        if json_data != None:
            json_data = json.loads(dataRecuperada.text[dataRecuperada.text.find("[") :]);
        else:
            return None;
    try:
        while json_data != [] and json_data[-1][0] > (endUNIX * 1000): # A importação da Canopus costuma vir com um dado após o tempo solicitado
            json_data.pop(); 
    except:
        print("ERRO META: "+str(dataRecuperada.text));
        return None;
        
    return json_data;

def ImportDataFromDate(Start_date, Final_date, Local_path):
    #region - - - - - - - - - Importar e criar arquivo CSV - - - - - - - - - 

    Intervalo = 600; # Intervalo entre dados em segundos
    IntervaloStep = Intervalo * 8900; # O valor limite de extração da API é 8928 pontos

    UnixTime_inicial = datetime.datetime.strptime(Start_date, '%d-%m-%Y').timestamp();
    UnixTime_final = (datetime.datetime.strptime(Final_date, '%d-%m-%Y') + datetime.timedelta(0, 59, 0, 0, 59, 23)).timestamp();

    gmt = (int)(datetime.datetime.now().hour - datetime.datetime.now(datetime.timezone.utc).hour); # Fuso horário
    UnixTime_inicial += gmt*3600;
    UnixTime_final += gmt*3600;

    df = pd.DataFrame(); # O arquivo que irá guardar os dados
    Lista_feed = Get_Feeds_IDs_Tag_Name();
    # FIM - Declarações

    print("Iniciando importação da Canopus . . .");
    for id, grandeza, medidor in tqdm(Lista_feed):
        _unixTime_inicial = UnixTime_inicial;

        while(_unixTime_inicial <= UnixTime_final): # Verifica o tamanho da faixa, se for muito grande extrai os dados por partes do tamanho de IntervaloStep
            _unixTime_final = _unixTime_inicial + IntervaloStep if _unixTime_inicial + IntervaloStep < UnixTime_final else UnixTime_final;

            dados_parcial = Get_Feed_Data_From_Server(id, int(_unixTime_inicial), int(_unixTime_final), Intervalo);
            try:
                df_parc  = pd.DataFrame(np.array(dados_parcial), columns = ['tempo','valor']);
                df_parc['grandeza'] = len(list(df_parc['tempo']))*[grandeza];
                df_parc['predio'] = len(list(df_parc['tempo']))*[medidor];
                df_parc['data_Brasilia'] = pd.to_datetime(df_parc['tempo'], unit='ms');
                df = pd.concat([df_parc, df]);
            except Exception as e:
                print(f'ERRO {e} ocorreu em {medidor}' if dados_parcial != [] else f'ERRO medidor {medidor} sem dados nessa data');

            _unixTime_inicial += IntervaloStep + Intervalo;

    df.to_csv(Local_path, index=False);
    return None;

def ExportData(Local_path, Export_path, File_name, client_ctx, Delete_local_file):
    #region - - - - - - - - - Exportar para o Sharepoint - - - - - - - - - 
    print("Iniciando exportação ao Sharepoint . . .");
    with open(Local_path, 'rb') as _content_file:
        _file_content = _content_file.read();

    client_ctx.web.get_folder_by_server_relative_url(Export_path).upload_file(File_name, _file_content).execute_query();
    print("Exportação Concluída.");   

    if Delete_local_file:
        os.remove(Local_path);
        print("Arquivo CSV local excluído.");
    return None;

def LoginSharepoint(Sharepoint_url, SP_username, SP_password):
    while True:
        try:
            _userCredentials = UserCredential(SP_username, SP_password);
            _ctx = ClientContext(Sharepoint_url).with_credentials(_userCredentials);
            _ctx.web.current_user.get().execute_query();
            return _ctx;
        except:
            SP_username = input("Email usuário Teams: ");
            SP_password = getpass("Senha Teams: ");

main();
