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
    bool IsLeftPoint(Point pt, Line line);//判断是否在线的左侧
    bool IsRightPoint(Point pt, Line line);//判断是否在线的右侧
    float Cross2D(Point a,Point b);//叉乘
    QList<Point> hullpts;//凸包点集
    QList<Point> dispts;//保存离散点
    QList<Triangle> tins;//三角网列表


public:
    ConvexHull();//默认构造
    void generateHull(QVector<Point>& pts);//求解离散点集的凸包
    QList<Triangle>& GetInitTin(QList<Point>& pts);//利用分割半平面算法划分凸包，建立初始化三角网
    QList<Point>& getHull(){return hullpts;}//获取凸包
    QList<Triangle>& getTins();//获取三角网
    QList<Triangle>& getDelaunay(QList<Triangle>& hulltins,QList<Point>& pts);//驻点插入法进行三角剖分
    QList<Point>& getDispts() ;//获取离散点
    void clearHull();//清除凸包
    std::vector<QPoint> sort_points(std::vector<QPoint> points);//极角排序
};
