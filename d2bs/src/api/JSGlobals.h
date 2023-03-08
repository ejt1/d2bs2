#pragma once

#include "JSBase.h"

namespace d2bs {
namespace api {

class JSGlobal : public JSBase<JSGlobal> {
 public:
  static bool Define(JSContext* cx, JS::HandleObject obj);

  static bool Include(JSContext* cx, unsigned argc, JS::Value* vp);
  static bool isIncluded(JSContext* cx, unsigned argc, JS::Value* vp);
  static bool Print(JSContext* cx, unsigned argc, JS::Value* vp);
  static bool Rand(JSContext* cx, unsigned argc, JS::Value* vp);
  static bool Say(JSContext* cx, unsigned argc, JS::Value* vp);
};

}  // namespace api
}  // namespace d2bs
