#ifndef __MAIN_H__
#define __MAIN_H__

#include <math.h>
#include <complex>

#define N 1024
#define NO_STAGES 8
#define WINDOW 256
#define OVERLAP 128

#define WINDOW_INDEX_LENGTH ((N-1)/(WINDOW-OVERLAP))+1

typedef float data_t;
typedef std::complex<data_t> data_comp;

//FFT(data_comp data_IN[FFT_LENGTH], data_comp data_OUT[FFT_LENGTH],int FFT_int_stage[NO_STAGES],data_comp data_OUT0[FFT_LENGTH],data_comp W[FFT_LENGTH/2]);

#endif
