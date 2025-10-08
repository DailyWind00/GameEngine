#!/usr/bin/env bash
set -e

# ==============================
# Dependencies install script
# for GameEngine Framework
# ==============================

# Root directory for dependencies
DEPS_DIR="dependencies"
mkdir -p $DEPS_DIR/temp
cd $DEPS_DIR

echo "[*] Installing dependencies into $DEPS_DIR folder."

# ------------------------------
# GLAD (OpenGL 4.0 core, no extensions)
# ------------------------------
if [ ! -d "glad" ]; then
	cd temp
    echo "[*] Fetching GLAD..."
    git clone --depth=1 https://github.com/Dav1dde/glad.git
    cd glad

    echo "[*] Creating temporary Python venv for GLAD..."
    python3 -m venv .venv
    source .venv/bin/activate
    pip install --upgrade pip --no-warn-script-location
    pip install glad

    echo "[*] Generating GLAD files..."
    glad --profile="core" --api="gl=4.5" --generator="c" --spec="gl" --extensions="" --out-path="." --no-loader
	mv include/* ../.. && mv src/glad.c ../../glad/glad.c

    deactivate
	echo "[*] Temporary Python venv removed."
	cd ../..
	rm -rf temp/glad
	echo "[*] GLAD installed."
else
    echo "[*] GLAD already installed."
fi

# ------------------------------
# GLFW
# ------------------------------
if [ ! -d "glfw" ]; then
	cd temp
    echo "[*] Fetching GLFW..."
    git clone --depth=1 https://github.com/glfw/glfw.git glfw
    cd glfw

    echo "[*] Building GLFW locally..."
    cmake -S . -B build -DCMAKE_INSTALL_PREFIX="../../glfw" -DBUILD_SHARED_LIBS=OFF
    cmake --build build --config Release
    cmake --install build

    echo "[*] Cleaning up GLFW build files..."
    cd ..
    rm -rf glfw
	cd ../glfw
	mv include/GLFW/* . && rm -rf include
	mv lib/libglfw3.a . && rm -rf lib
	cd ..

    echo "[*] GLFW installed."
else
    echo "[*] GLFW already installed."
fi

# ------------------------------
# GLM
# ------------------------------
if [ ! -d "glm" ]; then
	cd temp
    echo "[*] Fetching GLM..."
    git clone --depth=1 https://github.com/g-truc/glm.git
    cd glm/glm
	cd ..
	mv glm ../../glm
    cd ../..
	rm -rf temp/glm
	echo "[*] GLM installed."
else
    echo "[*] GLM already installed."
fi

# ------------------------------
# nlohmann/json
# ------------------------------
if [ ! -d "json" ]; then
	cd temp
    echo "[*] Fetching nlohmann/json..."
    git clone --depth=1 https://github.com/nlohmann/json.git
    mv json/single_include/nlohmann ../json
	cd ..
    # Create helper config header
    cat > json/json_config.hpp <<'EOF'
#include <fstream>
#include "json.hpp"
using json = nlohmann::json; // imagine being this self centered
EOF
	rm -rf temp/json json/json_fwd.hpp
	echo "[*] nlohmann/json installed."
else
    echo "[*] nlohmann/json already installed."
fi

# ------------------------------
# OpenCL headers (C + C++ minimal)
# ------------------------------
if [ ! -d "OpenCL" ]; then
    cd temp
    echo "[*] Fetching OpenCL C headers..."
    git clone --depth=1 https://github.com/KhronosGroup/OpenCL-Headers.git
    mkdir -p ../OpenCL
    cp OpenCL-Headers/CL/cl.h ../OpenCL
    cp OpenCL-Headers/CL/cl_gl.h ../OpenCL
    rm -rf OpenCL-Headers

    echo "[*] Fetching OpenCL C++ headers..."
    git clone --depth=1 https://github.com/KhronosGroup/OpenCL-CLHPP.git
    cp OpenCL-CLHPP/include/CL/opencl.hpp ../OpenCL
    rm -rf OpenCL-CLHPP

    cd ..
    echo "[*] OpenCL installed (minimal: cl.h, cl_gl.h, opencl.hpp)."
else
    echo "[*] OpenCL already installed."
fi

# ------------------------------
# stb_image
# ------------------------------
if [ ! -d "stb_image" ]; then
	cd temp
    echo "[*] Fetching stb..."
    git clone --depth=1 https://github.com/nothings/stb.git
	mkdir -p ../stb_image
    mv stb/stb_image.h ../stb_image/
	cd ..
    # Create stb_def.c
    cat > stb_image/stb_def.c <<'EOF'
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
EOF
	rm -rf temp/stb
	echo "[*] stb installed."
else
    echo "[*] stb already installed."
fi

rm -rf temp
cd ..
echo "[*] All dependencies installed successfully."
