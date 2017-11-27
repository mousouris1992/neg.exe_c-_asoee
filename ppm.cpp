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
		std::cerr << "~[Error] /readPPM()/ : Error while trying to open PPM file!" << std::endl;
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

	
	/*---------Reading_Bytes-------------------------*/
	/*---Reading bytes to a char * array helps the procedure perform faster --*/
	int size = (*w)*(*h) * 3;
	char * bytes = new char[size];


	infile.read(bytes, size); /*reading bytes from file, in a block of size [width]x[height]x[3]*/

	/* Checking if next byte is EOF , Afoy teleiwsei to read() , koitazoyme ean to epomeno akrivws byte einai to EOF
	k efoson einai , tote ola leitourgoun swsta, mporeis na 9ewrhseis k auto san ena eidos poiohtikoy elegxou ean k den einai
	ana byte */
	
	if (infile.peek() != EOF) { 
		std::cerr << "~[Error] /readPPM()/ : Error occured while reading Image's data , [width] x [height] x [3] size isn't compatible with the read block !" << std::endl << std::endl;
		return nullptr;
	}

	infile.close();

	/*---------Casting Char * bytes to float * Array-------------*/

	float* f_array = new float[size];

	for (int i = 0; i < size; i++) {
		
		f_array[i] = ((float)((unsigned char)bytes[i]))/255.0f;
	}

	//std::cout << "-[Report] : Total Bytes read : " << size << std::endl << "-[Report] : Total Pixels read : " << size/3 << std::endl << std::endl;

	return f_array;

	/*------------------Older version, reading byte by byte from file, a lot slower---------------
	pixels = 0;
	float temp_v;
	unsigned char byte;
	int x = 0;

	while (infile.peek() != EOF) {

		if (infile.good()) {

			byte = (unsigned char)infile.get();
			temp_v = ((float)byte) / 255.0f; 
			
			if (temp_v<0.0f || temp_v>1.0f) {
				std::cerr << "~[Error] /readPPM()/ : Error occured while reading Image's Data, float_value : " << temp_v << " - got out of [0.0f,1.0f] bounds" << std::endl;
				return nullptr;
			}
			f_array[x] = temp_v;
			x++;
		}
		else {
			std::cerr << "~[Error] /readPPM()/ : Error Occured while reading Image's Data!" << std::endl;
			return nullptr;
		}
		
	}

	/* Checking float Array bounds
	if (x - 1 >= 3 * (*w)*(*h)) {
		std::cerr << "~[Error] /readPPM()/ : Image <Data> array got out of expected bounds!" << std::endl;
		std::cout << "~[Error] /readPPM()/ : Image <Data> array expected bounds : < " << 3 * (*w)*(*h) << std::endl;
		std::cout << "~[Error] /readPPM()/ : Image <Data> array actual bounds : " << x - 1 << std::endl;
		return nullptr;
	}

	*/

}




bool imaging::WritePPM(const float * data, int w, int h, const char * filename) {
	
	if (data == nullptr) {
		std::cerr << "~[Error] /WritePPM()/ : float * data points to nullptr !" << std::endl << std::endl;
		return false;
	}

	/* Intialising char * bytes Array that will help perform the writing procedure faster */
	char * bytes = new char[w*h * 3];
	for (int i = 0; i < w*h * 3; i++) {
		bytes[i] = (char)data[i];
	}

	std::ofstream outfile;
	outfile.open(filename, std::ios::binary);
	if (!outfile.is_open()) {
		std::cerr << "~[Error] /WritePPM()/ : Error occured while trying to open new File !" << std::endl << std::endl;
		return false;
	}

	/*Writing Header*/
	
	/*Edw sthn kuriole3ia den yparxei kanenas elegxos , an k o monos elegxos poy uphrxe prin htan to file.good() , to opoio
	einai arketa geniko . Den epsaksa na soy pw thn alh8eia , ean mporoume na enta3oume kapoion elegxo kata thn diarkeia
	ths write an k 9ewrw pws einai ashmanto ka9ws gnwrizoyme akrivws to size twn bytes poy 9eloume na grapsoume.
	Isws enas elegxos na htan na tsekaroume oti to width , height klp einai >0 prin mpoume sthn read. */
	
	outfile << "P6 " << w << "\n" << h << " " << "255\n";
	outfile.write(&bytes[0], w*h * 3);
	outfile.close();

	return true;

	/*Older Version_Writing byte one by one_much slower*/

	/*float temp;
	char byte;
	for (int i = 0; i < w*h * 3; i++) {
		if (!outfile.good()) {
			std::cerr << "~[Error] /WritePPM()/ : Error occured while trying to write the image data to File !" << std::endl<<std::endl;
			return false;
		}
		temp = data[i] * 255.0f;
		byte = (char)temp;
		outfile << byte;	
	}*/


	/*-----------------YLOPOIHSH_ALEX------------------
	if (data = nullptr) {
	std::cerr << "Error, could not write neg file.\n";
	return true;
	}

	std::ofstream outfile; // starting open file operation

	// New name of the file that will be written. Kind(!) of like: Image01_neg.ppm
	std::string newfilename = filename;
	size_t pos = newfilename.find(".");
	newfilename.substr(pos)+="_neg.ppm"; // NA PEIRAMATISTW STO DEV NA KSERW OTI EINAI SWSTO

	outfile.open(newfilename, std::ofstream::out);
	*/
}

