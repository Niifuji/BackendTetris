#pragma once
#include "grid.h"
#include "block.h"
#include "color.h"
#include <vector>
#include <chrono>
class GameMainElements
{
public:
    void Draw();
    std::vector<Block> GetAllBlock();
    GameMainElements();
    Grid grid;
    Block GetRandom();
    void input();
    void MoveBlockLeft();
    void MoveBlockRight();
    void MoveBlockDown();
    void DropBlock();
    void SwapHoldBlock();
    bool GameOver;
    int Score;
    int GetComboCount() const;
    void Reset();
    std::chrono::steady_clock::time_point startDelay;
    bool delayActive;
    void LockBlock();
    bool isBlockOutSite();
    void UpdateScore(int LineCleared);
    void RotateBlock();
    void RotateBlockCounterClockwise();
    bool BlockFit();
    std::vector<Block> blocks;
    Block current;
    Block next;
    std::chrono::time_point<std::chrono::steady_clock> lastInputTime;
    Block heldBlock;
    bool canHold;
    int comboCount;
    std::chrono::steady_clock::time_point lastMoveLeft, lastMoveRight, lastMoveDown, lastInputCheck;
    std::chrono::milliseconds moveInterval = std::chrono::milliseconds(400);
    std::chrono::milliseconds lockDelay = std::chrono::milliseconds(1600);
    bool isBlockOutside();
    bool AdjustPositionForRotation();
};
