#!/usr/bin/env bash
set -euo pipefail

script_directory=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
project_directory=$(CDPATH= cd -- "$script_directory/.." && pwd)
renderer="${GEOCUBE_MUSIC_RENDERER:-$project_directory/build/release/geocube3d_music_render}"
soundfont="$project_directory/assets/audio/music/Roland.SC-55.sf2"
music_directory="$project_directory/assets/audio/music"

if [[ ! -x "$renderer" ]]; then
  printf 'Music renderer not found: %s\n' "$renderer" >&2
  printf 'Build it with: cmake --preset release && cmake --build --preset release --target geocube3d_music_render\n' >&2
  exit 1
fi

pids=()
for midi in "$music_directory"/*.mid; do
  name=$(basename "$midi" .mid)
  output="$music_directory/$name.ogg"
  printf 'Rendering %s -> %s\n' "$midi" "$output"
  "$renderer" "$soundfont" "$midi" "$output" &
  pids+=("$!")
done

status=0
for pid in "${pids[@]}"; do
  wait "$pid" || status=1
done
exit "$status"
