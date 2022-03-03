#pragma once
// Viewing window
constexpr int WINDOW_X = 800;
constexpr int WINDOW_Y = 800;
// Zoom level - higher values zoom more
constexpr int SCALE_X = 125;
constexpr int SCALE_Y = 125;
// Temporal elements
constexpr int FRAMERATE = 0;
constexpr int NUM_TERRAIN_LAYERS = 1;
// Higher erosion scale means slower erosion. Only relevant if FRAMERATE and NUM_TERRAIN_LAYERS is set.
constexpr int EROSION_SCALE = NUM_TERRAIN_LAYERS * 16;
constexpr int TEMPERATURE_SCALE = 600;
// Large foliage growth scale makes foliage smoother
constexpr int FOLIAGE_GROWTH_SCALE = 50;
// Level of Detail
constexpr int LOD = 8;
