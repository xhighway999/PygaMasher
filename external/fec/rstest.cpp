/* Test the Reed-Solomon codecs
 * for various block sizes and with random data and random error patterns
 *
 * Copyright 2002 Phil Karn, KA9Q
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */

#include <array>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
extern "C" {
#include "fec.h"
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

RsCodec<8, 32> block;

#include <iostream>

int main() {
  std::array<unsigned char, 255> data;
  data.fill(0);

  data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 13, 37};

  block.encode(data);

  data[0]   = 9;
  data[254] = 2;

  block.decode(data);

  for(const auto &elem : data)
    std::cout << std::to_string(elem) << std::endl;

  std::cout << "Errors " << block.getErrorStatistics().errors << std::endl;
}

//
// struct etab {
//   int symsize;
//   int genpoly;
//   int fcs;
//   int prim;
//   int nroots;
//   int ntrials;
// } Tab[] = {
//     {2, 0x7, 1, 1, 1, 10},
//     {3, 0xb, 1, 1, 2, 10},
//     {4, 0x13, 1, 1, 4, 10},
//     {5, 0x25, 1, 1, 6, 10},
//     {6, 0x43, 1, 1, 8, 10},
//     {7, 0x89, 1, 1, 10, 10},
//     {8, 0x11d, 1, 1, 32, 10},
//     {8, 0x187, 112, 11, 32, 10}, /* Duplicates CCSDS codec */
//     {9, 0x211, 1, 1, 32, 10},
//     {10, 0x409, 1, 1, 32, 10},
//     {11, 0x805, 1, 1, 32, 10},
//     {12, 0x1053, 1, 1, 32, 5},
//     {13, 0x201b, 1, 1, 32, 2},
//     {14, 0x4443, 1, 1, 32, 1},
//     {15, 0x8003, 1, 1, 32, 1},
//     {16, 0x1100b, 1, 1, 32, 1},
//     {0, 0, 0, 0, 0},
// };
//
// int exercise_char(struct etab *e);
//
// int main() {
//   int i;
//
//   srandom(time(NULL));
//   for(i = 0; Tab[i].symsize != 0; i++) {
//     int nn, kk;
//
//     nn = (1 << Tab[i].symsize) - 1;
//     printf("Loop %d Testing (%d,%d) code...\n", i, nn, kk);
//     if(Tab[i].symsize <= 8)
//       exercise_char(&Tab[i]);
//   }
//   exit(0);
// }
//
// int exercise_char(struct etab *e) {
//   int           nn = (1 << e->symsize) - 1;
//   unsigned char block[nn], tblock[nn];
//   int           errlocs[nn], derrlocs[nn];
//   int           i;
//   int           errors;
//   int           derrors, kk;
//   int           errval, errloc;
//   int           erasures;
//   int           decoder_errors = 0;
//   void *        rs;
//
//   if(e->symsize > 8)
//     return -1;
//
//   /* Compute code parameters */
//   kk = nn - e->nroots;
//
//   rs = init_rs_char(e->symsize, e->genpoly, e->fcs, e->prim, e->nroots, 0);
//   if(rs == NULL) {
//     printf("init_rs_char failed!\n");
//     return -1;
//   }
//   /* Test up to the error correction capacity of the code */
//   for(errors = 0; errors <= e->nroots / 2; errors++) {
//     /* Load block with random data and encode */
//     for(i = 0; i < kk; i++)
//       block[i] = random() & nn;
//     memcpy(tblock, block, sizeof(block));
//     encode_rs_char(rs, block, &block[kk]);
//
//     /* Make temp copy, seed with errors */
//     memcpy(tblock, block, sizeof(block));
//     memset(errlocs, 0, sizeof(errlocs));
//     memset(derrlocs, 0, sizeof(derrlocs));
//     erasures = 0;
//     for(i = 0; i < errors; i++) {
//       do {
//         errval = random() & nn;
//       } while(errval == 0); /* Error value must be nonzero */
//
//       do {
//         errloc = random() % nn;
//       } while(errlocs[errloc]
//               != 0); /* Must not choose the same location twice */
//
//       errlocs[errloc] = 1;
//
// #if FLAG_ERASURE
//       if(random() & 1) /* 50-50 chance */
//         derrlocs[erasures++] = errloc;
// #endif
//       tblock[errloc] ^= errval;
//     }
//
//     /* Decode the errored block */
//     derrors = decode_rs_char(rs, tblock, derrlocs, erasures);
//
//     if(derrors != errors) {
//       printf("(%d,%d) decoder says %d errors, true number is %d\n",
//              nn,
//              kk,
//              derrors,
//              errors);
//       decoder_errors++;
//     }
//     for(i = 0; i < derrors; i++) {
//       if(errlocs[derrlocs[i]] == 0) {
//         printf("(%d,%d) decoder indicates error in location %d without
//         error\n",
//                nn,
//                kk,
//                derrlocs[i]);
//         decoder_errors++;
//       }
//     }
//     if(memcmp(tblock, block, sizeof(tblock)) != 0) {
//       printf("(%d,%d) decoder uncorrected errors! output ^ input:", nn, kk);
//       decoder_errors++;
//       for(i = 0; i < nn; i++)
//         printf(" %02x", tblock[i] ^ block[i]);
//       printf("\n");
//     }
//   }
//
//   free_rs_char(rs);
//   return 0;
// }
