#include <vector>
#include "raylib.h"
#include <iostream>
#include "color.h"
#include "grid.h"

Grid::Grid() : backgroundColor(CLITERAL(Color) { 0, 0, 0, 255 })
{
    soHang = 20;
    soCot = 10;
    kichCoO = 30;
    for (int i = 0; i < soHang; i++)
    {
        for (int j = 0; j < soCot; j++)
        {
            grid[i][j] = 0;
        }
    }
}
void Grid::print()
{
    for (int i = 0; i < soHang; i++)
    {
        for (int j = 0; j < soCot; j++)
        {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
void Grid::draw()
{
    DrawRectangle(10, 10, soCot * kichCoO, soHang * kichCoO, { 128, 128, 128, 192 });
    for (int i = 0; i < soHang; i++)
    {
        for (int j = 0; j < soCot; j++)
        {
            Rectangle cellRect = { static_cast<float>(j * kichCoO + 10), static_cast<float>(i * kichCoO + 10), static_cast<float>(kichCoO - 1), static_cast<float>(kichCoO - 1) };
            DrawRectangleLinesEx(cellRect, 1.0f, WHITE);
            int cell = grid[i][j];
            if (cell != 0)
            {
                DrawRectangleRec(cellRect, GetCellColor()[cell]);
            }
        }
    }
}
bool Grid::IsCellOutside(int row, int column)
{
    if (row >= 0 && row < soHang && column >= 0 && column < soCot)
    {
        return false;
    }
    return true;
}
bool Grid::IsCellEmpty(int row, int column)
{
    if (grid[row][column] == 0)
    {
        return true;
    }
    return false;
}
bool Grid::IsRowFull(int row)
{
    for (int j = 0; j < soCot; j++)
    {
        if (grid[row][j] == 0)
        {
            return false;
        }
    }
    return true;
}
void Grid::Clear(int row)
{

    for (int j = 0; j < soCot; j++)
    {
        grid[row][j] = 0;
    }
}
void Grid::MoveRowDown(int row, int numberofRow)
{
    for (int i = 0; i < soCot; i++)
    {
        grid[row + numberofRow][i] = grid[row][i];
        grid[row][i] = 0;
    }
}
int Grid::clearFullRows()
{
    int soHangDaXoa = 0;
    for (int i = soHang - 1; i >= 0; i--)
    {
        if (IsRowFull(i))
        {
            Clear(i);

            soHangDaXoa++;
        }
        else if (soHangDaXoa > 0)
        {
            MoveRowDown(i, soHangDaXoa);
        }
    }
    return soHangDaXoa;
}
