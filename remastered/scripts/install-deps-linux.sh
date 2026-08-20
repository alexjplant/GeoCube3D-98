#!/usr/bin/env bash
set -euo pipefail

if ! command -v apt-get >/dev/null 2>&1; then
  echo "This script supports Debian/Ubuntu systems with apt-get." >&2
  exit 1
fi

if [[ "${EUID}" -eq 0 ]]; then
  APT=(apt-get)
elif command -v sudo >/dev/null 2>&1; then
  APT=(sudo apt-get)
else
  echo "Run as root or install sudo before running this script." >&2
  exit 1
fi

"${APT[@]}" update
"${APT[@]}" install -y \
  build-essential \
  cmake \
  git \
  libfluidsynth-dev \
  libgl1-mesa-dev \
  libsndfile1-dev \
  pkg-config \
  xvfb

echo "Linux dependencies installed. Configure from remastered/ with:"
echo "  cmake --preset debug"
