/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Semaphore.h
 * Author: marioandre
 *
 * Created on 31 de Março de 2017, 08:53
 */

#ifndef __semaphore_h
#define __semaphore_h
 
#include <semaphore.h>
 
class Semaphore
{
public:
    Semaphore(int i = 1) { sem_init(&sem, 0, i); }
    ~Semaphore() { sem_destroy(&sem); }
 
    void p() { sem_wait(&sem); }
    void v() { sem_post(&sem); }
 
    operator int()
    {
        int ret;
        sem_getvalue(&sem, &ret);
        return ret;
    }
 
private:
    sem_t sem;
};
 
#endif