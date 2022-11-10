#include "main.h"



//functions creates the starting index of the window for each computation
void win_start_index(int window_index[WINDOW_INDEX_LENGTH])
{

	int ind=0;
    for (int i=0;i<=N;i=i+(WINDOW-OVERLAP))
    {
    	window_index[ind]=i;
        ind++;
    }


}



void  create_windowed_signal(int Starts_win_ind,const data_comp FFT_input[N],data_comp windowed_signal[N])
{

	//get the windowed data

	for(int i=0;i<N;i++)
	{
		windowed_signal[i]=0.0+I*0.0;
	}
    int window_limit=Starts_win_ind+WINDOW;
    if(window_limit>N) //check if window limit is greater than the highest sample
    {
        window_limit=N;
    }
    //xil_printf("\n start window %d",Starts_win_ind);

    //extract from the original time series signal using the window
    for(int isg=Starts_win_ind;isg<window_limit;isg++)
    {
        windowed_signal[isg]=FFT_input[isg];
       // xil_printf("\nwindow %d %d ",isg,windowed_signal[isg]);

    }
    //xil_printf("\nwindow 5 %d ",windowed_signal[5]);


}





