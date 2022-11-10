#include "fft.h"



//create an array that stores the number of data point for each stage of the butterfly computation
int fft_no_of_stages(int FFT_int_stage[NO_STAGES])
{
	int no_of_stage = 0;
	int value=FFT_LENGTH;
	while (value!=1) {

	   value=value>>1;
	   FFT_int_stage[no_of_stage]=value;
	   no_of_stage++;
	}
	return no_of_stage;

}

//compute the bit reversal
void bit_reversal(data_comp data_in[FFT_LENGTH], data_comp dit_inp[FFT_LENGTH], int FFT_int_stage[NO_STAGES])
{

	int no_of_stages=fft_no_of_stages(FFT_int_stage);
    for (int j=0;j<FFT_LENGTH;j++)
    {
         int reversed=0;
         for (int i=0;i<no_of_stages;i++)
         {
               reversed|=((j>>i)&1)<<(no_of_stages-i-1);
         }
                dit_inp[j] = data_in[reversed];


           }

       }



 void twiddle_factor(data_comp W[FFT_LENGTH/2])
  {
       	 for(int i=0;i<FFT_LENGTH/2;i++)
       	 {
#pragma HLS UNROLL
       		// W[i]=cos(2 * M_PI *i/ FFT_LENGTH),-sin(2 * M_PI *i/ FFT_LENGTH);
       		 W[i]=data_comp(cos(2 * M_PI *i/ FFT_LENGTH) ,-sin(2 * M_PI *i/ FFT_LENGTH) );
       	 }

   }
