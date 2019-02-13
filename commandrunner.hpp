#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#ifndef __MINGW32__
#  include <fcntl.h>
#  include <unistd.h>
#endif
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
