#include "commandrunner.hpp"

void CommandRunner::runCommand(const std::string &str) {
#ifndef __MINGW32__
  fp = popen(str.c_str(), "r");
  d  = fileno(fp);
  fcntl(d, F_SETFL, O_NONBLOCK);
  if(fp == NULL) {
    printf("Failed to run command\n");
    exit(1);
  }
#else
  if((fp = _popen(str.c_str(), "r")) == NULL) {
    exit(1);
  }
#endif
  closed = false;
}

bool CommandRunner::readable() { return !closed; }

std::string CommandRunner::readAvail() {
  std::string availData;
  bool        dataInPipe = true;
  char        buf[1024];
  size_t      count = 1024;
  do {
    ssize_t r = fread(buf, 1, count, fp);
    if(r == -1 && errno == EAGAIN) {
      // no data yet
      dataInPipe = false;
    } else if(r > 0) {
      // received data
      availData.append(buf, r);
    } else {
      // pipe closed
      closed     = true;
      dataInPipe = false;
      return availData;
    }
    return availData;
  } while(dataInPipe);
}

void CommandRunner::close() { pclose(fp); }
