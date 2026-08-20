#include "audio/audio_system.h"

#ifndef __EMSCRIPTEN__
#include <fluidsynth.h>
#include <fluidsynth/midi.h>
#include <fluidsynth/settings.h>
#include <fluidsynth/synth.h>
#else
#include <emscripten/emscripten.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <utility>

namespace geocube::audio {
namespace {

constexpr int kOutputFrequency = 48000;
constexpr int kOutputChannels = 2;

#ifdef __EMSCRIPTEN__
EM_JS(void, playBrowserMusic, (const char* legacyName), {
  const name = UTF8ToString(legacyName).replace(/\.mid$/i, ".ogg");
  const path = "/assets/audio/music/" + name;
  let audio = window.geocubeMusicAudio;
  if (!audio) {
    audio = new Audio();
    audio.preload = "auto";
    audio.volume = 0.65;
    window.geocubeMusicAudio = audio;
  }
  // The page's audio-unlock prompt may create this element first.
  audio.loop = true;
  audio.pause();
  audio.currentTime = 0;
  if (window.geocubeMusicObjectUrl)
    URL.revokeObjectURL(window.geocubeMusicObjectUrl);
  try {
    const filesystem = typeof FS !== "undefined" ? FS : Module.FS;
    const bytes = filesystem.readFile(path);
    window.geocubeMusicObjectUrl = URL.createObjectURL(
        new Blob([bytes], {type: "audio/ogg"}));
    audio.src = window.geocubeMusicObjectUrl;
  } catch (error) {
    console.warn("GeoCube music asset unavailable", path, error);
    return;
  }
  audio.muted = false;
  audio.currentTime = 0;
  const attempt = audio.play();
  if (attempt && attempt.then)
    attempt.then(() => { window.geocubeMusicNeedsUnlock = false; })
      .catch((error) => {
        window.geocubeMusicNeedsUnlock = true;
        console.warn("GeoCube music playback was blocked", error);
      });
});

EM_JS(void, stopBrowserMusic, (), {
  const audio = window.geocubeMusicAudio;
  if (audio) {
    audio.pause();
    audio.currentTime = 0;
  }
  if (window.geocubeMusicObjectUrl) {
    URL.revokeObjectURL(window.geocubeMusicObjectUrl);
    window.geocubeMusicObjectUrl = null;
  }
});
#endif

std::size_t effectIndex(Effect effect)
{
  return static_cast<std::size_t>(effect);
}

const char* effectName(Effect effect)
{
  switch (effect) {
  case Effect::Fire:
    return "fire.wav";
  case Effect::Laser:
    return "laser.wav";
  case Effect::Thrust:
    return "thrust.wav";
  case Effect::Shield:
    return "shield.wav";
  case Effect::Hit:
    return "hit.wav";
  case Effect::PlayerHit:
    return "plyrhit.wav";
  case Effect::ShieldHit:
    return "shldhit.wav";
  case Effect::Jump:
    return "jump.wav";
  case Effect::Land:
    return "land.wav";
  case Effect::Count:
    return "";
  }
  return "";
}

} // namespace

AudioSystem::~AudioSystem()
{
  shutdown();
}

void AudioSystem::setError(std::string message)
{
  if (m_error.empty())
    m_error = std::move(message);
}

bool AudioSystem::initialize(const std::filesystem::path& root)
{
  shutdown();
  m_error.clear();
  if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
    setError(std::string("SDL audio initialization failed: ") + SDL_GetError());
    return false;
  }

  SDL_AudioSpec outputSpec{};
  outputSpec.format = SDL_AUDIO_F32;
  outputSpec.channels = kOutputChannels;
  outputSpec.freq = kOutputFrequency;
  m_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
                                       &outputSpec, audioCallback, this);
  if (!m_stream) {
    setError(std::string("audio device unavailable: ") + SDL_GetError());
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    return false;
  }

  for (std::size_t i = 0; i < effectIndex(Effect::Count); ++i) {
    const Effect effect = static_cast<Effect>(i);
    loadEffect(effect, root / "audio/effects" / effectName(effect));
  }
  m_musicDirectory = root / "audio/music";
#ifdef __EMSCRIPTEN__
  SDL_ResumeAudioStreamDevice(m_stream);
  return true;
#else
  m_fluidSettings = new_fluid_settings();
  if (!m_fluidSettings) {
    setError("FluidSynth settings allocation failed");
    return true;
  }
  fluid_settings_setnum(m_fluidSettings, "synth.sample-rate",
                        kOutputFrequency);
  fluid_settings_setnum(m_fluidSettings, "synth.gain", 0.5);
  m_synth = new_fluid_synth(m_fluidSettings);
  if (!m_synth) {
    setError("FluidSynth synthesizer allocation failed");
    return true;
  }
  m_soundFontId = fluid_synth_sfload(
      m_synth, (m_musicDirectory / "Roland.SC-55.sf2").string().c_str(), 1);
  if (m_soundFontId == FLUID_FAILED)
    setError(std::string("FluidSynth SoundFont load failed: ") +
             (m_musicDirectory / "Roland.SC-55.sf2").string());
  SDL_ResumeAudioStreamDevice(m_stream);
  return true;
#endif
}

bool AudioSystem::loadEffect(Effect effect, const std::filesystem::path& path)
{
  Sample& sample = m_effects[effectIndex(effect)];
  if (!loadWav(path, sample)) {
    // Optional audio must not prevent the game from starting.
    return false;
  }
  return true;
}

bool AudioSystem::loadWav(const std::filesystem::path& path, Sample& sample)
{
  SDL_AudioSpec sourceSpec{};
  Uint8* sourceData = nullptr;
  Uint32 sourceLength = 0;
  if (!SDL_LoadWAV(path.string().c_str(), &sourceSpec, &sourceData,
                   &sourceLength))
    return false;

  SDL_AudioSpec outputSpec{};
  outputSpec.format = SDL_AUDIO_F32;
  outputSpec.channels = kOutputChannels;
  outputSpec.freq = kOutputFrequency;
  SDL_AudioStream* converter =
      SDL_CreateAudioStream(&sourceSpec, &outputSpec);
  if (!converter) {
    SDL_free(sourceData);
    return false;
  }

  bool success = SDL_PutAudioStreamData(converter, sourceData,
                                        static_cast<int>(sourceLength)) &&
                 SDL_FlushAudioStream(converter);
  SDL_free(sourceData);
  if (!success) {
    SDL_DestroyAudioStream(converter);
    return false;
  }

  const int convertedLength = SDL_GetAudioStreamAvailable(converter);
  if (convertedLength <= 0) {
    SDL_DestroyAudioStream(converter);
    return false;
  }
  std::vector<std::uint8_t> converted(static_cast<std::size_t>(convertedLength));
  const int received = SDL_GetAudioStreamData(converter, converted.data(),
                                              convertedLength);
  SDL_DestroyAudioStream(converter);
  if (received != convertedLength)
    return false;

  sample.stereo.resize(static_cast<std::size_t>(received) / sizeof(float));
  std::memcpy(sample.stereo.data(), converted.data(),
              static_cast<std::size_t>(received));
  return !sample.stereo.empty();
}

void AudioSystem::playEffect(Effect effect, bool loop)
{
  if (!m_stream)
    return;
  std::lock_guard<std::mutex> lock(m_mutex);
  const Sample& sample = m_effects[effectIndex(effect)];
  if (sample.stereo.empty())
    return;
  if (loop) {
    for (const Voice& voice : m_voices) {
      if (!voice.music && voice.effect == effect && voice.loop)
        return;
    }
  }
  m_voices.push_back({&sample, 0, effect, false, loop});
}

void AudioSystem::stopEffect(Effect effect)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  m_voices.erase(
      std::remove_if(m_voices.begin(), m_voices.end(),
                     [effect](const Voice& voice) {
                       return !voice.music && voice.effect == effect;
                     }),
      m_voices.end());
}

void AudioSystem::playMusic(std::string_view legacyName)
{
#ifdef __EMSCRIPTEN__
  if (!legacyName.empty()) {
    const std::string name(legacyName);
    playBrowserMusic(name.c_str());
  }
#else
  if (!m_stream || !m_synth || m_soundFontId == FLUID_FAILED)
    return;
  std::lock_guard<std::mutex> lock(m_mutex);
  stopMidiLocked();
  // Stopping the MIDI player does not necessarily release sustained voices.
  // Reset every channel before scheduling the next track.
  fluid_synth_system_reset(m_synth);
  for (int channel = 0; channel < 16; ++channel) {
    fluid_synth_all_notes_off(m_synth, channel);
    fluid_synth_all_sounds_off(m_synth, channel);
  }
  m_player = new_fluid_player(m_synth);
  if (!m_player ||
      fluid_player_add(m_player,
                       (m_musicDirectory / std::string(legacyName)).string()
                           .c_str()) == FLUID_FAILED ||
      fluid_player_set_loop(m_player, -1) == FLUID_FAILED ||
      fluid_player_play(m_player) == FLUID_FAILED) {
    setError(std::string("FluidSynth MIDI playback failed: ") +
             std::string(legacyName));
    if (m_player) {
      delete_fluid_player(m_player);
      m_player = nullptr;
    }
  }
#endif
}

void AudioSystem::stopMusic()
{
#ifdef __EMSCRIPTEN__
  stopBrowserMusic();
#else
  std::lock_guard<std::mutex> lock(m_mutex);
  stopMidiLocked();
  if (m_synth) {
    fluid_synth_system_reset(m_synth);
    for (int channel = 0; channel < 16; ++channel) {
      fluid_synth_all_notes_off(m_synth, channel);
      fluid_synth_all_sounds_off(m_synth, channel);
    }
  }
#endif
}

#ifndef __EMSCRIPTEN__
void AudioSystem::stopMidiLocked()
{
  if (!m_player)
    return;
  fluid_player_stop(m_player);
  fluid_player_join(m_player);
  delete_fluid_player(m_player);
  m_player = nullptr;
}
#endif

void AudioSystem::audioCallback(void* userData, SDL_AudioStream* stream,
                                int additionalAmount, int totalAmount)
{
  (void)stream;
  (void)totalAmount;
  auto* audio = static_cast<AudioSystem*>(userData);
  const std::size_t sampleCount =
      static_cast<std::size_t>(std::max(additionalAmount, 0)) / sizeof(float);
  std::vector<float> output(sampleCount, 0.0f);
  audio->mix(output.data(), sampleCount / kOutputChannels);
  SDL_PutAudioStreamData(stream, output.data(), additionalAmount);
}

void AudioSystem::mix(float* output, std::size_t frames)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  std::vector<float> left(frames, 0.0f);
  std::vector<float> right(frames, 0.0f);
#ifndef __EMSCRIPTEN__
  if (m_synth && fluid_synth_write_float(m_synth, static_cast<int>(frames),
                                         left.data(), 0, 1, right.data(), 0,
                                         1) != FLUID_OK) {
    std::fill(left.begin(), left.end(), 0.0f);
    std::fill(right.begin(), right.end(), 0.0f);
  }
#endif
  for (std::size_t frame = 0; frame < frames; ++frame) {
    output[frame * 2] = left[frame];
    output[frame * 2 + 1] = right[frame];
  }
  for (Voice& voice : m_voices) {
    const std::size_t sampleFrames = voice.sample->stereo.size() / 2;
    for (std::size_t frame = 0; frame < frames; ++frame) {
      if (voice.frame >= sampleFrames) {
        if (!voice.loop)
          break;
        voice.frame = 0;
      }
      output[frame * 2] += voice.sample->stereo[voice.frame * 2];
      output[frame * 2 + 1] += voice.sample->stereo[voice.frame * 2 + 1];
      ++voice.frame;
    }
  }

  for (std::size_t index = 0; index < frames * 2; ++index)
    output[index] = std::clamp(output[index], -1.0f, 1.0f);

  m_voices.erase(
      std::remove_if(m_voices.begin(), m_voices.end(),
                     [](const Voice& voice) {
                       return !voice.loop &&
                              voice.frame >= voice.sample->stereo.size() / 2;
                     }),
      m_voices.end());
}

void AudioSystem::shutdown()
{
#ifdef __EMSCRIPTEN__
  stopBrowserMusic();
#endif
  if (m_stream) {
    SDL_DestroyAudioStream(m_stream);
    m_stream = nullptr;
  }
#ifndef __EMSCRIPTEN__
  if (m_player) {
    std::lock_guard<std::mutex> lock(m_mutex);
    stopMidiLocked();
  }
  if (m_synth) {
    delete_fluid_synth(m_synth);
    m_synth = nullptr;
  }
  if (m_fluidSettings) {
    delete_fluid_settings(m_fluidSettings);
    m_fluidSettings = nullptr;
  }
  m_soundFontId = -1;
#endif
  m_voices.clear();
  for (Sample& sample : m_effects)
    sample.stereo.clear();
  SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

} // namespace geocube::audio
