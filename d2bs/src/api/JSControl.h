#pragma once

#include "JSBase.h"

#include "game/Control.h"

namespace d2bs {
namespace api {

class JSControl : public JSBase<JSControl> {
  JSControl(Control* control);

 private:
  Control* control;
  DWORD type;
  DWORD x;
  DWORD y;
  DWORD sizex;
  DWORD sizey;

  // below is anything related to defining this object
 public:
  static bool Define(JSContext* cx, JS::HandleObject obj);

  static void Finalize(JSFreeOp* fop, JSObject* obj);

  static bool getControl(JSContext* cx, unsigned argc, JS::Value* vp);
};

}  // namespace api
}  // namespace d2bs
