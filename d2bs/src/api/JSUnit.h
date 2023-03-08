#pragma once

#include "JSBase.h"

namespace d2bs {
namespace api {

class JSUnit : public JSBase<JSUnit> {
 public:
  static bool Define(JSContext* cx, JS::HandleObject obj);

};

}  // namespace api
}  // namespace d2bs
