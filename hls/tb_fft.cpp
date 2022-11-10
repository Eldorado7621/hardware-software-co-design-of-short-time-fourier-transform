#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <complex>

#include <ap_axi_sdata.h>

#include <ap_int.h>
#include <hls_stream.h>
#include "fft.h"


#define TOL 0.01
#define NORM(x) ((x) * (x)).real()


using namespace std;


int main()
{

	axis_data tb_input_stream,tb_output_stream ;

	hls::stream<axis_data>tb_dataInStream, jjj;

	compintt t;
	ComplexData cmpdata;
	fpint iidata;


	float data_in[FFT_LENGTH];
	data_comp data_out[FFT_LENGTH];
	float temp1,temp2,temp3,temp4;
	int result=0;
	for(int z=0; z<1; z++){
		data_comp exp_out[FFT_LENGTH];
		ifstream FFTfileIN("inp_cpp.txt");  //reading input to the fft
		ifstream FFTfileOUT("out_cpp.txt");  //expected fft

		if (FFTfileIN.fail() || FFTfileOUT.fail()) {
			std::cerr << "Failed to open file." << endl;
			exit(-1);
		}


/*		for(int i=0; i<FFT_LENGTH; i++){
			FFTfileIN>>temp1>>temp2;
			data_in[i]=data_comp(temp1,temp2);
		}*/


		for(int i=0; i<FFT_LENGTH; i++){
			FFTfileIN>>temp1;
			data_in[i]=temp1;
		}
		FFTfileIN.close();
		for(int i=0; i<FFT_LENGTH; i++){

			iidata.fval=data_in[i];
			tb_input_stream.data=iidata.ival;


	/*		t.complexValueStruct.real=real(data_in[i]);
			t.complexValueStruct.imag=imag(data_in[i]);
			tb_input_stream.data=t.ival;*/

			//t.complexValueStruct={real(data_in[i]),imag(data_in[i])};


/*			cmpdata.complex.Re=real(data_in[i]);
			cmpdata.complex.Im=imag(data_in[i]);
			cmpdata.data.Re=cmpdata.complex.Re;
			cmpdata.data.Im=cmpdata.complex.Im;

			tb_input_stream.data=cmpdata.data;*/



			if(i==(FFT_LENGTH)-1)
				tb_input_stream.last=1;
			else
				tb_input_stream.last=0;
			tb_dataInStream.write(tb_input_stream);

		}
		fft(tb_dataInStream,jjj);
		for(int j=0; j<FFT_LENGTH;j++){
			FFTfileOUT >> temp3>> temp4;
			exp_out[j]=data_comp(temp3,temp4);
		}
		FFTfileOUT.close();



		for (int i=0;i<FFT_LENGTH;i++)
		{
			tb_output_stream=jjj.read();
			cmpdata.data.Re=tb_output_stream.data;
			data_out[i]=data_comp(cmpdata.complex.Re,cmpdata.complex.Im);

			//t.ival=tb_output_stream.data;
			//data_out[i]=data_comp(t.complexValueStruct.real,t.complexValueStruct.imag);

		}

		for(int k=0;k<FFT_LENGTH;k++){
			float n = NORM(exp_out[k]-data_out[k]);
			cout << "Exp: " << exp_out[k] << " \t- Got: " << data_out[k] << " \t- Inp: " << data_in[k] << " \t- Norm: " << n << endl;

			if (n>TOL) {
				result ++;
			}
		}




		/*		if (FFTfileIN.fail() || FFTfileOUT.fail()) {
			std::cerr << "Failed to open file." << endl;
			exit(-1);
		}
		data_t temp1,temp2,temp3,temp4;

		for(int i=0; i<FFT_LENGTH; i++){
			FFTfileIN>>temp1>>temp2;
			data_in[i]=data_comp(temp1,temp2);
//			cout << temp1 << " " << temp2 << endl;
		}
		FFTfileIN.close();

		fft(data_out,data_in);

		for(int j=0; j<FFT_LENGTH;j++){
			FFTfileOUT >> temp3 >> ws >> temp4;
			exp_out[j]=data_comp(temp3,temp4);
//			cout << "Python: " << temp3 << " " << temp4 << endl;
		}
		FFTfileOUT.close();*/


		/*		for(int k=0;k<FFT_LENGTH;k++){
			data_t n = NORM(exp_out[k]-data_out[k]);
			cout << "Exp: " << exp_out[k] << " \t- Got: " << data_out[k] << " \t- Inp: " << data_in[k] << " \t- Norm: " << n << endl;
//			cout << data_out[k].real() * 256 << ", " << data_out[k].imag() * 256 << endl;
			if (n>TOL) {
				result ++;
			}
		}*/

	}

	if (result == 0) {
		cout << "PASS" << endl;
	} else {
		cout << "FAIL: " << result << " errors" << endl;
	}
	return result;

}
