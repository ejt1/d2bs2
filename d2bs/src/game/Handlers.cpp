#include "Handlers.h"

#include "core/Log.h"
#include "core/Engine.h"

void GameDraw(void) {
  d2bs::s_engine->HandleUpdate();

}

void GameDrawOOG(void) {
  d2bs::s_engine->HandleUpdate();
}
