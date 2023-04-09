#include "convexhull.h"
#include "convexhull.h"
bool cmpXandY(Point p1,Point p2){
    float xandy1 = p1.x+p1.y;
    float xandy2 = p2.x+p2.y;
    return xandy1<xandy2;
}
bool cmpXsubY(Point p1,Point p2){
    float xandy1 = p1.x-p1.y;
    float xandy2 = p2.x-p2.y;
    return xandy1<xandy2;
}
bool ConvexHull::IsLeftPoint(Point pt, Line line)
{
    Point v1(line.p2.x-line.p1.x,line.p2.y-line.p1.y);//p1p2
    Point v2(line.p1.x-pt.x,line.p1.y-pt.y);// pp1
    float tmp =Cross2D(v1,v2);
    if(tmp>=0) //����0���ұ� 0���ߵ��ӳ�����
    {
        return false;
    }else if( tmp <0)
    {
        return true;
    }
}
bool ConvexHull::IsRightPoint(Point pt, Line line)
{
    Point v1(line.p2.x-line.p1.x,line.p2.y-line.p1.y);//p1p2
    Point v2(line.p1.x-pt.x,line.p1.y-pt.y);// pp1
    float tmp =Cross2D(v1,v2);
    if(tmp>0) //����0���ұ�
    {
        return true;
    }else
    {
        return false;
    }
}

float ConvexHull::Cross2D(Point a, Point b)
{
    return a.x*b.y-b.x*a.y;
}

QList<Point>& ConvexHull::getDispts() 
{
    return dispts;
}

void ConvexHull::clearHull()
{
    hullpts.clear();//����͹����
    dispts.clear();//������ɢ��
    tins.clear();//����������
}

QList<Triangle>& ConvexHull::getTins()
{
    return tins;
}

QList<Triangle>& ConvexHull::getDelaunay(QList<Triangle>& hulltins, QList<Point>& pts)
{
    int onLine = 0;
    for (int i = 0; i < pts.size(); i++)
    {
        QList<Triangle> delTin; //����Ҫɾ����������
        for (int j = 0; j < hulltins.size(); j++)
        {
            if (hulltins[j].isInTriangle(pts[i]) == true)
                delTin.push_back(hulltins[j]);
            if (hulltins[j].isOnTriangle(pts[i]) == true) {
                delTin.push_back(hulltins[j]);
                onLine++;
            }
        }
        QList<Line> borderLines;//Ѱ����ɢ������ڱ�
        QList<Triangle> newTri;//Ѱ����ɢ������ڱ�
        if (delTin.size() == 1)
        {
            Line l1 = delTin[0].l1;
            Line l2 = delTin[0].l2;
            Line l3 = delTin[0].l3;
            borderLines.push_back(l1);
            borderLines.push_back(l2);
            borderLines.push_back(l3);

            for (int j = 0; j < borderLines.size(); j++)
                newTri.push_back(Triangle(borderLines[j].p1, borderLines[j].p2, pts[i]));
            hulltins.removeOne(delTin[0]);
        }
        else if (delTin.size() == 2)
        {
            Line l[3];
            l[0] = delTin[0].l1;
            l[1] = delTin[0].l2;
            l[2] = delTin[0].l3;
            int index = 0;
            for (int m = 0; m < 3; m++)
                if (delTin[1].containsLine(l[m]) == 0)
                    borderLines.push_back(l[m]);
                else
                    index = delTin[1].containsLine(l[m]) - 1;
            for (int m = 0; m < 3; m++)
                if (m != index)
                    borderLines.push_back(delTin[1].l[m]);

            for (int j = 0; j < borderLines.size(); j++)
                newTri.push_back(Triangle(borderLines[j].p1, borderLines[j].p2, pts[i]));
            hulltins.removeOne(delTin[0]);
            hulltins.removeOne(delTin[1]);
        }

        delTin.clear();
        for (int s = 0; s < newTri.size(); s++) {
            for (int j = 0; j < 3; j++) {
                Line line = newTri[s].l[j];
                for (int m = 0; m < hulltins.size(); m++)
                {
                    {
                        if (hulltins[m].containsLine(line))
                        {
                            Circle tinCircle = Circle::genTriCircle(hulltins[m]);
                            if (tinCircle.isInCircle(vec3(pts[i])))
                            {
                                delTin.push_back(newTri[s]);
                                int x = hulltins[m].containsLine(line) - 1;
                                for (int k = 0; k < 3; k++)
                                    if (x != k)
                                        newTri.push_back(Triangle(hulltins[m].l[k].p1, hulltins[m].l[k].p2, pts[i]));
                                hulltins.removeAt(m);
                            }
                        }
                    }
                }
            }

        }

        for (int m = 0; m < newTri.size(); m++)
            hulltins.push_back(newTri[m]);
        for (int m = 0; m < delTin.size(); m++)
            hulltins.removeOne(delTin[m]);

    }
    tins = hulltins;
    return tins;
}

ConvexHull::ConvexHull()
{

}

void ConvexHull::generateHull(QVector<Point>& pts)
{
    QList<Point> othpts;
    hullpts.clear();
    for(int i =0; i< pts.size(); i++)
    {
        othpts.push_back(pts[i]);
    }
    //��������ĵ㣺min(x-y)��min(x+y)��max(x-y)��max(x+y)��˳�η���һ�����飬��ɳ�ʼ͹�������ﹹ��һ��˳ʱ��͹��
    Sort::quickSort(pts,0,pts.size()-1,cmpXsubY);

    hullpts.push_back(pts[0]);
    hullpts.push_back(pts[pts.size()-1]);
    othpts.removeOne(pts[0]);
    othpts.removeOne(pts[pts.size()-1]);
    Sort::quickSort(pts,0,pts.size()-1,cmpXandY);
    if(!hullpts.contains(pts[0]))
    {
        hullpts.insert(1,pts[0]);
    }
    if(!hullpts.contains(pts[pts.size()-1]))
    {
        hullpts.push_back(pts[pts.size()-1]);
    }

    othpts.removeOne(pts[0]);
    othpts.removeOne(pts[pts.size()-1]);
    int i = 0;
    bool over = true;
    int maxdisIndex = 0;


    while(i<hullpts.size())  //����ȫ��͹��
    {
        Line cline;//ȡ͹����һ����
        if(i == hullpts.size() -1 ) //���һ���㣬���ӵ�һ����
        {
            cline.init(hullpts[i],hullpts[0]);
        }else
        {
            cline.init(hullpts[i],hullpts[i+1]);
        }
        float maxdis = 0;

        for( int m = 0; m< othpts.size(); m++)
        {
            if(IsRightPoint(othpts[m],cline))  //����ע�⣬��Ӧ���������Ѱ�ң�����QT��Ļ�е�����ϵ(y����),���Ҫ��һ��ת��
            {
                float dist = Gemetry::PointToLine(othpts[m],cline);

                if(dist > maxdis)
                {
                    maxdis = dist;
                    maxdisIndex = m;
                    over =false; //�����㣬�Ͳ�������
                }
            }
        }
        if(over == true) //�������´ε�����ȥ��͹������һ����
        {
            i++;
        }else{// ������,�´ε�����Ȼ��i�㿪ʼ��������һ�������³�����������λ��
            hullpts.insert(i+1,othpts[maxdisIndex]);
            othpts.removeAt(maxdisIndex);
            over = true;
        }

    }
    dispts = othpts;
}

QList<Triangle>& ConvexHull::GetInitTin(QList<Point>& pts)
{
    QList<Point> hpts;
    for(int i = 0; i< pts.size() ; i++)
        hpts.push_back(pts[i]);
    tins.clear();
    int index = 0;
    while(hpts.size() >2)
    {
        int tag = index;
        float minangle = 180;  //ÿ�ι������ڵıߣ������ҽǶ���С��
        float maxangle = 0;

        for(int i = index; i< hpts.size() ; i++)
        {
            float tri_angle = 180.0;

            if(i == 0){
                tri_angle = Gemetry::angle3D(hpts.last(),hpts[i],hpts[i+1]);

            }
            else if(i == hpts.size()-1){
                tri_angle = Gemetry::angle3D(hpts[i-1],hpts[i],hpts[0]);
                //                tri.init(hpts[i-1],hpts[i],hpts[0]);
            }
            else{
                tri_angle = Gemetry::angle3D(hpts[i-1],hpts[i],hpts[i+1]);
                //                tri.init(hpts[i-1],hpts[i],hpts[i+1]);
            }
            if(tri_angle < minangle)
            {
                tag = i;
                minangle = tri_angle;
            }
        }
        int tagb = tag-1;
        int tage = tag+1;
        if(tag == 0)
            tagb =hpts.size()-1;
        if(tag == hpts.size()-1)
            tage = 0;
        tins.push_back(Triangle(hpts[tagb],hpts[tag],hpts[tage]));
        hpts.removeAt(tag);
    }
    return tins;
}

double polar_angle(const QPoint& point) {
    return atan2(point.y(), point.x());
}

bool Compare_polar_angle(const QPoint& a, const QPoint& b) {
    double angle_a = polar_angle(a);
    double angle_b = polar_angle(b);
    if (angle_a != angle_b) {
        return angle_a > angle_b;
    }
    return a.y() > b.y();
}

std::vector<QPoint> ConvexHull::sort_points(std::vector<QPoint> points) {
    QPoint first_point = *min_element(points.begin(), points.end(),[](const QPoint& a, const QPoint& b) 
    {
        if (a.y() != b.y()) {
            return a.y() < b.y();
        }
        return a.x() < b.x();
    });

    std::sort(points.begin(), points.end(), Compare_polar_angle);

    return points;
}