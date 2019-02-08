#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

class CommandRunner {
public:
  void        runCommand(const std::string &str);
  bool        readable();
  std::string readAvail();
  void        close();

private:
  FILE *fp;
  int   d;
  bool  closed = true;
};
