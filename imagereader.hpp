#pragma once

#include "commandrunner.hpp"
#include "ffmpeg.hpp"
#include "stb/stb_image.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
class ImageReader {
public:
  ImageReader();
  void setInputFile(const std::filesystem::path &path);
  void setOutputFile(const std::filesystem::path &path);
  void start();

private:
  std::ofstream outFile;
  void          readImage(const std::filesystem::path &path);

  std::filesystem::path inputVideo = "out.mp4";
  std::filesystem::path cachePath  = "PygaMasher_wd";
  std::filesystem::path outputPath = "file";

  size_t chunkSize = 8;
};
