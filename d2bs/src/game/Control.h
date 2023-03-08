#pragma once

#include "Structs.h"

namespace d2bs {
namespace game {

Control* FindControl(int localeid, int type, int disabled, int posx, int posy, int sizex, int sizey);
Control* FindControl(const wchar_t* text, int type, int disabled, int posx, int posy, int sizex, int sizey);

}
}
