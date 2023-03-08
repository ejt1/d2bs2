#pragma once

#include "JSConsole.h"
#include "JSControl.h"
#include "JSGlobals.h"
#include "JSUnit.h"

namespace d2bs {
namespace api {

bool DefineAll(JSContext* cx, JS::HandleObject global) {
  if (!JSGlobal::Define(cx, global)) {
    return false;
  }
  if (!JSConsole::Define(cx, global)) {
    return false;
  }
  if (!JSControl::Define(cx, global)) {
    return false;
  }
  if (!JSUnit::Define(cx, global)) {
    return false;
  }

  return true;
}

}  // namespace api
}  // namespace d2bs
