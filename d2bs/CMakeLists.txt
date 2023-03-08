find_path(DETOURS_INCLUDE_DIRS "detours/detours.h")
find_library(DETOURS_LIBRARY detours REQUIRED)

find_package(spdlog CONFIG REQUIRED)

set(D2BS_SRCS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/api/JSConsole.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/api/JSControl.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/api/JSGlobals.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/api/JSBase.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/api/JSUnit.cpp"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/Console.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/Engine.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/ExceptionHandler.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/Log.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/Settings.cpp"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/game/Control.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/game/Game.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/game/Handlers.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/game/Offset.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/game/Screen.cpp"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/scripting/ScriptEngine.cpp"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CommandLine.cpp"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
  
if(WIN32)
  configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/version.rc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.rc" @ONLY)
  list(APPEND D2BS_SRCS "${CMAKE_CURRENT_BINARY_DIR}/version.rc")
endif()

add_library(d2bs SHARED ${D2BS_SRCS})

target_compile_options(d2bs PRIVATE /W4)

target_compile_definitions(d2bs PRIVATE
  D2BS_VERSION="${D2BS_VERSION}")

target_include_directories(d2bs
  PRIVATE
    ${DETOURS_INCLUDE_DIRS}

    "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(d2bs PRIVATE
  ${DETOURS_LIBRARY}
  spdlog::spdlog
  spdlog::spdlog_header_only

  jspp)

install(TARGETS d2bs
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
