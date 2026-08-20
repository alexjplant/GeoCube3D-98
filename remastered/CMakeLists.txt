cmake_minimum_required(VERSION 3.24)

project(GeoCube3DRemastered VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)
include(GNUInstallDirs)

option(GEOCUBE_FETCH_SDL3 "Fetch SDL3 when no SDL3 CMake package is installed" ON)

if(NOT EMSCRIPTEN)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(FLUIDSYNTH REQUIRED fluidsynth)
  find_package(OpenGL REQUIRED)
endif()

find_package(SDL3 CONFIG QUIET)
set(GEOCUBE_SDL3_FETCHED OFF)

if(NOT SDL3_FOUND)
  if(NOT GEOCUBE_FETCH_SDL3)
    message(FATAL_ERROR
      "SDL3 was not found. Install SDL3 with CMake package files or set "
      "GEOCUBE_FETCH_SDL3=ON.")
  endif()

  # SDL3 release tags are used instead of a moving branch so a clean
  # configure gets the same dependency version on every supported platform.
  set(SDL_TESTS OFF CACHE BOOL "" FORCE)
  set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(SDL_OPENGL ON CACHE BOOL "" FORCE)
  if(EMSCRIPTEN)
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
  else()
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)
  endif()
  if(UNIX AND NOT APPLE)
    # SDL3's configure probe can miss GLVND's split OpenGL/GLX development
    # libraries even though FindOpenGL and the final target can use them.
    set(HAVE_OPENGL TRUE CACHE INTERNAL "" FORCE)
  endif()
  FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.0
    GIT_SHALLOW TRUE
    EXCLUDE_FROM_ALL
  )
  FetchContent_MakeAvailable(SDL3)
  set(GEOCUBE_SDL3_FETCHED ON)
endif()

if(TARGET SDL3::SDL3)
  set(GEOCUBE_SDL3_TARGET SDL3::SDL3)
elseif(TARGET SDL3::SDL3-shared)
  set(GEOCUBE_SDL3_TARGET SDL3::SDL3-shared)
elseif(TARGET SDL3-shared)
  set(GEOCUBE_SDL3_TARGET SDL3-shared)
elseif(TARGET SDL3::SDL3-static)
  set(GEOCUBE_SDL3_TARGET SDL3::SDL3-static)
elseif(TARGET SDL3-static)
  set(GEOCUBE_SDL3_TARGET SDL3-static)
else()
  message(FATAL_ERROR "SDL3 was found but no usable SDL3 CMake target exists")
endif()

add_executable(geocube3d
  src/audio/audio_system.cpp
  src/input/sdl_input.cpp
  src/main.cpp
  src/platform/settings.cpp
  src/render/model_loader.cpp
  src/render/renderer.cpp
  src/render/texture_loader.cpp
  src/platform/frame_clock.cpp
  src/platform/resource_root.cpp
  src/platform/sdl_shell.cpp
  src/ui/ui_controller.cpp
)

add_library(geocube_core STATIC
  src/core/collision.cpp
  src/core/game_world.cpp
  src/core/high_scores.cpp
)

target_compile_features(geocube_core PUBLIC cxx_std_17)
target_include_directories(geocube_core PUBLIC src)

if(NOT EMSCRIPTEN)
  add_executable(geocube3d_core_tests
    tests/core_tests.cpp
  )
  target_link_libraries(geocube3d_core_tests PRIVATE geocube_core)
  add_executable(geocube3d_asset_validate
    tools/asset_validate.cpp
  )
  target_compile_features(geocube3d_asset_validate PRIVATE cxx_std_17)
  add_executable(geocube3d_model_tests
    tests/model_tests.cpp
    src/render/model_loader.cpp
  )
  target_compile_features(geocube3d_model_tests PRIVATE cxx_std_17)
  target_include_directories(geocube3d_model_tests PRIVATE src)

  add_executable(geocube3d_music_render
    tools/render_music.cpp
  )
  target_compile_features(geocube3d_music_render PRIVATE cxx_std_17)
  pkg_check_modules(SNDFILE REQUIRED sndfile)
  target_link_libraries(geocube3d_music_render PRIVATE
    ${FLUIDSYNTH_LIBRARIES} ${SNDFILE_LIBRARIES})
  target_include_directories(geocube3d_music_render PRIVATE
    ${FLUIDSYNTH_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS})
  target_link_directories(geocube3d_music_render PRIVATE
    ${FLUIDSYNTH_LIBRARY_DIRS} ${SNDFILE_LIBRARY_DIRS})

  enable_testing()
  add_test(NAME geocube3d_core COMMAND geocube3d_core_tests)
  add_test(NAME geocube3d_models
           COMMAND geocube3d_model_tests "${CMAKE_CURRENT_SOURCE_DIR}/assets/models")
endif()

target_compile_features(geocube3d PRIVATE cxx_std_17)
target_include_directories(geocube3d PRIVATE src)
target_link_libraries(geocube3d PRIVATE geocube_core ${GEOCUBE_SDL3_TARGET})

if(EMSCRIPTEN)
  target_compile_definitions(geocube3d PRIVATE GEOCUBE_WEB=1)
  target_link_options(geocube3d PRIVATE
    "-sUSE_WEBGL2=1"
    "-sMIN_WEBGL_VERSION=2"
    "-sMAX_WEBGL_VERSION=2"
    "-sALLOW_MEMORY_GROWTH=1"
    "-sFORCE_FILESYSTEM=1"
    "-sGL_ENABLE_GET_PROC_ADDRESS=1"
    "-sEXIT_RUNTIME=0"
    "-sENVIRONMENT=web"
    "--preload-file=${CMAKE_CURRENT_SOURCE_DIR}/assets@/assets"
    "--exclude-file=${CMAKE_CURRENT_SOURCE_DIR}/assets/audio/music/Roland.SC-55.sf2"
    "--exclude-file=${CMAKE_CURRENT_SOURCE_DIR}/assets/audio/music/music.mid"
    "--exclude-file=${CMAKE_CURRENT_SOURCE_DIR}/assets/audio/music/synth.mid"
    "--exclude-file=${CMAKE_CURRENT_SOURCE_DIR}/assets/audio/music/reggae.mid"
    "--exclude-file=${CMAKE_CURRENT_SOURCE_DIR}/assets/audio/music/spin.mid"
    "--exclude-file=${CMAKE_CURRENT_SOURCE_DIR}/assets/audio/music/funk.mid"
    "--exclude-file=${CMAKE_CURRENT_SOURCE_DIR}/assets/audio/music/snare.mid"
    "--exclude-file=${CMAKE_CURRENT_SOURCE_DIR}/assets/audio/music/wierd.mid"
    "--shell-file=${CMAKE_CURRENT_SOURCE_DIR}/web/shell.html"
  )
  set_target_properties(geocube3d PROPERTIES SUFFIX ".html")
else()
  target_link_libraries(geocube3d PRIVATE OpenGL::GL ${FLUIDSYNTH_LIBRARIES})
  target_include_directories(geocube3d PRIVATE ${FLUIDSYNTH_INCLUDE_DIRS})
  target_link_directories(geocube3d PRIVATE ${FLUIDSYNTH_LIBRARY_DIRS})
endif()

if(UNIX AND NOT APPLE)
  set_target_properties(geocube3d PROPERTIES
    INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
  )
elseif(APPLE)
  set_target_properties(geocube3d PROPERTIES
    INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}"
  )
endif()

if(MSVC)
  target_compile_options(geocube_core PRIVATE /W4 /permissive-)
  target_compile_options(geocube3d_core_tests PRIVATE /W4 /permissive-)
  target_compile_options(geocube3d_asset_validate PRIVATE /W4 /permissive-)
  target_compile_options(geocube3d_model_tests PRIVATE /W4 /permissive-)
  target_compile_options(geocube3d PRIVATE /W4 /permissive-)
else()
  target_compile_options(geocube_core PRIVATE -Wall -Wextra -Wpedantic)
  target_compile_options(geocube3d PRIVATE -Wall -Wextra -Wpedantic)
  if(NOT EMSCRIPTEN)
    target_compile_options(geocube3d_core_tests PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(geocube3d_asset_validate PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(geocube3d_model_tests PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(geocube3d_music_render PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endif()

# Keep the build tree runnable without requiring the shell's current directory.
if(NOT EMSCRIPTEN)
  file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

if(NOT EMSCRIPTEN)
  add_test(NAME geocube3d_headless COMMAND geocube3d --headless)
  add_test(NAME geocube3d_assets
           COMMAND geocube3d_asset_validate "${CMAKE_CURRENT_BINARY_DIR}/assets")
endif()

if(NOT EMSCRIPTEN)
  install(TARGETS geocube3d
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION .
  )
  install(TARGETS geocube3d_asset_validate
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets/" DESTINATION assets)

  set(CPACK_PACKAGE_NAME "GeoCube3DRemastered")
  set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "GeoCube 3D single-player remastered port")
  include(CPack)
endif()

if(GEOCUBE_SDL3_FETCHED AND NOT EMSCRIPTEN)
  if(WIN32)
    install(FILES "$<TARGET_FILE:${GEOCUBE_SDL3_TARGET}>"
      DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
  elseif(APPLE)
    install(FILES "$<TARGET_FILE:${GEOCUBE_SDL3_TARGET}>"
      DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
  else()
    install(FILES
      "$<TARGET_FILE:${GEOCUBE_SDL3_TARGET}>"
      "$<TARGET_SONAME_FILE:${GEOCUBE_SDL3_TARGET}>"
      DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
  endif()
endif()
