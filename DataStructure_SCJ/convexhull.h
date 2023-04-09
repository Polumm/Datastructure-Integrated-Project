#pragma once
#include <QVector>
#include <QVector3D>
#include <QList>
#include "sort.h"
#include <QDebug>
extern bool cmpXandY(Point p1,Point p2);
extern bool cmpXsubY(Point p1,Point p2);
class ConvexHull
{
    bool IsLeftPoint(Point pt, Line line);//�ж��Ƿ����ߵ����
    bool IsRightPoint(Point pt, Line line);//�ж��Ƿ����ߵ��Ҳ�
    float Cross2D(Point a,Point b);//���
    QList<Point> hullpts;//͹���㼯
    QList<Point> dispts;//������ɢ��
    QList<Triangle> tins;//�������б�


public:
    ConvexHull();//Ĭ�Ϲ���
    void generateHull(QVector<Point>& pts);//�����ɢ�㼯��͹��
    QList<Triangle>& GetInitTin(QList<Point>& pts);//���÷ָ��ƽ���㷨����͹����������ʼ��������
    QList<Point>& getHull(){return hullpts;}//��ȡ͹��
    QList<Triangle>& getTins();//��ȡ������
    QList<Triangle>& getDelaunay(QList<Triangle>& hulltins,QList<Point>& pts);//פ����뷨���������ʷ�
    QList<Point>& getDispts() ;//��ȡ��ɢ��
    void clearHull();//���͹��
    std::vector<QPoint> sort_points(std::vector<QPoint> points);//��������
};
