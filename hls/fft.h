#ifndef __MAIN_H__
#define __MAIN_H__

#include <cmath>
#include <complex>
#include <iostream>

#include <ap_axi_sdata.h>
#include <ap_int.h>
#include <hls_stream.h>

#define FFT_LENGTH 16
#define NO_STAGES 4

using namespace std;


typedef float time_series_data;
typedef complex<time_series_data> data_comp;


typedef ap_axis<64,2,5,6> axis_data;

union fpint {
		int ival;		// integer alias
		float fval;		// floating-point alias
	};



typedef struct{
	float Re;
	float Im;

}Complex;

typedef struct{
	int Re;
	int Im;

}Data;

typedef union{
	Complex complex; //complex alias
	int64_t reg;   //integer alias
}ComplexData;



/*struct axis_datain{
	data_comp data;
	ap_uint<1> last;

};*/

void fft(hls::stream<axis_data> &dataInStream,hls::stream<axis_data> &dataOutStream);
#endif
