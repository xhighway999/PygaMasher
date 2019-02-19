#pragma once

#include "rscodec.hpp"

#include <filesystem>
#include <fstream>
#include <mutex>
#include <thread>

namespace fs = std::filesystem;

class RsDecoder {
public:
  ~RsDecoder();
  void   setInputFile(const fs::path &inputPath);
  void   setOutputFile(const fs::path &outputPath);
  void   start();
  float  getProgress();
  size_t getTotalErrors();
  bool   done();

private:
  fs::path    m_inputPath, m_outputPath;
  std::mutex  m_mutex;
  std::thread m_thread;
  float       m_progress;
  bool        m_done        = false;
  size_t      m_totalErrors = 0;
};
