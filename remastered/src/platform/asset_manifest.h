#ifndef GEOCUBE_PLATFORM_ASSET_MANIFEST_H
#define GEOCUBE_PLATFORM_ASSET_MANIFEST_H

#include <array>
#include <string_view>

namespace geocube::platform {

enum class AssetKind {
  Mesh,
  Obj,
  Mtl,
  Bitmap,
  Wave,
  Midi,
  Ogg,
  SoundFont,
};

struct AssetRequirement {
  std::string_view path;
  AssetKind kind;
  bool native;
  bool web;
  bool startup;
};

inline constexpr std::array<AssetRequirement, 42> kAssetManifest{{
    {"models/ship.mesh", AssetKind::Mesh, true, true, true},
    {"models/cube.mesh", AssetKind::Mesh, true, true, true},
    {"models/ship.obj", AssetKind::Obj, true, true, false},
    {"models/ship.mtl", AssetKind::Mtl, true, true, false},
    {"models/cube.obj", AssetKind::Obj, true, true, false},
    {"models/cube.mtl", AssetKind::Mtl, true, true, false},
    {"models/jack.obj", AssetKind::Obj, true, true, false},
    {"models/jack.mtl", AssetKind::Mtl, true, true, false},
    {"models/portal.obj", AssetKind::Obj, true, true, false},
    {"models/portal.mtl", AssetKind::Mtl, true, true, false},
    {"models/sphere3.obj", AssetKind::Obj, true, true, false},
    {"models/sphere3.mtl", AssetKind::Mtl, true, true, false},
    {"models/pasta.obj", AssetKind::Obj, true, true, false},
    {"models/pasta.mtl", AssetKind::Mtl, true, true, false},
    {"textures/canvas.bmp", AssetKind::Bitmap, true, true, true},
    {"textures/bullet.bmp", AssetKind::Bitmap, true, true, true},
    {"textures/explode.bmp", AssetKind::Bitmap, true, true, false},
    {"textures/logo.bmp", AssetKind::Bitmap, true, true, false},
    {"audio/effects/fire.wav", AssetKind::Wave, true, true, true},
    {"audio/effects/laser.wav", AssetKind::Wave, true, true, false},
    {"audio/effects/thrust.wav", AssetKind::Wave, true, true, false},
    {"audio/effects/shield.wav", AssetKind::Wave, true, true, false},
    {"audio/effects/hit.wav", AssetKind::Wave, true, true, false},
    {"audio/effects/plyrhit.wav", AssetKind::Wave, true, true, false},
    {"audio/effects/shldhit.wav", AssetKind::Wave, true, true, false},
    {"audio/effects/jump.wav", AssetKind::Wave, true, true, false},
    {"audio/effects/land.wav", AssetKind::Wave, true, true, false},
    {"audio/music/music.mid", AssetKind::Midi, true, false, true},
    {"audio/music/synth.mid", AssetKind::Midi, true, false, false},
    {"audio/music/reggae.mid", AssetKind::Midi, true, false, false},
    {"audio/music/spin.mid", AssetKind::Midi, true, false, false},
    {"audio/music/funk.mid", AssetKind::Midi, true, false, false},
    {"audio/music/snare.mid", AssetKind::Midi, true, false, false},
    {"audio/music/wierd.mid", AssetKind::Midi, true, false, false},
    {"audio/music/Roland.SC-55.sf2", AssetKind::SoundFont, true, false, true},
    {"audio/music/music.ogg", AssetKind::Ogg, false, true, true},
    {"audio/music/synth.ogg", AssetKind::Ogg, false, true, true},
    {"audio/music/reggae.ogg", AssetKind::Ogg, false, true, true},
    {"audio/music/spin.ogg", AssetKind::Ogg, false, true, true},
    {"audio/music/funk.ogg", AssetKind::Ogg, false, true, true},
    {"audio/music/snare.ogg", AssetKind::Ogg, false, true, true},
    {"audio/music/wierd.ogg", AssetKind::Ogg, false, true, true},
}};

} // namespace geocube::platform

#endif
