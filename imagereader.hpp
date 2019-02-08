#pragma once

#include "commandrunner.hpp"
#include "stb/stb_image.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
class ImageReader {
public:
  ImageReader();
  void setInputFile(const std::string &path);
  void setOutputFile(const std::string &path);
  void start();

private:
  std::ofstream outFile;
  void          readImage(const std::string &path);

  std::string inputVideo = "out.mp4";
  std::string cachePath  = "out";
  std::string outputPath = "file";

  size_t chunkSize = 8;
};
