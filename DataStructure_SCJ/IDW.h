#pragma once
#include <time.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "kdtree.h"
#include <vector>
#include <array>
class IDW
{
	Kdtree::KdNodeVector nodes;
	Kdtree::KdTree* IDWtree;
public:
	IDW();
	IDW(std::vector<std::vector<double>>& points);
	double calDistance(const Kdtree::CoordPoint& p, const Kdtree::CoordPoint& q);
	void calWeight(const Kdtree::KdNode& nodeUnknown, const Kdtree::KdNodeVector& nodes, std::vector<double>& weight);
	double calValue(const Kdtree::KdNode& nodeUnknown, const Kdtree::KdNodeVector& nodes);
	void print_nodes() const;
	void calIDW(const std::vector<std::vector<double>>& points_unknown, std::vector<double>& values,int k);
};

