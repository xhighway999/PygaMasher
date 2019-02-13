#include "imagewriter.hpp"
void ImageWriter::start() {
  // TODO: Add multiple of 8 check
  bytesPerImage = width * height / (chunkSize * chunkSize);

  currentByte  = 0;
  currentImage = 0;
  try {
    if(std::filesystem::exists(wd))
      std::filesystem::remove_all(wd);
    std::filesystem::create_directory(wd);
  } catch(const std::exception &e) {
    std::cout << e.what() << "\n";
    exit(1);
  }
}
void ImageWriter::writeData(char *data, size_t size) {
  size_t dataRemaining = size;

  do {
    size_t bytesFreeInImage = bytesPerImage - currentImageData.size();

    if(bytesFreeInImage != 0) {
      size_t bytesToWrite = std::min(bytesFreeInImage, dataRemaining);
      currentImageData.insert(
          currentImageData.end(), data, data + bytesToWrite);
      data += bytesToWrite;
      dataRemaining -= bytesToWrite;

    } else {
      flushImage();
    }
  } while(dataRemaining != 0);
}

void ImageWriter::flushImage() {
  currentImageData.resize(bytesPerImage);

  // Create a vector that stores the final image
  std::vector<char> image;

  image.resize(width * height);

  // Lambda function to address a pixel

  auto setPixel = [&image, this](size_t x, size_t y, bool state) {
    size_t i = y * width + x;

    unsigned char c = 0;
    if(state)
      c = 255;

    image[i] = c;
  };

  auto setChunk = [this, &setPixel](size_t x, size_t y, unsigned char data) {
    for(size_t j = 0; j < chunkSize; j++) {
      for(size_t i = 0; i < 8; i++) {
        unsigned char val = (data >> i) & 0b00000001;
        setPixel((x * chunkSize) + i, (y * chunkSize) + j, val * 255);
      }
    }
  };
  size_t chunk          = 0;
  auto   writeNextChunk = [this, &setChunk, &chunk](char val) {
    size_t x = chunk % (width / chunkSize);
    size_t y = chunk / (width / chunkSize);
    setChunk(x, y, val);
    chunk++;
  };

  for(const char &byte : currentImageData) {
    writeNextChunk(byte);
  }
  char t[32];
  sprintf(t, "%05zu", currentImage++);
  std::string number = t;

  std::string           fileName = number + ".png";
  std::filesystem::path filePath = wd / fileName;
  stbi_write_png(filePath.string().c_str(), width, height, 1, &image[0], width);
  currentImageData.resize(0);
}

void ImageWriter::end() {
  // TODO: Mark data ending with something or have a header
  flushImage();
  std::filesystem::path fullWdPath = wd / "%05d.png";

  std::string command = ffmpegName + " -y -framerate 1 -i \""
                        + fullWdPath.string() + "\" -c:v libx264 \""
                        + fileOutputPath.string() + "\"";
  cmdRunner.runCommand(command);

  std::string log;

  do {
    std::string part = cmdRunner.readAvail();
    log += part;

  } while(cmdRunner.readable());
  cmdRunner.close();
}

void ImageWriter::setOutputFile(const std::string &path) {
  fileOutputPath = path;
}
