#include "imagereader.hpp"

ImageReader::ImageReader() {}
void ImageReader::start() {
  std::filesystem::remove_all(cachePath);
  std::filesystem::create_directory(cachePath);
  std::string command = "ffmpeg -y -i " + inputVideo
                        + " -vf \"select=not(mod(n\\,30))\" -vsync vfr -q:v 2 "
                        + cachePath + "/img_%05d.png";
  CommandRunner runner;
  runner.runCommand(command);
  while(runner.readable()) {
    runner.readAvail();
  }
  runner.close();

  outFile.open(outputPath, std::ios::binary | std::ios::out);

  std::vector<std::string> paths;
  for(const auto &p : std::filesystem::directory_iterator(cachePath)) {
    if(p.path().extension() == ".png") {
      paths.push_back(p.path());
    }
  }

  std::sort(paths.begin(), paths.end());

  for(const auto &p : paths) {
    readImage(p);
  }

  outFile.close();
}

void ImageReader::readImage(const std::string &path) {
  int            w, h, n;
  unsigned char *data = stbi_load(path.c_str(), &w, &h, &n, 1);

  auto getPixel = [&data, w](size_t x, size_t y) {
    size_t        i = y * w + x;
    unsigned char m = data[i];
    return 127 < m;
  };
  auto readChunk = [this, &getPixel](size_t x, size_t y) {
    unsigned char val = 0;
    for(size_t i = 0; i < chunkSize; i++) {
      // TODO: Sample any of the other 7 pixels
      unsigned char pix =
          getPixel(x * chunkSize + i, y * chunkSize + (chunkSize / 2));
      val |= pix << i;
    }
    return val;
  };
  // sample every CHUNK pixel with a given offset
  std::vector<unsigned char> imageData;
  imageData.resize((w / chunkSize) * (h / chunkSize));
  for(size_t i = 0; i < (w / chunkSize) * (h / chunkSize); i++) {
    size_t x     = i % (w / chunkSize);
    size_t y     = i / (w / chunkSize);
    imageData[i] = readChunk(x, y);
  }

  outFile.write(reinterpret_cast<char *>(&imageData[0]), imageData.size());
  free(data);
}

void ImageReader::setInputFile(const std::string &path) { inputVideo = path; }
void ImageReader::setOutputFile(const std::string &path) { outputPath = path; }

