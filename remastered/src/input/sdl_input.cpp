#include "input/sdl_input.h"

namespace geocube::input {

void SdlInput::setBindings(const platform::ControlBindings& bindings)
{
  for (std::size_t i = 0; i < bindings.size(); ++i) {
    if (static_cast<core::Action>(i) == core::Action::Back)
      m_bindings[i] = SDLK_UNKNOWN;
    else
      m_bindings[i] = keyFromName(bindings[i]);
  }
}

SDL_Keycode SdlInput::keyFromName(const std::string& name)
{
  if (name == "Numpad 5") return SDLK_KP_5;
  if (name == "Page Up") return SDLK_PAGEUP;
  if (name == "Page Down") return SDLK_PAGEDOWN;
  if (name == "Comma") return SDLK_COMMA;
  if (name == "Period") return SDLK_PERIOD;
  if (name == "Space") return SDLK_SPACE;
  return SDL_GetKeyFromName(name.c_str());
}

std::string SdlInput::keyName(SDL_Keycode key)
{
  if (key == SDLK_KP_5) return "Numpad 5";
  if (key == SDLK_PAGEUP) return "Page Up";
  if (key == SDLK_PAGEDOWN) return "Page Down";
  if (key == SDLK_COMMA) return "Comma";
  if (key == SDLK_PERIOD) return "Period";
  return SDL_GetKeyName(key);
}

bool SdlInput::actionForKey(SDL_Keycode key, core::Action& action) const
{
  for (std::size_t i = 0; i < m_bindings.size(); ++i) {
    if (m_bindings[i] == key) {
      action = static_cast<core::Action>(i);
      return true;
    }
  }
  return false;
}

void SdlInput::handleEvent(const SDL_Event& event)
{
  if (event.type == SDL_EVENT_TEXT_INPUT) {
    m_textInput += event.text.text;
    return;
  }
  if (event.type == SDL_EVENT_WINDOW_FOCUS_LOST) {
    clear();
    m_textInput.clear();
    return;
  }

  if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
    if (m_capture && event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
      m_capturedKeyName = keyName(event.key.key);
      m_capture = false;
      return;
    }
    if (event.key.key == SDLK_BACKSPACE) {
      if (event.type == SDL_EVENT_KEY_DOWN)
        m_state.press(core::Action::Back);
      else
        m_state.release(core::Action::Back);
      return;
    }
    core::Action action;
    if (!actionForKey(event.key.key, action))
      return;

    if (event.type == SDL_EVENT_KEY_DOWN)
      m_state.press(action);
    else
      m_state.release(action);
    return;
  }

  if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_state.press(core::Action::ThrustForward);
    else if (event.button.button == SDL_BUTTON_RIGHT)
      m_state.press(core::Action::ThrustBackward);
  } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_state.release(core::Action::ThrustForward);
    else if (event.button.button == SDL_BUTTON_RIGHT)
      m_state.release(core::Action::ThrustBackward);
  }
}

void SdlInput::eventCallback(const SDL_Event& event, void* userData)
{
  static_cast<SdlInput*>(userData)->handleEvent(event);
}

} // namespace geocube::input
