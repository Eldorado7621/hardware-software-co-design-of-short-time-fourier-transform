#include "helper.h"
#include <iostream>
using namespace std;


void FFT0(int FFT_stage,int pass_check,int index_shift,int pass_shift,data_comp data_IN[FFT_LENGTH], data_comp data_OUT[FFT_LENGTH],data_comp W[FFT_LENGTH/2]){

	int butterfly_span=0,butterfly_pass=0;
	FFT_label1: for (int i = 0; i < FFT_LENGTH/2; i++) {
		int index = butterfly_span << index_shift;
		int Ulimit = butterfly_span + (butterfly_pass<<pass_shift);
		int Llimit = Ulimit + FFT_stage;
		data_comp Product = W[index] * data_IN[Llimit];//calculate the product
		data_OUT[Llimit] = data_IN[Ulimit]-Product;
		data_OUT[Ulimit] = data_IN[Ulimit]+Product;
		if (butterfly_span<FFT_stage-1){
			butterfly_span++;
		} else if (butterfly_pass<pass_check-1) {
			butterfly_span = 0;	butterfly_pass++;
		} else {
			butterfly_span = 0;	butterfly_pass=0;
		}
	}
}

void fft(hls::stream<axis_data> &dataInStream,hls::stream<axis_data> &dataOutStream)
{

#pragma HLS INTERFACE axis register both port=dataInStream
#pragma HLS INTERFACE axis register both port=dataOutStream
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL_BUS

	axis_data output_stream,input_stream;
	compintt idata,odata;

	ComplexData inputData,outputData;

	data_comp data_in[FFT_LENGTH];
	static data_comp data_OUT0[FFT_LENGTH]={0};
	static data_comp data_OUT1[FFT_LENGTH]={0};
	static data_comp W[FFT_LENGTH/2];
	int FFT_int_stage[FFT_LENGTH];

	fpint iidata;


//loop can be unrolled
	for(int i=0;i<FFT_LENGTH;i++)
	{
#pragma HLS PIPELINE

		input_stream=dataInStream.read();
		//inputData.data=input_stream.data;

		//inputData.data.Re=(int32_t)(0xFFFFFFFF&input_stream.data);
		//inputData.data.Im=(int32_t)(0xFFFFFFFF&(input_stream.data>>32));

		inputData.reg=input_stream.data;

		//iidata.ival=input_stream.data;

		data_in[i]=data_comp(inputData.complex.Re,inputData.complex.Im);

		//data_in[i]=data_comp(iidata.fval);


	}

	//calculate the bit reversal
	bit_reversal(data_in, data_OUT1,FFT_int_stage);

	//calculate the twiddle factor
	twiddle_factor(W);

	//calculate the intermediate FFT stages
	for (int i=0;i<NO_STAGES;i=i+2)
		{
		  FFT0(FFT_int_stage[NO_STAGES-(i+1)],FFT_int_stage[i],NO_STAGES-(i+1),i+1,data_OUT1,data_OUT0,W);
		  if(i+1<NO_STAGES)
		  {
			FFT0(FFT_int_stage[NO_STAGES-(i+2)],FFT_int_stage[i+1],NO_STAGES-(i+2),i+2,data_OUT0,data_OUT1,W);
		  }

		}
	if(NO_STAGES%2==0)
	{
		for (int i=0;i<FFT_LENGTH-1;i++)
		{
#pragma HLS PIPELINE

			outputData.complex.Re=real(data_OUT1[i]);
			outputData.complex.Im=imag(data_OUT1[i]);
			//output_stream.data=(int64_t)(outputData.data.Im)<<32|(int64_t)(outputData.data.Re);
			output_stream.data=outputData.reg;


/*			odata.complexValueStruct.real=real(data_OUT1[i]);
			odata.complexValueStruct.imag=imag(data_OUT1[i]);
			output_stream.data=odata.ival;*/

			output_stream.last=0;
			output_stream.keep=input_stream.keep;
			output_stream.strb=input_stream.strb;
			output_stream.user=input_stream.user;
			output_stream.id=input_stream.id;
			output_stream.dest=input_stream.dest;
			dataOutStream.write(output_stream);
		}

		outputData.complex.Re=real(data_OUT1[FFT_LENGTH-1]);
		outputData.complex.Im=imag(data_OUT1[FFT_LENGTH-1]);
		//output_stream.data=(int64_t)(outputData.data.Im)<<32|(int64_t)(outputData.data.Re);
		output_stream.data=outputData.reg;

/*		odata.complexValueStruct.real=real(data_OUT1[FFT_LENGTH-1]);
		odata.complexValueStruct.imag=imag(data_OUT1[FFT_LENGTH-1]);
		output_stream.data=odata.ival;*/

		output_stream.keep=input_stream.keep;
		output_stream.strb=input_stream.strb;
		output_stream.user=input_stream.user;
		output_stream.id=input_stream.id;
		output_stream.dest=input_stream.dest;
		output_stream.last=1;
		dataOutStream.write(output_stream);
	}
	else
	{
		for (int i=0;i<FFT_LENGTH-1;i++)
		{
#pragma HLS PIPELINE

/*			odata.complexValueStruct.real=real(data_OUT0[i]);
			odata.complexValueStruct.imag=imag(data_OUT0[i]);
			output_stream.data=odata.ival;*/


			outputData.complex.Re=real(data_OUT0[i]);
			outputData.complex.Im=imag(data_OUT0[i]);
			//output_stream.data=(int64_t)(outputData.data.Im)<<32|(int64_t)(outputData.data.Re);
			output_stream.data=outputData.reg;

			output_stream.keep=input_stream.keep;
			output_stream.strb=input_stream.strb;
			output_stream.user=input_stream.user;
			output_stream.id=input_stream.id;
			output_stream.dest=input_stream.dest;
			output_stream.last=0;
			dataOutStream.write(output_stream);
		}
/*		odata.complexValueStruct.real=real(data_OUT0[FFT_LENGTH-1]);
		odata.complexValueStruct.imag=imag(data_OUT0[FFT_LENGTH-1]);
		output_stream.data=odata.ival;*/

		outputData.complex.Re=real(data_OUT0[FFT_LENGTH-1]);
		outputData.complex.Im=imag(data_OUT0[FFT_LENGTH-1]);
		//output_stream.data=(int64_t)(outputData.data.Im)<<32|(int64_t)(outputData.data.Re);
		output_stream.data=outputData.reg;

		output_stream.keep=input_stream.keep;
		output_stream.strb=input_stream.strb;
		output_stream.user=input_stream.user;
		output_stream.id=input_stream.id;
		output_stream.dest=input_stream.dest;
		output_stream.last=1;
		dataOutStream.write(output_stream);
	}

}















