#pragma once
#include<qthread.h>
#include"convexhull.h"
#include<qtime>
class MyThread : public QThread
{
    Q_OBJECT

public:
    explicit MyThread(ConvexHull* hull, QList<Triangle>* tr, QObject* parent = nullptr) : QThread(parent) { myHull = hull; DelaTins = tr; }//���캯����ͨ��ָ�빲���ڴ�
    void run()//�߳�ִ�к���
    {
        QTime timer;
        timer.start();
        ////���ӻ���ʼ��������
        //*DelaTins = (*myHull).GetInitTin((*myHull).getHull());
        //(*DelaTins) = (*myHull).getDelaunay((*myHull).getTins(), (*myHull).getDispts());
        
        //������ʼ������
        InitTin = (*myHull).GetInitTin((*myHull).getHull());
        //�����빹����ϸ������
        (*DelaTins) = (*myHull).getDelaunay((*myHull).getTins(), (*myHull).getDispts());
        int elapsedTime = timer.elapsed();
        float elapsedTimeInSeconds = elapsedTime / 1000.0f;
        emit resultReady(elapsedTimeInSeconds);
    }
    ConvexHull* myHull;//͹��ָ��
    QList<Triangle>* DelaTins;//������ָ��
    QList<Triangle> InitTin;//��ʼ��������ָ��
signals:
    void resultReady(float);//�㷨ִ������ź�
public slots:
    void sleepAndWait();//�㷨����
    void killThread();//��ֹ�߳�
};
