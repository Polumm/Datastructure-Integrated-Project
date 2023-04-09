#include "IDW.h"
/// <summary>
/// ��Vector���鹹��
/// </summary>
/// <param name="points"></param>
IDW::IDW(std::vector<std::vector<double>>& points) {
    // ����kd��(��ֵ�ռ�)
    for (int i = 0; i < points.size(); ++i) {
        std::vector<double> point;
        point.push_back(points[i][0]);
        point.push_back(points[i][1]);
        nodes.push_back(Kdtree::KdNode(point, &points[i][2]));
    }
}
/// <summary>
/// ��������Ҷ�ӽڵ�֮���ŷʽ����
/// </summary>
/// <param name="p"></param>
/// <param name="q"></param>
/// <returns></returns>
double IDW::calDistance(const Kdtree::CoordPoint& p, const Kdtree::CoordPoint& q) {
    size_t i;
    double dist = 0.0;
    for (i = 0; i < p.size(); i++) dist += (p[i] - q[i]) * (p[i] - q[i]);
    return sqrt(dist);
}
/// <summary>
/// ����KD������Ȩ���б�
/// </summary>
/// <param name="nodeUnknown">����ֵ��</param>
/// <param name="nodes">k���ڽڵ��б�</param>
/// <param name="weight">Ȩ���б�</param>
void IDW::calWeight(const Kdtree::KdNode& nodeUnknown, const Kdtree::KdNodeVector& nodes, std::vector<double>& weight) {
    double sigma = 0, distanceRevr = 0;
    for (int i = 0; i < nodes.size(); i++)
    {
        distanceRevr = 1 / calDistance(nodeUnknown.point, nodes[i].point);
        sigma += distanceRevr;
        weight.push_back(distanceRevr);
    }
    for (int i = 0; i < nodes.size(); i++) weight[i] /= sigma;
}
/// <summary>
/// ����KD������Ȩֵ��
/// </summary>
/// <param name="nodeUnknown"></param>
/// <param name="nodes"></param>
/// <returns></returns>
double IDW::calValue(const Kdtree::KdNode& nodeUnknown, const Kdtree::KdNodeVector& nodes)
{
    double sigma = 0;
    std::vector<double> weight;
    calWeight(nodeUnknown, nodes, weight);
    for (int i = 0; i < nodes.size(); i++) sigma += (*((double*)nodes[i].data)) * weight[i];
    return sigma;
}
/// <summary>
/// ��ӡIDW�������нڵ㼰������ֵ
/// </summary>
void IDW::print_nodes() const
{
    size_t i, j;
    for (i = 0; i < nodes.size(); ++i) {
        if (i > 0)
            std::cout << " ";
        std::cout << "(";
        for (j = 0; j < nodes[i].point.size(); j++) {
            if (j > 0)
                std::cout << ",";
            std::cout << nodes[i].point[j];
        }
        std::cout << " " << *((double*)nodes[i].data);
        std::cout << ")";
    }
    std::cout << '\n';
}
/// <summary>
/// Ѱ��k�ڽ�����������ֵ
/// </summary>
/// <param name="points_unknown">δ֪���б�</param>
/// <param name="values">���������б�</param>
/// <param name="k">��ֵ������ڽ��ڵ���</param>
void IDW::calIDW(const std::vector<std::vector<double>>& points_unknown, std::vector<double>& values,int k)
{
    Kdtree::KdTree tree(&nodes);
    IDWtree = &tree;
    Kdtree::KdNodeVector pointUnknow;
    Kdtree::KdNodeVector neibours;
    for (int i = 0; i < points_unknown.size(); ++i) {
        std::vector<double> point;
        point.push_back(points_unknown[i][0]);
        point.push_back(points_unknown[i][1]);
        pointUnknow.push_back(Kdtree::KdNode(point));
    }

    for (int i = 0; i < pointUnknow.size(); i++)
    {
        IDWtree->k_nearest_neighbors(pointUnknow[i].point, k, &neibours);
        values.push_back(calValue(pointUnknow[i].point, neibours));
        std::cout << values[i] << " ";
    }
}