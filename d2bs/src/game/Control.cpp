#include "Control.h"

#include "Constants.h"
#include "Game.h"

namespace d2bs {
namespace game {

Control* FindControl(int localeid, int type, int disabled, int posx, int posy, int sizex, int sizey) {
  if (sGame->State() != GameState::Menu || localeid < 0) {
    return nullptr;
  }

  wchar_t* text = D2LANG_GetLocaleText(static_cast<WORD>(localeid));
  if (!text) {
    return nullptr;
  }

  return FindControl(text, type, disabled, posx, posy, sizex, sizey);
}

Control* FindControl(const wchar_t* text, int type, int disabled, int posx, int posy, int sizex, int sizey) {
  if (sGame->State() != GameState::Menu) {
    return nullptr;
  }

  if (!text && type == -1 && disabled == -1 && posx == -1 && posy == -1 && sizex == -1 && sizey == -1) {
    return *p_D2WIN_FirstControl;
  }

  for (Control* control = *p_D2WIN_FirstControl; control; control = control->pNext) {
    if (type >= 0 && static_cast<int>(control->dwType) != type) {
      continue;
    }

    if (disabled >= 0 && static_cast<int>(control->dwDisabled) == disabled) {
      if (control->dwType == CONTROL_BUTTON && control->unkState == 1) {
        continue;
      }
    } else if (disabled >= 0 && static_cast<int>(control->dwDisabled) != disabled) {
      continue;
    }

    if (posx >= 0 && static_cast<int>(control->dwPosX) != posx) {
      continue;
    }

    if (posy >= 0 && static_cast<int>(control->dwPosY) != posy) {
      continue;
    }

    if (sizex >= 0 && static_cast<int>(control->dwSizeX) != sizex) {
      continue;
    }

    if (sizey >= 0 && static_cast<int>(control->dwSizeY) != sizey) {
      continue;
    }

    if (text && control->dwType == CONTROL_BUTTON) {
      if (!control->wText2) {
        return nullptr;
      }
      if (wcscmp(control->wText2, text) != 0) {
        continue;
      }
    }

    if (text && control->dwType == CONTROL_TEXTBOX) {
      if (control->pFirstText != NULL && control->pFirstText->wText[0] != NULL) {
        if (!control->pFirstText->wText[0]) {
          return NULL;
        }

        if (wcsstr(text, control->pFirstText->wText[0]) == 0) {
          continue;
        }
      } else {
        continue;
      }
    }

    // if we reached the end all conditions came true, hopefully
    return control;
  }

  return nullptr;
}

}  // namespace game
}  // namespace d2bs
