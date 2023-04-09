#include "MyThread.h"
void MyThread::sleepAndWait()
{
    this->msleep(600);
}

void MyThread::killThread()
{
    this->quit();
    this->wait();
    delete this;
}
