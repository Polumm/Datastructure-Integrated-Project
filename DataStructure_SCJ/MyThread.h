#pragma once
#include<qthread.h>
#include"convexhull.h"
#include<qtime>
class MyThread : public QThread
{
    Q_OBJECT

public:
    explicit MyThread(ConvexHull* hull, QList<Triangle>* tr, QObject* parent = nullptr) : QThread(parent) { myHull = hull; DelaTins = tr; }//构造函数，通过指针共享内存
    void run()//线程执行函数
    {
        QTime timer;
        timer.start();
        ////可视化初始化三角网
        //*DelaTins = (*myHull).GetInitTin((*myHull).getHull());
        //(*DelaTins) = (*myHull).getDelaunay((*myHull).getTins(), (*myHull).getDispts());
        
        //建立初始三角网
        InitTin = (*myHull).GetInitTin((*myHull).getHull());
        //逐点插入构建精细三角网
        (*DelaTins) = (*myHull).getDelaunay((*myHull).getTins(), (*myHull).getDispts());
        int elapsedTime = timer.elapsed();
        float elapsedTimeInSeconds = elapsedTime / 1000.0f;
        emit resultReady(elapsedTimeInSeconds);
    }
    ConvexHull* myHull;//凸包指针
    QList<Triangle>* DelaTins;//三角网指针
    QList<Triangle> InitTin;//初始化三角网指针
signals:
    void resultReady(float);//算法执行完毕信号
public slots:
    void sleepAndWait();//算法休眠
    void killThread();//终止线程
};
