#include "Color.h"
const Color darkGrey = { 26, 31, 40, 255 };
const Color green = { 47, 230, 23, 255 };
const Color red = { 230, 47, 23, 255 };
const Color orange = { 230, 123, 23, 255 };
const Color yellow = { 230, 230, 23, 255 };
const Color purple = { 123, 23, 230, 255 };
const Color blue = { 23, 123, 230, 255 };
const Color cyan = { 23, 230, 230, 255 };
const Color LightBlue = { 26, 31, 40, 255 };
std::vector<Color> GetCellColor()
{
    return { darkGrey, green, red, orange, yellow, purple, blue, cyan, LightBlue };
}
