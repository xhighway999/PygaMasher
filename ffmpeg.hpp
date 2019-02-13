#pragma once
#include <string>
#ifdef _WIN32
static std::string ffmpegName = "ffmpeg.exe";
#else
static std::string ffmpegName = "ffmpeg";
#endif
