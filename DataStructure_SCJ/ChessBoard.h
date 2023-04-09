#pragma once
#include <iostream>
#include<vector>
#include <fstream>
#include <map>
/// <summary>
/// �ݹ�������������㷨�������̸����
/// </summary>
class ChessBoard
{
    std::vector<std::vector<char>> grid;//����
    std::vector<std::vector<char>> maxAreaGrid;//�������
    std::map<char, int> colorMap;//��ɫӳ���
    int maxXPos; int maxYPos;//����ķ�����ͨ����һ�������
    char maxColor;//����ķ�����ͨ�����ɫ
public:
    ChessBoard(std::vector<std::vector<char>> grid, std::map<char, int>& color_count);//���캯��
    ~ChessBoard();//��������
    int getArea(std::vector<std::vector<char>>& grid, int i, int j, char color);//��ȡ������ݹ�����
    int traverseChessboard();// �������̣����õݹ�
    std::vector<std::vector<char>> getMaxArea();//�������ķ�����ͨ���������
};

