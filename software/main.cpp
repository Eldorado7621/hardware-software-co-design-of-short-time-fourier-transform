#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <xtime_l.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "stft.h"
#include "data.h"
#include "xfft1024c.h"
#include <iostream>
#include "helper.h"

 XFft1024c xf;

void hw_fft(data_comp data_hw_out[N],data_comp windowed_signal[N])
{

   XFft1024c_Write_data_IN_Words(&xf,0, (long unsigned int *)(windowed_signal),N*2);

   XFft1024c_Start(&xf);
   while(!XFft1024c_IsDone(&xf));

   XFft1024c_Read_data_OUT_Words(&xf,0, (long unsigned int *)(data_hw_out),N*2);
}

//compute the FFT for each butterfly stage
void FFT0(int FFT_stage,int pass_check,int index_shift,int pass_shift,data_comp data_IN[N], data_comp data_OUT[N],data_comp W[N/2]){

	int butterfly_span=0,butterfly_pass=0;
	FFT_label1: for (int i = 0; i < N/2; i++) {
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
void FFT(const data_comp data_IN[N], data_comp data_OUT[N],int FFT_int_stage[NO_STAGES],data_comp data_OUT0[N],data_comp W[N/2])
{
	//calculate the bit reversal
	bit_reversal(data_IN, data_OUT,FFT_int_stage);

	//calculate the twiddle factor
	twiddle_factor(W);

	//calculate the intermediate FFT stages
	for (int i=0;i<NO_STAGES;i=i+2)
		{
		  FFT0(FFT_int_stage[NO_STAGES-(i+1)],FFT_int_stage[i],NO_STAGES-(i+1),i+1,data_OUT,data_OUT0,W);
		  if(i+1<NO_STAGES)
		  {
			FFT0(FFT_int_stage[NO_STAGES-(i+2)],FFT_int_stage[i+1],NO_STAGES-(i+2),i+2,data_OUT0,data_OUT,W);
		  }

		}
	}

int main()
{

	print("entering main function\n\r");

	//create buffer to hold data
	static data_comp data_hw_out[N];
	data_comp windowed_signal[N];
	int window_index[WINDOW_INDEX_LENGTH];//array to store the index for each window interval


	static data_comp data_OUT[N];
	static data_comp data_OUT0[N];
	static data_comp W[N/2]; //for the twiddle factor
	int FFT_int_stage[NO_STAGES];

	 XTime hw_processor_start, hw_processor_stop;
	 XTime sw_processor_start, sw_processor_stop;

	//buffer to store the stft data
	char text_temp[32];

	 //hardware initialization

	 int status;
	 XFft1024c_Initialize(&xf, XPAR_FFT1024C_0_DEVICE_ID);
	 XFft1024c_Config* xfptr_cfg;
	 xfptr_cfg=XFft1024c_LookupConfig(XPAR_FFT1024C_0_DEVICE_ID);
	 if(!xfptr_cfg)
     {
	   	printf("error in loading config");
     }
    status=XFft1024c_CfgInitialize(&xf,xfptr_cfg);
    if(status!=XST_SUCCESS)
   {
   	printf("error in initializing config");
   }





	//get the index position that signifies the starting index of the window for each interval
	win_start_index(window_index);

	//hardware computation
	//get count at the begining pf the computation
	 XTime_GetTime(&hw_processor_start);
	 for(int i=0;i<WINDOW_INDEX_LENGTH;i++)
	 {


		// xil_printf("\ncomputing FFT for window index %d \n",i);
		create_windowed_signal(window_index[i],FFT_input,windowed_signal);

	 	 //hardware FFT IP computation
	 	//hw_fft(data_hw_out,windowed_signal);

		XFft1024c_Write_data_IN_Words(&xf,0, (long unsigned int *)(windowed_signal),N*2);

		   XFft1024c_Start(&xf);
		   while(!XFft1024c_IsDone(&xf));

		   XFft1024c_Read_data_OUT_Words(&xf,0, (long unsigned int *)(data_hw_out),N*2);


		 //print out the fft of the windowed signal
//		 for (int j=0;j<N;j++)
//		 {
//
//			 printf("\n %d: PL output: %f + I%f ",(N*i)+(j+1),data_hw_out[j].real(),data_hw_out[j].imag());
//			 printf("\n PL input: %f",windowed_signal[j].real());
//			 printf("\n abs %f\n",std::abs(data_hw_out[j]));
//			 //std::cout<<"PL output:"<<data_hw_out[j].real()<<std::endl;
//		 }

	 }
	 XTime_GetTime(&hw_processor_stop);
	 printf("\n PL computation succesful");
	 float hw_processing_time=1000000.0*(hw_processor_stop-hw_processor_start)/(COUNTS_PER_SECOND);

	  //software computations starts
/*	 XTime_GetTime(&sw_processor_start);
	 for(int i=0;i<WINDOW_INDEX_LENGTH;i++)
	 {
		// xil_printf("\ncomputing FFT for window index %d \n",i);
		 create_windowed_signal(window_index[i],FFT_input,windowed_signal);

		 //FFT software computation starts

		 FFT(windowed_signal,data_OUT,FFT_int_stage,data_OUT0,W);

		 //print out the fft of the windowed signal
//		 for (int j=0;j<N;j++)
//		 {
//		 	 printf("\n %d: PL output: %f + I%f ",(N*i)+(j+1),data_hw_out[j].real(),data_hw_out[j].imag());
//
//				 //std::cout<<"PL output:"<<data_hw_out[j].real()<<std::endl;
//		}

	 }*/
	 // XTime_GetTime(&sw_processor_stop);
	  printf("\n software computation succesful");
	  float sw_processing_time=1000000.0*(hw_processor_stop-hw_processor_start)/(COUNTS_PER_SECOND);
	  printf("\n software processing time: %f, hardware processing time: %f",sw_processing_time,0.0);




    return 0;
}
