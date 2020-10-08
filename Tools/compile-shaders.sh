#!/usr/bin/env sh

mkdir -p cmake-build-debug/bin/Shaders

echo "Compiling shaders"
echo "shaders/main.vert.glsl -> cmake-build-debug/bin/Shaders/main.vert.spv"
glslc -fshader-stage=vert Shaders/main.vert.glsl -o cmake-build-debug/bin/Shaders/main.vert.spv
echo "shaders/main.frag.glsl -> cmake-build-debug/bin/Shaders/main.frag.spv"
glslc -fshader-stage=frag Shaders/main.frag.glsl -o cmake-build-debug/bin/Shaders/main.frag.spv

echo "Done."
