#pragma once
#include <iostream>
#include<vector>
#include <fstream>
#include <map>
/// <summary>
/// 递归深度优先搜索算法用于棋盘格求解
/// </summary>
class ChessBoard
{
    std::vector<std::vector<char>> grid;//棋盘
    std::vector<std::vector<char>> maxAreaGrid;//最大区域
    std::map<char, int> colorMap;//颜色映射表
    int maxXPos; int maxYPos;//最大四方向连通域中一点的坐标
    char maxColor;//最大四方向连通域的颜色
public:
    ChessBoard(std::vector<std::vector<char>> grid, std::map<char, int>& color_count);//构造函数
    ~ChessBoard();//析构函数
    int getArea(std::vector<std::vector<char>>& grid, int i, int j, char color);//获取面积，递归深搜
    int traverseChessboard();// 遍历棋盘，调用递归
    std::vector<std::vector<char>> getMaxArea();//求解最大四方向连通域的最大面积
};

