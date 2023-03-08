#pragma once

#include <js/Initialization.h>

namespace jspp {

class init_guard {
 public:
  init_guard() {
    if (!JS_IsInitialized()) {
      JS_Init();
    }
  }

  ~init_guard() {
    if (JS_IsInitialized()) {
      JS_ShutDown();
    }
  }
};

}  // namespace jspp
