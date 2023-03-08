#include "Screen.h"

#include "Game.h"
#include "Offset.h"
#include "Ptrs.h"
#include "utils/Localization.h"

namespace d2bs {
namespace game {

void WorldToScreen(POINT* pos) {
  D2COMMON_MapToAbsScreen(&pos->x, &pos->y);
  pos->x -= D2CLIENT_GetMouseXOffset();
  pos->y -= D2CLIENT_GetMouseYOffset();
}

void ScreenToWorld(POINT* pt) {
  D2COMMON_MapToAbsScreen(&pt->x, &pt->y);
  pt->x -= D2CLIENT_GetMouseXOffset();
  pt->y -= D2CLIENT_GetMouseYOffset();
}

POINT ScreenToAutomap(int x, int y) {
  POINT result{};
  x *= 32;
  y *= 32;
  result.x = ((x - y) / 2 / (*p_D2CLIENT_Divisor)) - (*p_D2CLIENT_Offset).x + 8;
  result.y = ((x + y) / 4 / (*p_D2CLIENT_Divisor)) - (*p_D2CLIENT_Offset).y - 8;
  if (D2CLIENT_GetAutomapSize()) {
    --result.x;
    result.y += 5;
  }
  return result;
}

void AutomapToScreen(POINT* pt) {
  pt->x = 8 - p_D2CLIENT_Offset->x + (pt->x * (*p_D2CLIENT_AutomapMode));
  pt->y = 8 + p_D2CLIENT_Offset->y + (pt->y * (*p_D2CLIENT_AutomapMode));
}

POINT CalculateTextLength(const char* text, int font) {
  return POINT();
}

void DrawText(const std::string& text, POINT* pt, int color, int font) {
  std::wstring ustr = util::UTF8ToWide(text);
  DrawText(ustr, pt, color, font);
}

void DrawText(const std::wstring& text, POINT* pt, int color, int font) {
  DWORD old = D2WIN_SetTextSize(font);
  D2WIN_DrawText(text.c_str(), pt->x, pt->y, color, 0);
  D2WIN_SetTextSize(old);
}

void DrawCenterText(const std::string& text, POINT* pt, int color, int font, int div) {
  std::wstring ustr = util::UTF8ToWide(text);
  DrawCenterText(ustr, pt, color, font, div);
}

void DrawCenterText(std::wstring& text, POINT* pt, int color, int font, int div) {
  DWORD width = 0;
  DWORD fileno = 0;
  DWORD old = D2WIN_SetTextSize(font);
  D2WIN_GetTextSize(&text[0], &width, &fileno);
  D2WIN_SetTextSize(old);
  POINT adjusted{pt->x - (width >> div), pt->y};
  DrawText(text, &adjusted, color, font);
}

POINT ScreenSize() {
  if (sGame->State() == GameState::Menu) {
    return {800, 600};
  }
  return {static_cast<LONG>(*p_D2CLIENT_ScreenSizeX), static_cast<LONG>(*p_D2CLIENT_ScreenSizeY)};
}

int ScreenSizeX() {
  return ScreenSize().x;
}

int ScreenSizeY() {
  return ScreenSize().y;
}

bool ScrollingText() {
  if (sGame->State() != GameState::InGame) {
    return false;
  }

  // XXX: wtf?
  HWND d2Hwnd = D2GFX_GetHwnd();
  WindowHandlerList* whl = p_STORM_WindowHandlers->table[(0x534D5347 ^ (DWORD)d2Hwnd) % p_STORM_WindowHandlers->length];
  MessageHandlerHashTable* mhht;
  MessageHandlerList* mhl;

  while (whl) {
    if (whl->unk_0 == 0x534D5347 && whl->hWnd == d2Hwnd) {
      mhht = whl->msgHandlers;
      if (mhht != NULL && mhht->table != NULL && mhht->length != 0) {
        // 0x201 - WM_something click
        mhl = mhht->table[0x201 % mhht->length];

        if (mhl != NULL) {
          while (mhl) {
            if (mhl->message && mhl->unk_4 < 0xffffffff && mhl->handler == D2CLIENT_CloseNPCTalk) {
              return true;
            }
            mhl = mhl->next;
          }
        }
      }
    }
    whl = whl->next;
  }

  return false;
}

}  // namespace game
}  // namespace d2bs
