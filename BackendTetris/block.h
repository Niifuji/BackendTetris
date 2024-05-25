#pragma once
#include <vector>
#include <map>
#include "pos.h"
#include "color.h"
#include "grid.h"
class Block
{
public:
    Block();
    int id;
    void Draw(int offsetX, int offsetY);
    void Rotate();
    void UndoRotate();
    void Rotate180();
    std::vector<Pos> GetCellPos();
    void Move(int rowOff, int colOff);
    void UndoRotate180();
    void ResetPosition();
    void RotateCounterClockwise();
    void UndoRotateCounterClockwise();
    std::map<int, std::vector<Pos>> cells;
    std::vector<Color> colors;
    int kichCoO;
    int rotateState;
    int rowOffS;
    int colOffS;
    int initialRowOffS;
    int initialColOffS;
};
class Lblock : public Block
{
public:
    Lblock();
};
class Jblock : public Block
{
public:
    Jblock();
};
class Iblock : public Block
{
public:
    Iblock();
};
class Oblock : public Block
{
public:
    Oblock();
};
class Sblock : public Block
{
public:
    Sblock();
};
class Tblock : public Block
{
public:
    Tblock();
};
class Zblock : public Block
{
public:
    Zblock();
};
