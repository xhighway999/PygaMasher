#include "rsencoder.hpp"

void RsEncoder::setInputFile(const fs::path &inputPath) {
  std::lock_guard<std::mutex> g(m_mutex);
  m_inputPath = inputPath;
}

void RsEncoder::setOutputFile(const fs::path &outputPath) {
  std::lock_guard<std::mutex> g(m_mutex);
  m_outputPath = outputPath;
}

void RsEncoder::start() {
  this->m_done = false;
  m_thread     = std::thread([this]() {
    // Open both files
    std::string inputPath, outputPath;
    size_t      bytesConverted, fileSize;

    // Lock the mutex and execute code, that uses shared variables
    {
      std::lock_guard<std::mutex> g(m_mutex);

      // Remove the old file, if any
      if(fs::exists(this->m_outputPath)) {
        fs::remove(this->m_outputPath);
      }
      bytesConverted = 0, fileSize = fs::file_size(this->m_inputPath);
      inputPath = this->m_inputPath, outputPath = this->m_outputPath;
      if(!fs::exists(inputPath)) {
        std::cout << "ERROR: file " << inputPath << " does not exist";
      }
    }
    RsCodec<8, 32> codec;
    std::ifstream  iFile(inputPath, std::ifstream::binary);
    if(!iFile.is_open()) {
      std::cout << "ERROR" << errno << ": Could not open iFile " << inputPath
                << std::endl;
      this->m_done = true;
      return;
    }
    std::ofstream oFile(this->m_outputPath, std::fstream::binary);

    std::array<unsigned char, codec.nn> block;
    do {
      // Read a chunk of the input file
      iFile.read(reinterpret_cast<char *>(&block[0]), codec.kk);

      // Encode is with RS
      codec.encode(block);

      // Write it to the output File
      oFile.write(reinterpret_cast<char *>(&block[0]), codec.nn);
      bytesConverted += codec.kk;

      // Update progress
      {
        std::lock_guard<std::mutex> g(m_mutex);
        m_progress = (float) bytesConverted / (float) fileSize;
      }

    } while(iFile.good());

    {
      std::lock_guard<std::mutex> g(m_mutex);
      m_done = true;
    }
  });
}

float RsEncoder::getProgress() {
  std::lock_guard<std::mutex> g(m_mutex);
  return m_progress;
}

bool RsEncoder::done() {
  std::lock_guard<std::mutex> g(m_mutex);
  return m_done;
}

RsEncoder::~RsEncoder() {
  if(m_thread.joinable())
    m_thread.join();
}
