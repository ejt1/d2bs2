#pragma once

#include "Structs.h"

#include <string>

// undefine windows retarded DrawText
#undef DrawText

namespace d2bs {
namespace game {

void WorldToScreen(POINT* pos);
void ScreenToWorld(POINT* pt);
POINT ScreenToAutomap(int x, int y);
void AutomapToScreen(POINT* pt);

POINT CalculateTextLength(const char* text, int font);
void DrawText(const std::string& text, POINT* pt, int color, int font);
void DrawText(const std::wstring& text, POINT* pt, int color, int font);
void DrawCenterText(const std::string& text, POINT* pt, int color, int font, int div);
void DrawCenterText(std::wstring& text, POINT* pt, int color, int font, int div);

POINT ScreenSize();
int ScreenSizeX();
int ScreenSizeY();

bool ScrollingText();

}
}
