#include <fluidsynth.h>
#include <sndfile.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace {

constexpr int kSampleRate = 48000;
constexpr int kChannels = 2;
constexpr int kBlockFrames = 480;
constexpr int kTailFrames = kSampleRate / 2;

bool writeBlock(fluid_synth_t* synth, SNDFILE* output, int frames)
{
  std::vector<float> left(static_cast<std::size_t>(frames));
  std::vector<float> right(static_cast<std::size_t>(frames));
  std::vector<float> interleaved(static_cast<std::size_t>(frames) * kChannels);
  if (fluid_synth_write_float(synth, frames, left.data(), 0, 1, right.data(),
                              0, 1) != FLUID_OK)
    return false;
  for (int frame = 0; frame < frames; ++frame) {
    interleaved[static_cast<std::size_t>(frame) * kChannels] = left[frame];
    interleaved[static_cast<std::size_t>(frame) * kChannels + 1] = right[frame];
  }
  return sf_writef_float(output, interleaved.data(), frames) == frames;
}

} // namespace

int main(int argc, char** argv)
{
  if (argc != 4) {
    std::cerr << "Usage: geocube3d_music_render <soundfont> <midi> <ogg>\n";
    return 2;
  }

  fluid_settings_t* settings = new_fluid_settings();
  if (!settings)
    return 1;
  fluid_settings_setnum(settings, "synth.sample-rate", kSampleRate);
  fluid_settings_setnum(settings, "synth.gain", 0.5);
  fluid_synth_t* synth = new_fluid_synth(settings);
  if (!synth) {
    delete_fluid_settings(settings);
    return 1;
  }
  const int soundFont = fluid_synth_sfload(synth, argv[1], 1);
  if (soundFont == FLUID_FAILED) {
    std::cerr << "Could not load SoundFont: " << argv[1] << '\n';
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
    return 1;
  }

  SF_INFO info{};
  info.samplerate = kSampleRate;
  info.channels = kChannels;
  info.format = SF_FORMAT_OGG | SF_FORMAT_VORBIS;
  SNDFILE* output = sf_open(argv[3], SFM_WRITE, &info);
  if (!output) {
    std::cerr << "Could not open OGG output: " << sf_strerror(nullptr) << '\n';
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
    return 1;
  }

  fluid_player_t* player = new_fluid_player(synth);
  const bool ready = player && fluid_player_add(player, argv[2]) != FLUID_FAILED &&
                     fluid_player_set_loop(player, 0) != FLUID_FAILED &&
                     fluid_player_play(player) != FLUID_FAILED;
  if (!ready) {
    std::cerr << "Could not play MIDI: " << argv[2] << '\n';
    if (player)
      delete_fluid_player(player);
    sf_close(output);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
    return 1;
  }

  bool started = false;
  bool success = true;
  for (;;) {
    const int status = fluid_player_get_status(player);
    if (status == FLUID_PLAYER_PLAYING)
      started = true;
    if (started && status == FLUID_PLAYER_DONE)
      break;
    success = writeBlock(synth, output, kBlockFrames) && success;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  fluid_player_join(player);
  for (int remaining = kTailFrames; remaining > 0 && success;
       remaining -= kBlockFrames)
    success = writeBlock(synth, output, std::min(kBlockFrames, remaining));

  delete_fluid_player(player);
  sf_close(output);
  fluid_synth_sfunload(synth, soundFont, 1);
  delete_fluid_synth(synth);
  delete_fluid_settings(settings);
  if (!success) {
    std::cerr << "Could not write OGG output: " << argv[3] << '\n';
    return 1;
  }
  return 0;
}
