#pragma once

#include "JSBase.h"

namespace d2bs {
namespace api {

class JSConsole : public JSBase<JSConsole> {
 public:
  static bool Define(JSContext* cx, JS::HandleObject obj);

  static bool Log(JSContext* cx, unsigned argc, JS::Value* vp);
};

}  // namespace api
}  // namespace d2bs
