#pragma once
#include <vector>
#include "raylib.h"
#include <iostream>
#include "color.h"
class Grid
{
public:
    Grid();
    int grid[20][10];
    void print();
    void draw();
    bool IsCellOutside(int row, int column);
    bool IsCellEmpty(int row, int column);
    int clearFullRows();
    int soHang;
    int soCot;
    int kichCoO;
    bool IsRowFull(int row);
    void Clear(int row);
    std::vector<Color> colors;
    void MoveRowDown(int row, int numberofRow);
    Color backgroundColor;
};
