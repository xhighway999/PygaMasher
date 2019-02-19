/* User include file for libfec
 * Copyright 2004, Phil Karn, KA9Q
 * May be used under the terms of the GNU Lesser General Public License (LGPL)
 */

#ifndef _FEC_H_
#define _FEC_H_

/* General purpose RS codec, 8-bit symbols */
void encode_rs_char(void *rs,unsigned char *data,unsigned char *parity);
int decode_rs_char(void *rs,unsigned char *data,int *eras_pos,
		   int no_eras);
void *init_rs_char(int symsize,int gfpoly,
		   int fcr,int prim,int nroots,
		   int pad);
void free_rs_char(void *rs);

/* CCSDS standard (255,223) RS codec with conventional (*not* dual-basis)
 * symbol representation
 */
void encode_rs_8(unsigned char *data,unsigned char *parity,int pad);
int decode_rs_8(unsigned char *data,int *eras_pos,int no_eras,int pad);

/* CCSDS standard (255,223) RS codec with dual-basis symbol representation */
void encode_rs_ccsds(unsigned char *data,unsigned char *parity,int pad);
int decode_rs_ccsds(unsigned char *data,int *eras_pos,int no_eras,int pad);

/* Tables to map from conventional->dual (Taltab) and
 * dual->conventional (Tal1tab) bases
 */
extern unsigned char Taltab[],Tal1tab[];


/* CPU SIMD instruction set available */
extern enum cpu_mode {UNKNOWN=0,PORT,MMX,SSE,SSE2,ALTIVEC} Cpu_mode;
void find_cpu_mode(void); /* Call this once at startup to set Cpu_mode */

/* Determine parity of argument: 1 = odd, 0 = even */
#ifdef __i386__
static inline int parityb(unsigned char x){
  __asm__ __volatile__ ("test %1,%1;setpo %0" : "=g"(x) : "r" (x));
  return x;
}
#else
void partab_init();

static inline int parityb(unsigned char x){
  extern unsigned char Partab[256];
  extern int P_init;
  if(!P_init){
    partab_init();
  }
  return Partab[x];
}
#endif


static inline int parity(int x){
  /* Fold down to one byte */
  x ^= (x >> 16);
  x ^= (x >> 8);
  return parityb(x);
}

/* Useful utilities for simulation */
double normal_rand(double mean, double std_dev);
unsigned char addnoise(int sym,double amp,double gain,double offset,int clip);

extern int Bitcnt[];

/* Dot product functions */
void *initdp(signed short coeffs[],int len);
void freedp(void *dp);
long dotprod(void *dp,signed short a[]);

void *initdp_port(signed short coeffs[],int len);
void freedp_port(void *dp);
long dotprod_port(void *dp,signed short a[]);


/* Sum of squares - accepts signed shorts, produces unsigned long long */
unsigned long long sumsq(signed short *in,int cnt);
unsigned long long sumsq_port(signed short *in,int cnt);



/* Low-level data structures and routines */


#endif /* _FEC_H_ */



