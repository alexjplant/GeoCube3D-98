#ifndef GEOCUBE_AUDIO_SYSTEM_H
#define GEOCUBE_AUDIO_SYSTEM_H

#include <SDL3/SDL.h>
#ifndef __EMSCRIPTEN__
#include <fluidsynth/types.h>
#endif

#include <array>
#include <cstddef>
#include <filesystem>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

namespace geocube::audio {

enum class Effect : std::size_t {
  Fire,
  Laser,
  Thrust,
  Shield,
  Hit,
  PlayerHit,
  ShieldHit,
  Jump,
  Land,
  Count,
};

class AudioSystem final {
public:
  AudioSystem() = default;
  ~AudioSystem();

  AudioSystem(const AudioSystem&) = delete;
  AudioSystem& operator=(const AudioSystem&) = delete;

  bool initialize(const std::filesystem::path& root);
  void shutdown();

  bool available() const { return m_stream != nullptr; }
  const std::string& error() const { return m_error; }

  void playEffect(Effect effect, bool loop = false);
  void stopEffect(Effect effect);
  void playMusic(std::string_view legacyName);
  void stopMusic();

private:
  struct Sample {
    std::vector<float> stereo;
  };

  struct Voice {
    const Sample* sample = nullptr;
    std::size_t frame = 0;
    Effect effect = Effect::Fire;
    bool music = false;
    bool loop = false;
  };

  static void audioCallback(void* userData, SDL_AudioStream* stream,
                            int additionalAmount, int totalAmount);
  bool loadEffect(Effect effect, const std::filesystem::path& path);
  bool loadWav(const std::filesystem::path& path, Sample& sample);
  void mix(float* output, std::size_t frames);
#ifndef __EMSCRIPTEN__
  void stopMidiLocked();
#endif
  void setError(std::string message);

  SDL_AudioStream* m_stream = nullptr;
#ifndef __EMSCRIPTEN__
  fluid_settings_t* m_fluidSettings = nullptr;
  fluid_synth_t* m_synth = nullptr;
  fluid_player_t* m_player = nullptr;
  int m_soundFontId = -1;
#endif
  std::filesystem::path m_musicDirectory;
  std::array<Sample, static_cast<std::size_t>(Effect::Count)> m_effects;
  std::vector<Voice> m_voices;
  std::mutex m_mutex;
  std::string m_error;
};

} // namespace geocube::audio

#endif
