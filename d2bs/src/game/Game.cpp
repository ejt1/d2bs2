#include "Game.h"

#include <format>
#include "utils/Localization.h"

namespace d2bs {

Game* Game::GetInstance() {
  static Game instance;
  return &instance;
}

void Game::Update() {
  UnitAny* player = D2CLIENT_GetPlayerUnit();
  Control* firstControl = *p_D2WIN_FirstControl;

  if (player && !firstControl) {
    if (player && player->pUpdateUnit) {
      state_ = GameState::Busy;
    } else if (player->pInventory && player->pPath &&
               // player->pPath->xPos &&
               player->pPath->pRoom1 && player->pPath->pRoom1->pRoom2 && player->pPath->pRoom1->pRoom2->pLevel &&
               player->pPath->pRoom1->pRoom2->pLevel->dwLevelNo) {
      state_ = GameState::InGame;
    } else {
      state_ = GameState::Busy;
    }
  } else if (!player && firstControl) {
    state_ = GameState::Menu;
  } else if (!player && !firstControl) {
    state_ = GameState::Null;
  }
}

void __declspec(naked) __fastcall Say_ASM(MSG*) {
  __asm
  {
		POP EAX
		PUSH ECX
		PUSH EAX
		PUSH EBP
		MOV EBP, ESP
		SUB ESP,0x110
		PUSH EBX
		PUSH ESI
		PUSH EDI
		MOV EBX, [EBP+8]
    // MOV EBP, [D2LANG_Say_II]
		JMP D2CLIENT_Say_I
  }
}

void Game::SayFormatted(const std::string& str) {
  std::wstring ustr = util::UTF8ToWide(str);

  if (*p_D2CLIENT_PlayerUnit) {
    memcpy(p_D2CLIENT_ChatMsg, ustr.c_str(), (ustr.length() + 1) * sizeof(wchar_t));

    MSG msg {};
    msg.hwnd = D2GFX_GetHwnd();
    msg.message = WM_CHAR;
    msg.wParam = VK_RETURN;
    msg.lParam = 0x11C001;
    msg.time = NULL;
    msg.pt.x = 0x79;
    msg.pt.y = 0x01;
    Say_ASM(&msg);
  }
}

GameState& Game::State() {
  return state_;
}

}  // namespace d2bs
