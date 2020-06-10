/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: marioandre
 *
 * Created on 18 de Maio de 2017, 22:03
 */

#include <iostream>
#include <fstream> //biblioteca específica para a manipulação de dados em arquivos
#include <uuid/uuid.h>
#include <unistd.h>
#include <errno.h>
 
#include "Thread.h"
#include "Semaphore.h"
#include "Mutex.h"
 
using namespace std;

Semaphore s1(1);
Semaphore s2(0);

Semaphore s3(1);
Semaphore s4(0);

const int PRODS = 100;
const int REP = 10; 
 
const int CONS = 20; 
 
const int BUF_SIZE = 35; 
 
Thread * prods[PRODS];
Thread * cons[CONS];
 
uuid_t buffer[BUF_SIZE];
static int prod_pos = 0;
static int cons_pos = 0;
 
bool finished = false;
 
void* producer(void * arg)
{
    s3.p();
    cout << "O produtor nasceu!" << endl;
    s3.v();
    s4.v();
    
    int rep = REP;
 
    char fname[36+1]; //vetor tipo char com 37 posições
        
    while(rep--) 
    {
        s1.p();
        if(++prod_pos == BUF_SIZE) prod_pos = 0; //incrementa prod_pos e compara com BUF_SIZE
            	
        uuid_generate(buffer[prod_pos]); //A função uuid_generate cria um novo identificador universalmente único (UUID)
        uuid_unparse(buffer[prod_pos], fname); //uuid_unparse(uuid_t uu, char *out)Converter um UUID de representação binária para uma string. //A função uuid_unparse(uuid_t uu, char *out) converte o UUID fornecido da representação binária em uma seqüência de 36 bytes (mais caracter '\ 0') do formulário 1b4e28ba-2fa1-11d2-883f-0016d3cca427 e armazena esse valor na seqüência de caracteres apontada por out 
		
        string name(fname,sizeof(uuid_t)*2 + 4); //name(fname,36)
                
        ofstream file(name.c_str()); //Cria um arquivo com o nome do conteudo da variavel name
        file << name; // escreve o conteudo de name no arquivo
        file.close(); //fecha o arquivo
        cout << "Produtor produziu: " << name << endl;
        s2.v();
               
    }
       
    exit(REP);
}

// a função sizeof() mostra quantos bytes certa variável ocupa em memoria
//sizeof(uuid_t)=16;
//construtor string (const char* s, size_t n) Copia os primeiros n caracteres da matriz de caracteres apontados por s.
//ofstream: objetos que escrevem dados em um arquivo. 
/*ofstream escreve;
escreve.open("strings.txt");
escreve << "Utilizamos os objetos ofstream para escrever em arquivos\n"; */
//c_str() - Retorna um ponteiro para uma matriz que contém uma seqüência de caracteres com terminação nula

void* consumer(void* arg)
{
    s4.p();
    cout << "O Consumidor nasceu!" << endl;
    s4.v();
 
    char fname[36+1];
    int consumed = 0;
 
    while(true)
    {
	s2.p(); 	
        if(finished) exit(consumed);
               
        consumed++;
                
        if(++cons_pos == BUF_SIZE) cons_pos = 0;
	uuid_unparse(buffer[cons_pos], fname);
                
	{           
            ifstream file(fname); //ifstream: objetos que leêm dados em um arquivo
			
            if(!file.good()) continue; //good():retorna verdadeiro se o arquivo pode ser usado. //Continue: ao executar essa linha pula a iteração atual e pula para a proxima iteração
            
            string str;
            file >> str;
           
            cout << "Consumidor consumiu" << "(" << consumed << "): " << str << endl;
                            
	}
        if(remove(fname)) cerr << "Error: " << errno << endl; //remove() remove o arquivo especificado em fname
        s1.v();
    }
    exit(consumed);
}

int main()
{
    cout << "## Produtor Massivo x Problema do Consumidor ##" << endl << endl;
 
    // Create
    for(int i = 0; i < PRODS; i++)
    	prods[i] = new Thread(&producer, 0, 0);
    for(int i = 0; i < CONS; i++)
    	cons[i] = new Thread(&consumer, 0, 0);
 
    // Join
    int status = 0;
    int produced = 0;
    int consumed = 0;
    for(int i = 0; i < PRODS; i++)
    {
    	prods[i]->join(&status);
    	produced += status;
    }
    
    finished = true;
    for(int j=0; j< CONS; j++){
        s2.v();
    }
    
    for(int i = 0; i < CONS; i++)
    {
    	cons[i]->join(&status);
    	consumed += status;
    }
    
    cout << "Total produzido: " << produced << endl;
    cout << "Total consumido: " << consumed << endl;
 
    return 0;
}
