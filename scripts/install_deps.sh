#!/usr/bin/env bash
set -euo pipefail

DEPS_DIR="${HOME}/goinfre/nibbler_deps"
BUILD_DIR="${DEPS_DIR}/_build"
NPROC=$(nproc 2>/dev/null || echo 4)

CMAKE_COMPAT=(-DCMAKE_POLICY_VERSION_MINIMUM=3.5)

mkdir -p "${DEPS_DIR}/include" "${DEPS_DIR}/lib" "${BUILD_DIR}"

echo "==> Installing dependencies into: ${DEPS_DIR}"

# ---------------------------------------------------------------------------
# udev : fichiers de dev absents sur les machines 42 (pas de sudo).
# On pose le header stable de systemd + un symlink "libudev.so" pointant vers
# la lib runtime (déjà présente sur le système via systemd-libs).
# SDL2 et raylib n'en ont pas besoin au build (SDL charge udev via dlopen).
# ---------------------------------------------------------------------------
if [ ! -f "${DEPS_DIR}/include/libudev.h" ]; then
    echo "==> Fetching libudev.h"
    curl -fsSL -o "${DEPS_DIR}/include/libudev.h" \
        "https://raw.githubusercontent.com/systemd/systemd/v255/src/libudev/libudev.h" \
    || wget -qO "${DEPS_DIR}/include/libudev.h" \
        "https://raw.githubusercontent.com/systemd/systemd/v255/src/libudev/libudev.h"
fi

UDEV_RUNTIME_LIB=""
for candidate in \
    /usr/lib64/libudev.so.1 \
    /lib64/libudev.so.1 \
    /usr/lib/x86_64-linux-gnu/libudev.so.1 \
    /lib/x86_64-linux-gnu/libudev.so.1; do
    if [ -e "${candidate}" ]; then
        UDEV_RUNTIME_LIB="${candidate}"
        break
    fi
done

if [ -z "${UDEV_RUNTIME_LIB}" ]; then
    echo "!! ATTENTION : libudev.so.1 introuvable sur cette machine." >&2
    echo "!! SFML (Window) ne pourra pas se construire ni s'exécuter." >&2
else
    echo "==> udev runtime found: ${UDEV_RUNTIME_LIB}"
    ln -sfn "${UDEV_RUNTIME_LIB}" "${DEPS_DIR}/lib/libudev.so"
fi

# ---------------------------------------------------------------------------
# libogg (dépendance de libvorbis et libFLAC, requises par SFML Audio)
# ---------------------------------------------------------------------------
if [ ! -f "${DEPS_DIR}/lib/libogg.so" ]; then
    echo "==> Building libogg"
    cd "${BUILD_DIR}"
    if [ ! -d "ogg" ]; then
        git clone --branch v1.3.5 --depth 1 https://github.com/xiph/ogg.git ogg
    fi
    cmake -S ogg -B ogg/build \
        -DCMAKE_INSTALL_PREFIX="${DEPS_DIR}" \
        -DCMAKE_INSTALL_LIBDIR=lib \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=ON \
        -DBUILD_TESTING=OFF \
        "${CMAKE_COMPAT[@]}"
    cmake --build ogg/build -j "${NPROC}"
    cmake --install ogg/build
else
    echo "==> libogg already installed, skipping"
fi

# ---------------------------------------------------------------------------
# libvorbis (requise par SFML Audio)
# ---------------------------------------------------------------------------
if [ ! -f "${DEPS_DIR}/lib/libvorbis.so" ]; then
    echo "==> Building libvorbis"
    cd "${BUILD_DIR}"
    if [ ! -d "vorbis" ]; then
        git clone --branch v1.3.7 --depth 1 https://github.com/xiph/vorbis.git vorbis
    fi
    cmake -S vorbis -B vorbis/build \
        -DCMAKE_INSTALL_PREFIX="${DEPS_DIR}" \
        -DCMAKE_INSTALL_LIBDIR=lib \
        -DCMAKE_PREFIX_PATH="${DEPS_DIR}" \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=ON \
        "${CMAKE_COMPAT[@]}"
    cmake --build vorbis/build -j "${NPROC}"
    cmake --install vorbis/build
else
    echo "==> libvorbis already installed, skipping"
fi

# ---------------------------------------------------------------------------
# libFLAC (requise par SFML Audio)
# Toutes les options docs/manpages/tools coupées
# ---------------------------------------------------------------------------
if [ ! -f "${DEPS_DIR}/lib/libFLAC.so" ]; then
    echo "==> Building libFLAC"
    cd "${BUILD_DIR}"
    if [ ! -d "flac" ]; then
        git clone --branch 1.4.3 --depth 1 https://github.com/xiph/flac.git flac
    fi
    cmake -S flac -B flac/build \
        -DCMAKE_INSTALL_PREFIX="${DEPS_DIR}" \
        -DCMAKE_INSTALL_LIBDIR=lib \
        -DCMAKE_PREFIX_PATH="${DEPS_DIR}" \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=ON \
        -DBUILD_PROGRAMS=OFF \
        -DBUILD_EXAMPLES=OFF \
        -DBUILD_TESTING=OFF \
        -DBUILD_DOCS=OFF \
        -DINSTALL_MANPAGES=OFF \
        "${CMAKE_COMPAT[@]}"
    cmake --build flac/build -j "${NPROC}"
    cmake --install flac/build
else
    echo "==> libFLAC already installed, skipping"
fi

# ---------------------------------------------------------------------------
# OpenAL Soft (requis par SFML Audio)
# ---------------------------------------------------------------------------
if [ ! -f "${DEPS_DIR}/lib/libopenal.so" ]; then
    echo "==> Building OpenAL Soft"
    cd "${BUILD_DIR}"
    if [ ! -d "openal-soft" ]; then
        git clone --branch 1.24.3 --depth 1 https://github.com/kcat/openal-soft.git
    fi
    cmake -S openal-soft -B openal-soft/build \
        -DCMAKE_INSTALL_PREFIX="${DEPS_DIR}" \
        -DCMAKE_INSTALL_LIBDIR=lib \
        -DCMAKE_BUILD_TYPE=Release \
        -DLIBTYPE=SHARED \
        -DALSOFT_UTILS=OFF \
        -DALSOFT_EXAMPLES=OFF \
        "${CMAKE_COMPAT[@]}"
    cmake --build openal-soft/build -j "${NPROC}"
    cmake --install openal-soft/build
else
    echo "==> OpenAL already installed, skipping"
fi

# ---------------------------------------------------------------------------
# SFML
# ---------------------------------------------------------------------------
if [ ! -d "${DEPS_DIR}/include/SFML" ]; then
    echo "==> Building SFML"
    cd "${BUILD_DIR}"
    if [ ! -d "SFML" ]; then
        git clone --branch 2.6.x --depth 1 https://github.com/SFML/SFML.git
    fi
    cmake -S SFML -B SFML/build \
        -DCMAKE_INSTALL_PREFIX="${DEPS_DIR}" \
        -DCMAKE_INSTALL_LIBDIR=lib \
        -DCMAKE_PREFIX_PATH="${DEPS_DIR}" \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=ON \
        -DUDEV_PATH_INCLUDES="${DEPS_DIR}/include" \
        -DUDEV_PATH_LIB="${DEPS_DIR}/lib" \
        "${CMAKE_COMPAT[@]}"
    cmake --build SFML/build -j "${NPROC}"
    cmake --install SFML/build
else
    echo "==> SFML already installed, skipping"
fi

# ---------------------------------------------------------------------------
# SDL2
# ---------------------------------------------------------------------------
if [ ! -d "${DEPS_DIR}/include/SDL2" ]; then
    echo "==> Building SDL2"
    cd "${BUILD_DIR}"
    if [ ! -d "SDL" ]; then
        git clone --branch release-2.30.0 --depth 1 https://github.com/libsdl-org/SDL.git
    fi
    cmake -S SDL -B SDL/build \
        -DCMAKE_INSTALL_PREFIX="${DEPS_DIR}" \
        -DCMAKE_INSTALL_LIBDIR=lib \
        -DCMAKE_PREFIX_PATH="${DEPS_DIR}" \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=ON \
        "${CMAKE_COMPAT[@]}"
    cmake --build SDL/build -j "${NPROC}"
    cmake --install SDL/build
else
    echo "==> SDL2 already installed, skipping"
fi

# ---------------------------------------------------------------------------
# raylib
# ---------------------------------------------------------------------------
if [ ! -f "${DEPS_DIR}/include/raylib.h" ]; then
    echo "==> Building raylib"
    cd "${BUILD_DIR}"
    if [ ! -d "raylib" ]; then
        git clone --branch 5.0 --depth 1 https://github.com/raysan5/raylib.git
    fi
    cmake -S raylib -B raylib/build \
        -DCMAKE_INSTALL_PREFIX="${DEPS_DIR}" \
        -DCMAKE_INSTALL_LIBDIR=lib \
        -DCMAKE_PREFIX_PATH="${DEPS_DIR}" \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=ON \
        -DBUILD_EXAMPLES=OFF \
        "${CMAKE_COMPAT[@]}"
    cmake --build raylib/build -j "${NPROC}"
    cmake --install raylib/build
else
    echo "==> raylib already installed, skipping"
fi

echo "==> Done. Deps installed in ${DEPS_DIR}"