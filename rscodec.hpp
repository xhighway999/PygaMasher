#pragma once

#include <array>
#include <iostream> //TODO remove

extern "C" {
#include "fec/fec.h"
};

template<size_t symsize, size_t nroots>
class RsCodec {
public:
  RsCodec(int genpoly = 0x187, int fcs = 112, int prim = 11) {
    rs = init_rs_char(symsize, genpoly, fcs, prim, nroots, 0);
  };
  ~RsCodec() { free_rs_char(rs); }

  static constexpr int nn = (1 << symsize) - 1, kk = nn - nroots;

  void encode(std::array<unsigned char, nn> &block) {
    encode_rs_char(rs, &block[0], &block[kk]);
  }
  void decode(std::array<unsigned char, nn> &block) {
    errorStatistics.errlocs.fill(0);
    errorStatistics.errors =
        decode_rs_char(rs, &block[0], &errorStatistics.errlocs[0], 0);
  }

  template<size_t nn>
  struct ErrorStatistics {
    std::array<int, nn> errlocs;
    int                 errors;
  };

private:
  ErrorStatistics<nn>           errorStatistics;
  void *                        rs;
  std::array<unsigned char, nn> block;

public:
  const ErrorStatistics<nn> &getErrorStatistics() { return errorStatistics; }
};

