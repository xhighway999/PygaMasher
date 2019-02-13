#pragma once

#include "commandrunner.hpp"
#include "ffmpeg.hpp"
#include "stb/stb_image_write.h"

#include <algorithm>
#include <climits>
#include <dirent.h>
#include <fcntl.h>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <vector>
class ImageWriter {
public:
  void start();
  void writeData(char *data, size_t size);
  void end();

  void setOutputFile(const std::string &path);

private:
  void   flushImage();
  size_t width = 1280, height = 720, chunkSize = 8, bytesPerImage, currentByte;
  std::filesystem::path wd = "PygaMasher_wd";
  std::filesystem::path fileOutputPath;
  std::vector<uint8_t>  currentImageData;
  size_t                currentImage;
  CommandRunner         cmdRunner;
};
