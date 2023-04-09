#include "ChessBoard.h"

ChessBoard::ChessBoard(std::vector<std::vector<char>> grid, std::map<char, int>& color_count)
{
    this->grid = grid;
    this->colorMap = color_count;
    this->maxAreaGrid = grid;
}

ChessBoard::~ChessBoard()
{
}

int ChessBoard::getArea(std::vector<std::vector<char>>& grid, int i, int j, char color)
{
    //��������ÿ�� +1 �������ж��Ƿ�������鳤�ȼ���
    if (i >= grid.size() || i < 0)
        return 0;
    else if (j >= grid[0].size() || j < 0)
        return 0;
    if (grid[i][j] == color)
    {
        grid[i][j] = -1;
        return 1 + getArea(grid, i + 1, j, color) + getArea(grid, i - 1, j, color) + getArea(grid, i, j + 1, color) + getArea(grid, i, j - 1, color);
    }
    return 0;
}
int ChessBoard::traverseChessboard()
{
    int maxArea = 0;
    int area = 0;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[0].size(); j++)
        {
            for (auto it = colorMap.begin(); it != colorMap.end(); ++it)
            {
                if (grid[i][j] == it->first)
                {                
                    area = getArea(grid, i, j, it->first);
                    if (area > maxArea)
                    {
                        maxArea = area;
                        maxXPos = i;
                        maxYPos = j;
                        maxColor = maxAreaGrid[i][j];
                    }
                }
            }
        }
    }
    return maxArea;
}

/// <summary>
/// �������������������̵�����������Ϊ-1
/// </summary>
/// <returns></returns>
std::vector<std::vector<char>> ChessBoard::getMaxArea()
{
    getArea(maxAreaGrid, maxXPos, maxYPos, maxColor);
    return maxAreaGrid;
}
