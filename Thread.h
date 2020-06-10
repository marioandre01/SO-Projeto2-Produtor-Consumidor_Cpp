/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Thread.h
 * Author: marioandre
 *
 * Created on 31 de Março de 2017, 08:17
 */

#ifndef __thread_h
#define __thread_h

#include <pthread.h>
#include <string.h>
#include <stdlib.h>

class Thread
{
public:
    Thread(void * (*entry)(void *), void * arg, int arg_size) {
        _arg = malloc(arg_size); //Alocação de memoria. Recebe como argumento números inteiros positivos (arg_size), que irão representar o número de bytes que sera alocado. Essa função retorna um ponteiro contendo o endereço do bloco alocado.
        memcpy(_arg, arg, arg_size); //copia blocos de memória. Ela recebe 3 parâmetros: destino, origem e número de bytes a serem copiados.
        if(pthread_create(&_thread, 0, entry, _arg)) //if fail //se thread criado retorna 1 e entao _thread = 0, senao retorna 0 e nao faz nada
            _thread = 0;
    }
    ~Thread() { free(_arg); }

    int join(int * status) {
        void * res;
        if(pthread_join(_thread, &res)) return -1; //fail
        *status = *((int*)res);
        free(res);
        return 0;//success
    }
    friend void exit(int status = 0) {
        void * exit_status = malloc(sizeof(int));
        memcpy(exit_status, &status, sizeof(int));
        pthread_exit(exit_status);
    }
    friend void yield() { pthread_yield(); }

private:
    pthread_t _thread;
    void * _arg;
};

#endif