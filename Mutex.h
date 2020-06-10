/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mutex.h
 * Author: marioandre
 *
 * Created on 12 de Maio de 2017, 00:14
 */

#ifndef __mutex_h
#define __mutex_h
 
#include <pthread.h>
 
//mutex é uma abreviação de exclusão mútua (do inglês mutual exclusion)

class Mutex
{
public:
    Mutex() {}
    ~Mutex() {pthread_mutex_destroy(&mut);} // Destrói o mutex
 
    
    int init() { pthread_mutex_init(&mut, NULL); } //Inicia mutex
    void lock() { pthread_mutex_lock(&mut); } // Trava o mutex
    bool try_lock() { return (pthread_mutex_trylock(&mut) == 0); } // true when succeeds. Tenta acessar um mutex (retorna valor indicando sucesso ou falha no lock)
    void unlock() { pthread_mutex_unlock(&mut); } // Libera o mutex
 
private:
    pthread_mutex_t mut;
};
 
#endif
