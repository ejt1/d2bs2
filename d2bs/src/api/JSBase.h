#pragma once

#include <jspp/jspp.h>

#include <vector>

namespace d2bs {
namespace api {

// Base class for all our JS objects, this follows the CRTP.
template <typename T>
class JSBase {
 protected:
  static JSClassOps js_class_ops_;
  static JSClass js_class_;
  static JSPropertySpec js_props_[];
  static JSFunctionSpec js_methods_[];
  static JSFunctionSpecWithHelp js_functions_[];
};

}  // namespace api
}  // namespace d2bs
