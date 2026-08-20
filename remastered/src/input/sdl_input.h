#ifndef GEOCUBE_INPUT_SDL_INPUT_H
#define GEOCUBE_INPUT_SDL_INPUT_H

#include "core/game_world.h"

#include <SDL3/SDL.h>

#include <array>
#include <string>

#include "platform/settings.h"

namespace geocube::input {

class SdlInput final {
public:
  void beginFrame()
  {
    m_state.clearPressed();
    m_textInput.clear();
    m_capturedKeyName.clear();
  }
  void handleEvent(const SDL_Event& event);
  void clear() { m_state.reset(); }
  void setBindings(const platform::ControlBindings& bindings);
  void startKeyCapture() { m_capture = true; }

  const core::InputState& state() const { return m_state; }
  const std::string& textInput() const { return m_textInput; }
  const std::string& capturedKeyName() const { return m_capturedKeyName; }

  static void eventCallback(const SDL_Event& event, void* userData);

private:
  bool actionForKey(SDL_Keycode key, core::Action& action) const;
  static SDL_Keycode keyFromName(const std::string& name);
  static std::string keyName(SDL_Keycode key);
  core::InputState m_state;
  std::string m_textInput;
  std::array<SDL_Keycode, core::actionIndex(core::Action::Count)>
      m_bindings{};
  bool m_capture = false;
  std::string m_capturedKeyName;
};

} // namespace geocube::input

#endif
