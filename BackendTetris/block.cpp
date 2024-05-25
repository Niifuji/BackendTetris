#include "block.h"
#include "pos.h"
#include "color.h"
#include <random>
Block::Block()
{
    id = rand() % 7 + 1;

    kichCoO = 30;
    rotateState = 0;
    colors = GetCellColor();
    rowOffS = 0;
    colOffS = 0;
    initialRowOffS = 0;
    initialColOffS = 0;
}
void Block::Draw(int offsetX, int offsetY)
{
    std::vector<Pos> tiles = GetCellPos();
    for (Pos tile : tiles)
    {
        DrawRectangle(tile.col * kichCoO + offsetX, tile.row * kichCoO + offsetY, kichCoO - 1, kichCoO - 1, colors[id]);
    }
}
void Block::Move(int rowOff, int colOff)
{
    rowOffS += rowOff;
    colOffS += colOff;
}
std::vector<Pos> Block::GetCellPos()
{

    std::vector<Pos> tiles = cells[rotateState];
    std::vector<Pos> result;
    for (Pos tile : tiles)
    {
        Pos newTile = Pos(tile.row + rowOffS, tile.col + colOffS);
        result.push_back(newTile);
    }
    return result;
}
void Block::Rotate()
{
    rotateState++;
    if (rotateState == cells.size())
    {
        rotateState = 0;
    }
}
void Block::UndoRotate()
{
    rotateState--;
    if (rotateState == -1)
    {
        rotateState = cells.size() - 1;
    }
}
void Block::Rotate180()
{
    rotateState += 2;
    if (rotateState >= cells.size())
    {
        rotateState -= cells.size();
    }
}
void Block::UndoRotate180()
{
    rotateState -= 2;
    if (rotateState < 0)
    {
        rotateState += cells.size();
    }
}

void Block::RotateCounterClockwise()
{
    rotateState--;
    if (rotateState == -1)
    {
        rotateState = cells.size() - 1;
    }
}
void Block::UndoRotateCounterClockwise()
{
    rotateState++;
    if (rotateState == cells.size())
    {
        rotateState = 0;
    }
}
void Block::ResetPosition()
{
    rowOffS = initialRowOffS;
    colOffS = initialColOffS;
}
Lblock::Lblock()
{
    id = 1;
    cells[0] = { Pos(0, 2), Pos(1, 0), Pos(1, 1), Pos(1, 2) };
    cells[1] = { Pos(0, 1), Pos(1, 1), Pos(2, 1), Pos(2, 2) };
    cells[2] = { Pos(1, 0), Pos(1, 1), Pos(1, 2), Pos(2, 0) };
    cells[3] = { Pos(0, 0), Pos(0, 1), Pos(1, 1), Pos(2, 1) };
    Move(0, 3);
}
Jblock::Jblock()
{
    id = 2;
    cells[0] = { Pos(0, 0), Pos(1, 0), Pos(1, 1), Pos(1, 2) };
    cells[1] = { Pos(0, 1), Pos(0, 2), Pos(1, 1), Pos(2, 1) };
    cells[2] = { Pos(1, 0), Pos(1, 1), Pos(1, 2), Pos(2, 2) };
    cells[3] = { Pos(0, 1), Pos(1, 1), Pos(2, 0), Pos(2, 1) };
    Move(0, 3);
}
Iblock::Iblock()
{
    id = 3;
    cells[0] = { Pos(1, 0), Pos(1, 1), Pos(1, 2), Pos(1, 3) };
    cells[1] = { Pos(0, 2), Pos(1, 2), Pos(2, 2), Pos(3, 2) };
    cells[2] = { Pos(2, 0), Pos(2, 1), Pos(2, 2), Pos(2, 3) };
    cells[3] = { Pos(0, 1), Pos(1, 1), Pos(2, 1), Pos(3, 1) };
    Move(0, 3);
}
Oblock::Oblock()
{
    id = 4;
    cells[0] = { Pos(0, 0), Pos(0, 1), Pos(1, 0), Pos(1, 1) };
    cells[1] = { Pos(0, 0), Pos(0, 1), Pos(1, 0), Pos(1, 1) };
    cells[2] = { Pos(0, 0), Pos(0, 1), Pos(1, 0), Pos(1, 1) };
    cells[3] = { Pos(0, 0), Pos(0, 1), Pos(1, 0), Pos(1, 1) };
    Move(0, 3);
}
Sblock::Sblock()
{
    id = 5;
    cells[0] = { Pos(0, 1), Pos(0, 2), Pos(1, 0), Pos(1, 1) };
    cells[1] = { Pos(0, 1), Pos(1, 1), Pos(1, 2), Pos(2, 2) };
    cells[2] = { Pos(1, 1), Pos(1, 2), Pos(2, 0), Pos(2, 1) };
    cells[3] = { Pos(0, 0), Pos(1, 0), Pos(1, 1), Pos(2, 1) };
    Move(0, 3);
}
Tblock::Tblock()
{
    id = 6;
    cells[0] = { Pos(0, 1), Pos(1, 0), Pos(1, 1), Pos(1, 2) };
    cells[1] = { Pos(0, 1), Pos(1, 1), Pos(1, 2), Pos(2, 1) };
    cells[2] = { Pos(1, 0), Pos(1, 1), Pos(1, 2), Pos(2, 1) };
    cells[3] = { Pos(0, 1), Pos(1, 0), Pos(1, 1), Pos(2, 1) };
    Move(0, 3);
}
Zblock::Zblock()
{
    id = 7;
    cells[0] = { Pos(0, 0), Pos(0, 1), Pos(1, 1), Pos(1, 2) };
    cells[1] = { Pos(0, 2), Pos(1, 1), Pos(1, 2), Pos(2, 1) };
    cells[2] = { Pos(1, 0), Pos(1, 1), Pos(2, 1), Pos(2, 2) };
    cells[3] = { Pos(0, 1), Pos(1, 0), Pos(1, 1), Pos(2, 0) };
    Move(0, 3);
}