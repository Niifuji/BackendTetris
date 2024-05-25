#include "raylib.h"
#include "gameMainElements.h"
#include <string>
#include "font.h"

enum GameState
{
    LOADING,
    MENU,
    GAME,
    SETTINGS
};

#define NUM_FRAMES 712
Texture2D frames[NUM_FRAMES];
int currentFrame = 0;
float frameTime = 1.0f / 30.0f;
float timer = 0.0f;

GameState gameState = LOADING;
double UpdateTime = 0;
bool GamePaused = false;
bool countdown = false;
int countdownValue = 3;
double moveInterval = 2;
bool EventTrigger(double interv);
void DrawMenu(Font font);
void DrawGame(Font font, GameMainElements& gameMainElements);
void DrawCountdown(Font font, int countdownValue);
void DrawLoading(Font font, int loadedFrames, int totalFrames);

bool EventTrigger(double interv)
{
    double currentTime = GetTime();
    if (currentTime - UpdateTime >= interv)
    {
        UpdateTime = currentTime;
        return true;
    }
    return false;
}
int main()
{
    InitWindow(600, 800, "Backend Tetris");
    SetTargetFPS(30);
    GameMainElements gameMainElements = GameMainElements();
    Font font = LoadFont("path_to_font.ttf");

    int loadedFrames = 0;

    while (loadedFrames < NUM_FRAMES - 1 && !WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        frames[loadedFrames] = LoadTexture(TextFormat("Resource Files/frames/frame_%03d.png", loadedFrames + 1));
        loadedFrames++;

        DrawLoading(font, loadedFrames, NUM_FRAMES - 1);
        EndDrawing();
    }

    gameState = MENU;

    while (!WindowShouldClose())
    {
        timer += GetFrameTime();
        if (timer >= frameTime)
        {
            timer = 0.0f;
            currentFrame++;
            if (currentFrame >= NUM_FRAMES)
                currentFrame = 0;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(frames[currentFrame], 0, 0, WHITE);


        switch (gameState)
        {
        case MENU:
            DrawMenu(font);
            if (IsKeyPressed(KEY_ENTER))
            {
                countdown = true;
                countdownValue = 4;
            }
            break;
        case GAME:
            if (!GamePaused)
            {
                gameMainElements.input();
                if (EventTrigger(moveInterval))
                {
                    gameMainElements.MoveBlockDown();
                }
                if (IsKeyPressed(KEY_P))
                {
                    GamePaused = true;
                }
            }
            else
            {
                if (IsKeyPressed(KEY_P))
                {
                    GamePaused = false;
                }
            }
            DrawGame(font, gameMainElements);
            if (IsKeyPressed(KEY_R))
            {
                gameMainElements.Reset();
            }
            if (gameMainElements.GameOver && IsKeyPressed(KEY_ENTER))
            {
                gameMainElements.Reset();
                gameState = MENU;
            }
            break;
        }

        if (countdown)
        {
            DrawCountdown(font, countdownValue);
            if (EventTrigger(1))
            {
                countdownValue--;
                if (countdownValue < 0)
                {
                    countdown = false;
                    gameState = GAME;
                }
            }
        }

        DrawTextEx(font, "made with love by alicia <3", { 20, 750 }, 24, 2, BLACK);
        EndDrawing();
    }

    for (int i = 0; i < NUM_FRAMES; i++)
    {
        UnloadTexture(frames[i]);
    }
    UnloadFont(font);
    CloseWindow();
    return 0;
}

void DrawMenu(Font font)
{
    DrawTextEx(font, "BACKEND'S TETRIS", { 50, 100 }, 40, 2, BLACK);
    DrawTextEx(font, "Press ENTER to Start", { 90, 200 }, 25, 2, BLACK);
}

void DrawGame(Font font, GameMainElements& gameMainElements)
{
    DrawTextEx(font, "SCORE", { 380, 10 }, 30, 2, BLACK);
    DrawTextEx(font, "NEXT", { 390, 120 }, 30, 2, BLACK);

    char score[10];
    sprintf_s(score, "%d", gameMainElements.Score);
    DrawRectangleRounded({ 350, 40, 200, 60 }, 0.3, 0, { 128, 128, 128, 192 });
    DrawTextEx(font, score, { 380, 55 }, 25, 2, WHITE);   // Smaller font
    char combo[10];
    sprintf_s(combo, "Combo: %d", gameMainElements.GetComboCount());
    DrawTextEx(font, combo, { 480, 750 }, 20, 2, BLACK);

    if (gameMainElements.GameOver)
    {
        DrawTextEx(font, "GAME OVER", { 280, 200 }, 30, 2, BLACK);
    }
    else if (GamePaused)
    {
        DrawTextEx(font, "PAUSED", { 300, 200 }, 30, 2, BLACK);
    }

    gameMainElements.Draw();
}

void DrawCountdown(Font font, int countdownValue)
{
    if (countdownValue > 0)
    {
        DrawTextEx(font, TextFormat("%d", countdownValue), { 270, 500 }, 100, 2, BLACK);
    }
    else
    {
        DrawTextEx(font, "GO!", { 270, 500 }, 100, 2, BLACK);
    }
}
void DrawLoading(Font font, int loadedFrames, int totalFrames)
{
    float progress = (float)loadedFrames / (float)totalFrames;
    DrawTextEx(font, "Loading...", { 210, 300 }, 50, 2, BLACK);
    DrawRectangle(120, 600, 380, 50, LIGHTGRAY);
    DrawRectangle(120, 600, (int)(380 * progress), 50, PINK);
    DrawTextEx(font, TextFormat("%d%%", (int)(progress * 100)), { 250, 400 }, 30, 2, BLACK);
}
