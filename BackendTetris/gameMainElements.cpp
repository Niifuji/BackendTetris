#include "grid.h"
#include "block.h"
#include <random>
#include <chrono>
#include "gameMainElements.h"
#include "font.h"

// Constructor to initialize the game elements
GameMainElements::GameMainElements()
{
    grid = Grid();                                   // Initialize the grid
    blocks = GetAllBlock();                          // Get all blocks
    current = GetRandom();                           // Set current block to a random block
    next = GetRandom();                              // Set next block to a random block
    heldBlock = Block();                             // Initialize held block
    heldBlock.id = -1;                               // Set held block id to -1 indicating no block is held
    GameOver = false;                                // Initialize GameOver flag
    Score = 0;                                       // Initialize score
    comboCount = 0;                                  // Initialize combo count
    canHold = true;                                  // Allow holding a block
    lastMoveLeft = std::chrono::steady_clock::now(); // Initialize time points for last move
    lastMoveRight = std::chrono::steady_clock::now();
    lastMoveDown = std::chrono::steady_clock::now();
    lastInputCheck = std::chrono::steady_clock::now();
    lastInputTime = std::chrono::steady_clock::now();
    delayActive = false;                           // Initialize delay flag
    startDelay = std::chrono::steady_clock::now(); // Initialize start delay time point
    moveInterval = std::chrono::milliseconds(50);  // Set move interval to 50 milliseconds
    lockDelay = std::chrono::milliseconds(800);   // Set lock delay to 800 milliseconds
}

// Get the current combo count
int GameMainElements::GetComboCount() const
{
    return comboCount;
}

// Get a random block from the available blocks
Block GameMainElements::GetRandom()
{
    if (blocks.empty())
    {
        blocks = GetAllBlock(); // Refill blocks if empty
    }
    int random = rand() % blocks.size();   // Get random index
    Block block = blocks[random];          // Get block at random index
    blocks.erase(blocks.begin() + random); // Remove the selected block from the list
    return block;
}

// Get all block types
std::vector<Block> GameMainElements::GetAllBlock()
{
    return { Iblock(), Tblock(), Sblock(), Oblock(), Jblock(), Lblock(), Zblock() }; // Return a vector of all block types
}

// Draw the game elements
void GameMainElements::Draw()
{
    grid.draw();                                                              // Draw the grid
    current.Draw(10, 10);                                                     // Draw the current block
    next.Draw(350, 200);                                                      // Draw the next block
    DrawRectangleRounded({ 350, 400, 200, 100 }, 0.5, 0, { 128, 128, 128, 192 }); // Draw the hold area
    if (heldBlock.id != -1)
    { // Draw the held block if any
        heldBlock.Draw(370, 420);
    }
}

// Lock the current block in place and check for game over or row clears
void GameMainElements::LockBlock()
{
    std::vector<Pos> tiles = current.GetCellPos(); // Get the positions of the current block's tiles
    for (Pos tile : tiles)
    {
        grid.grid[tile.row][tile.col] = current.id; // Lock each tile into the grid
    }
    current = next; // Set current block to the next block
    canHold = true; // Allow holding again
    if (!BlockFit())
    {                    // Check if the new current block fits
        GameOver = true; // If not, game over
    }
    next = GetRandom();                  // Get a new next block
    int RowClear = grid.clearFullRows(); // Clear full rows and get the number of rows cleared
    if (RowClear > 0)
    {
        comboCount++; // Increase combo count if rows were cleared
    }
    else
    {
        comboCount = 0; // Reset combo count if no rows were cleared
    }
    UpdateScore(RowClear); // Update the score based on rows cleared
}

// Handle player input for moving and rotating blocks
void GameMainElements::input()
{
    auto now = std::chrono::steady_clock::now(); // Get current time
    int key = GetKeyPressed();                   // Get the last key pressed

    if (GameOver && key == KEY_ENTER)
    { // Restart the game if game over and enter is pressed
        GameOver = false;
        Reset();
    }
    if (!GameOver)
    { // Handle input if game is not over
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastInputCheck) >= moveInterval)
        {
            if (IsKeyDown(KEY_DOWN))
            { // Move block down if down key is pressed
                MoveBlockDown();
                lastMoveDown = now;
                lastInputTime = now;
            }

            if (IsKeyDown(KEY_LEFT))
            { // Move block left if left key is pressed
                MoveBlockLeft();
                lastMoveLeft = now;
                lastInputTime = now;
            }

            if (IsKeyDown(KEY_RIGHT))
            { // Move block right if right key is pressed
                MoveBlockRight();
                lastMoveRight = now;
                lastInputTime = now;
            }

            lastInputCheck = now; // Update the last input check time
        }

        switch (key)
        { // Handle other key presses
        case KEY_UP:
            RotateBlock(); // Rotate block clockwise
            lastInputTime = now;
            break;
        case KEY_Z:
            RotateBlockCounterClockwise(); // Rotate block counterclockwise
            lastInputTime = now;
            break;
        case KEY_SPACE:
            DropBlock(); // Drop block instantly
            lastInputTime = now;
            break;
        case KEY_LEFT_SHIFT:
            if (canHold)
            { // Swap held block if holding is allowed
                SwapHoldBlock();
                canHold = false;
                lastInputTime = now;
            }
            break;
        case KEY_A:
            current.Rotate180(); // Rotate block 180 degrees
            if (isBlockOutside() || !BlockFit())
            { // Undo rotation if the block doesn't fit
                current.UndoRotate180();
            }
            lastInputTime = now;
            break;
        }
    }
}

// Move the block left if possible
void GameMainElements::MoveBlockLeft()
{
    if (!GameOver)
    {
        current.Move(0, -1); // Move block left
        if (isBlockOutside() || !BlockFit())
        { // Undo move if the block doesn't fit
            current.Move(0, 1);
        }
    }
}

// Move the block right if possible
void GameMainElements::MoveBlockRight()
{
    if (!GameOver)
    {
        current.Move(0, 1); // Move block right
        if (isBlockOutside() || !BlockFit())
        { // Undo move if the block doesn't fit
            current.Move(0, -1);
        }
    }
}

// Move the block down if possible, lock the block if it cannot move further
void GameMainElements::MoveBlockDown()
{
    if (!GameOver)
    {
        current.Move(1, 0); // Move block down
        if (isBlockOutside() || !BlockFit())
        {                        // If block cannot move further down
            current.Move(-1, 0); // Undo move

            if (!delayActive)
            { // Start the lock delay if not already active
                startDelay = std::chrono::steady_clock::now();
                delayActive = true;
            }

            auto now = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - startDelay).count() >= lockDelay.count())
            {
                LockBlock(); // Lock the block if delay has passed
                delayActive = false;
            }
        }
        else
        {
            delayActive = false; // Reset delay if block can move down
        }
    }
}

// Drop the block instantly
void GameMainElements::DropBlock()
{
    if (!GameOver)
    {
        while (!isBlockOutside() && BlockFit())
        {
            current.Move(1, 0); // Move block down until it cannot move further
        }
        current.Move(-1, 0); // Move back to the last valid position
        LockBlock();         // Lock the block
    }
}

// Check if the block is outside the grid
bool GameMainElements::isBlockOutside()
{
    std::vector<Pos> tiles = current.GetCellPos();
    for (Pos tile : tiles)
    {
        if (tile.row < 0 || tile.row >= grid.soHang || tile.col < 0 || tile.col >= grid.soCot)
        {
            return true; // Block is outside the grid
        }
    }
    return false;
}
// Reset the game to the initial state
void GameMainElements::Reset()
{
    grid = Grid();          // Reset the grid
    blocks = GetAllBlock(); // Refill blocks
    current = GetRandom();  // Set current block to a random block
    next = GetRandom();     // Set next block to a random block
    Score = 0;              // Reset score
    comboCount = 0;         // Reset combo count
    canHold = true;         // Allow holding a block
    delayActive = false;    // Reset delay flag
    GameOver = false;       // Reset game over flag
    heldBlock.id = -1;       // Reset held block
}
// Update the score based on the number of lines cleared
void GameMainElements::UpdateScore(int LineCleared)
{
    switch (LineCleared)
    {
    case 1:
        Score += 100;
        break;
    case 2:
        Score += 300;
        break;
    case 3:
        Score += 500;
        break;
    case 4:
        Score += 1000;
        break;
    default:
        break;
    }
    Score += comboCount * 50; // Add combo bonus
}

// Swap the current block with the held block
void GameMainElements::SwapHoldBlock()
{
    if (heldBlock.id == -1)
    {                        // If no block is held
        heldBlock = current; // Hold the current block
        heldBlock.rotateState = 0;
        current = next;     // Set current block to the next block
        next = GetRandom(); // Get a new next block
    }
    else
    { // Swap the current block with the held block
        Block temp = current;
        current = heldBlock;
        current.ResetPosition();
        heldBlock = temp;
        heldBlock.rotateState = 0;
        heldBlock.ResetPosition();
    }
}

// Adjust the block position if it goes out of bounds during rotation
bool GameMainElements::AdjustPositionForRotation()
{
    std::vector<Pos> tiles = current.GetCellPos();
    int leftmost = tiles[0].col, rightmost = tiles[0].col;
    int topmost = tiles[0].row, bottommost = tiles[0].row;

    for (Pos tile : tiles)
    {
        leftmost = std::min(leftmost, tile.col);
        rightmost = std::max(rightmost, tile.col);
        topmost = std::min(topmost, tile.row);
        bottommost = std::max(bottommost, tile.row);
    }

    int offsetX = 0, offsetY = 0;
    if (leftmost < 0)
        offsetX = -leftmost;
    else if (rightmost >= grid.soCot)
        offsetX = grid.soCot - 1 - rightmost;

    if (topmost < 0)
        offsetY = -topmost;
    else if (bottommost >= grid.soHang)
        offsetY = grid.soHang - 1 - bottommost;
    if (current.rotateState == 3 && offsetX == 0 && offsetY == 0 && !BlockFit())
    {
        if (current.id == 0)
        {
            offsetX = -1;
        }
        else
        {
            offsetX = 1;
        }
    }

    current.Move(offsetY, offsetX);
    return !isBlockOutside() && BlockFit();
}

// Rotate the block clockwise
void GameMainElements::RotateBlock()
{
    if (!GameOver)
    {
        current.Rotate();
        if (isBlockOutside() || !BlockFit())
        { // Adjust position if the block doesn't fit
            if (!AdjustPositionForRotation())
            {
                current.UndoRotate(); // Undo rotation if adjustment fails
            }
        }
    }
}

// Rotate the block counterclockwise
void GameMainElements::RotateBlockCounterClockwise()
{
    if (!GameOver)
    {
        current.RotateCounterClockwise();
        if (isBlockOutside() || !BlockFit())
        { // Adjust position if the block doesn't fit
            if (!AdjustPositionForRotation())
            {
                current.UndoRotateCounterClockwise(); // Undo rotation if adjustment fails
            }
        }
    }
}

// Check if the block fits in the current position

bool GameMainElements::BlockFit()
{
    std::vector<Pos> tiles = current.GetCellPos();
    for (Pos tile : tiles)
    {
        if (tile.row >= grid.soHang || tile.col < 0 || tile.col >= grid.soCot ||
            (tile.row >= 0 && !grid.IsCellEmpty(tile.row, tile.col)))
        {
            return false;
        }
    }
    return true;
}

// Im suck at memorize everything in this terrible coding technique so i have to comment all the code to understand it
// Gomenasai