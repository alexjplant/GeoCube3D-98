#!/usr/bin/env bash
set -euo pipefail

if ! command -v brew >/dev/null 2>&1; then
  echo "Homebrew is required: https://brew.sh" >&2
  exit 1
fi

# Make pkg-config discover Homebrew's FluidSynth .pc file on both Intel and
# Apple Silicon installations for commands run by this script.
BREW_PREFIX="$(brew --prefix)"
export PATH="${BREW_PREFIX}/bin:${PATH}"
export PKG_CONFIG_PATH="${BREW_PREFIX}/lib/pkgconfig:${BREW_PREFIX}/share/pkgconfig:${PKG_CONFIG_PATH:-}"

brew install cmake git pkg-config fluid-synth libsndfile

echo "macOS dependencies installed. In a new shell, initialize Homebrew with:"
echo "  eval \"\$(brew shellenv)\""
echo "Then configure from remastered/ with:"
echo "  cmake --preset debug"
