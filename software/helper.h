#include "main.h"

//create an array that stores the number of data point for each stage of the butterfly computation
int data_points_stages(int FFT_int_stage[NO_STAGES])
{
	int no_of_stage = 0;
	int value=N;
	while (value!=1) {

	   value=value>>1;
	   FFT_int_stage[no_of_stage]=value;
	   no_of_stage++;
	}
	return no_of_stage;

}

//compute the bit reversal
void bit_reversal(const data_comp data_in[N], data_comp dit_inp[N], int FFT_int_stage[NO_STAGES])
{

	int size_a=data_points_stages(FFT_int_stage);
    for (int j=0;j<N;j++)
    {
         int reversed=0;
         for (int i=0;i<size_a;i++)
         {
               reversed|=((j>>i)&1)<<(size_a-i-1);
         }
                dit_inp[j] = data_in[reversed];


           }

       }



 void twiddle_factor(data_comp W[N/2])
  {
       	 for(int i=0;i<N/2;i++)
       	 {
       		// W[i]=cos(2 * M_PI *i/ N),-sin(2 * M_PI *i/ N);
       		 W[i]=data_comp(cos(2 * M_PI *i/ N) ,-sin(2 * M_PI *i/ N) );
       	 }

   }
