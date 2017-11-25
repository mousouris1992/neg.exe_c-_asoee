#include "ppm.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

using namespace imaging;



float * imaging::ReadPPM(const char * filename, int * w, int * h) {


	std::ifstream infile;

	


	infile.open(filename, std::ifstream::binary);

	if (!infile.is_open()) {
		std::cerr << "~[Error] : Error while trying to open PPM file!" << std::endl;
		return nullptr;
	}
	/*------------------*/
	int counter = 0;
	while (counter < 4) { /*Skiping Header*/

		if (infile.peek() == ' ' || infile.peek() == '\n') {
			counter++;
		}
		infile.get();
	}

	/* 1.0v Ylopoihsh me Linear dynamiko float* Array ----------------------------------*/
	int size = (*w)*(*h) * 3;
	float* f_array = new float[size];

	float temp_v;
	unsigned char byte;
	int pixels = 0;
	int x = 0;

	while (infile.peek() != EOF) {

		if (infile.good()) {

			byte = (unsigned char)infile.get();
			temp_v = ((float)byte) / 255.0f; 
			
			if (temp_v<0.0f || temp_v>1.0f) {
				std::cerr << "~[Error] : Error occured while reading Image's Data, float_value : " << temp_v << " - got out of [0.0f,1.0f] bounds" << std::endl;
				return nullptr;
			}
			f_array[x] = temp_v;
			x++;
		}
		else {
			std::cerr << "~[Error] : Error Occured while reading Image's Data!" << std::endl;
			return nullptr;
		}
		
	}

	pixels = x / 3;
	std::cout << "-[Report] : Total Bytes read : " << x << std::endl << "-[Report] : Total Pixels read : " << pixels << std::endl << std::endl;

	/* Checking float Array bounds */
	if (x - 1 >= 3 * (*w)*(*h)) {
		std::cerr << "~[Error] : Image <Data> array got out of expected bounds!" << std::endl;
		std::cout << "~[Error] : Image <Data> array expected bounds : < " << 3 * (*w)*(*h) << std::endl;
		std::cout << "~[Error] : Image <Data> array actual bounds : " << x - 1 << std::endl;
		return nullptr;
	}
	return f_array;
	/*------------------------------------------------------------------------------------*/


}




bool imaging::WritePPM(const float * data, int w, int h, const char * filename) {

	return true;

}
