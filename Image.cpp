#include "Image.h"
#include "Color.h"
#include <iostream>
#include <string>
#include "ppm\ppm.h"
#include <string.h>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <algorithm>


using namespace imaging;


Color * Image::getRawDataPtr() {

	return buffer;
}



Color Image::getPixel(unsigned int x, unsigned int y) const {
	
	if (x*width + y >= height * width) {
		std::cout << "~[Error] : Requested [x][y] {R,G,B} is out of expected [" << width<<"] x ["<<height << "] bounds !\n          -Returning default Color{0,0,0} !" << std::endl;
		return Color(0, 0, 0);
	}

	return buffer[x*width + y];

}

void Image::setPixel(unsigned int x, unsigned int y, Color & value) {

	if (x*width + y >= height * width) {
		std::cout << "~[Error] : Requested [x][y] {R,G,B} is out of expected [" << width << "] x [" << height << "] bounds !\n          -Aborting procedure !" << std::endl;
	}
	else {
		buffer[x*width + y] = value;
	}

}

void Image::setData(const Color * & data_ptr) {


	
	if (width*height <= 0) {
		std::cerr << "~[Error] : Error while trying to initialise Color * Image::Buffer , size is <=0 " << std::endl;
	}
	else {

		buffer = new Color[width*height];

		for (unsigned int i = 0; i <width*height; i++) {
			buffer[i] = data_ptr[i];
		}
		std::cout << "-[Report] : Color * Buffer initialised succesfully!" << std::endl<<std::endl;
	}
	
	

}




Image::Image()
	: width(0), height(0), buffer(nullptr)
{

}


Image::Image(unsigned int width, unsigned int height) {

	this->width = width;
	this->height = height;
}

Image::Image(unsigned int width, unsigned int height, const Color * data_ptr) {

	this->width = width;
	this->height = height;

}

Image::Image(const Image &src) {

}

Image::~Image() {

}

bool Image::load(const std::string & filename, const std::string & format) {


	/*Checking if File has the 'ppm' Extension*/
	if (format!="ppm") {
		std::cerr << "~[Error] : File is not a supportable Format" << std::endl;
		return false;
	}

	std::ifstream infile;
	/*Trying to open File*/

	
	infile.open(filename, std::ifstream::binary);
	/*Checking if file is open!*/
	if (!infile.is_open()) {
		std::cerr << "~[Error] : Error occured while trying to open file .\nFile maybe doesn't exist!" << std::endl;
		return false;
	}

	/*Reading_Header*/

	unsigned int counter = 0;
	unsigned int pointer = 0;
	std::string line;

	char** header = new char*[4];
	

	/*Counting to 4 'spaces' or '\n' */
	while (counter < 4) {

		if (infile.good()) {

			if (infile.peek() != ' ' && infile.peek() != '\n') {
				line += infile.get();
			}
			else {
				infile.get();
				char * temp_c = new char[line.length() + 1];
				strcpy_s(temp_c, line.length() + 1, line.c_str());
				header[pointer] = temp_c;
				std::cout <<"Header["<<pointer<<"] : "<< header[pointer] << std::endl << std::endl; //Printing Header
				counter++;
				pointer++;
				line = "";
			}

		}
		else {
			std::cerr << "~[Error] : Error Occured while reading Header" << std::endl;
			return false;
		}
		
	}

	
	/*Checking if File is a P6 Format*/
	try {
		if (strcmp(header[0],"P6")!=0) {
			std::cerr << "~[Error] : File is not a <P6.ppm> Format" << std::endl;
			return false;
		}
	}
	catch (const char err) {
		std::cerr << "*[Exception] : Exception Occured while trying to read Header" << std::endl;
		return false;
	}
	

	/*Casting Header[] to Integer to validate it's data*/

	int* iheader = new int[3];
	for (unsigned int i = 0; i < 3; i++) {
		iheader[i] = 0;
	}


	int power;
	int temp_v;
	for (int i = 1; i < 4; i++) {

		iheader[i - 1] = 0;
		line = header[i];
		power = line.length() - 1;

		for (unsigned int j = 0; j < line.length(); j++) {
			temp_v = (*(header[i] + j) - '0')*(int)pow(10.0, power);
			iheader[i - 1] += temp_v;
			power--;
		}

	}

	
	
	

	/*Checking Header's informations validity*/
	for (unsigned int i = 0; i < 3; i++) {

		switch (i) {
		case 0: if (iheader[i] <=0) { std::cerr << "~[Error] : Image's Width didn't get initialised or it's [0]!" << std::endl; return false;} break;
		case 1: if (iheader[i] <=0) { std::cerr << "~[Error] : Image's Height didn't get initialised or it's [0]!" << std::endl; return false;}break;
		case 2: if (iheader[i] != 255) { std::cerr << "~[Error] : Image's Max Value for Color isn't 255!" << std::endl; return false; } break;

		}

	}

	width = iheader[0];
	height = iheader[1];

	std::cout << "-[Report] : Image's width = " << width<<std::endl<< "-[Report] : Image's height = " << height << std::endl;


	/*readPPM() now can get called*/
	const char* c = filename.c_str();
	
	float * read_array = imaging::ReadPPM(c, (iheader), (iheader+1));

	if (read_array == nullptr) { /*Checking if ReadPPM() returned -> nullptr */
		std::cerr << "~[Error] : Error occured while trying to read Image data" << std::endl;
		return false;
	}

	std::cout << "-[Report] : float * Array succesfully initialised !" << std::endl;

	/*for (int i = 0; i < 3 * (iheader[0])*(iheader[1]); i++) {
		std::cout << read_array[i] << std::endl; 
	}
	Checking float * read_array values* /


	/*------Constructing <Color * Buffer> from float * read_array that holds the Image data------*/
	Color * buff = new Color[iheader[0] * iheader[1]];
	
	pointer = 0;
	counter = 0;
	int size = 3 * (iheader[0])*(iheader[1]); // size = 3 * width * height ;
	float temp_ar[3]{ -1.0f,-1.0f,-1.0f };


	for (int i = 0; i < size; i++) {

		temp_ar[counter] = read_array[i];
		
		if (counter < 2) {
			counter++;
		}
		else {
			
			Color color(temp_ar[0], temp_ar[1], temp_ar[2]);
			buff[pointer] = color;
			pointer++;
			counter = 0;
		}
	}

	/* Checking Color Array bounds */
	if (pointer-1 >= iheader[0] * iheader[1]) {
		std::cerr << "~[Error] : Image's <Color> Array got out of expected bounds !" << std::endl;
		std::cout << "~[Error] : Image's <Color> Array expected bounds : < " << iheader[0] * iheader[1] << std::endl;
		std::cout << "~[Error] : Image's <Color> actual bounds :" << pointer << std::endl;
		return false;
	}
	std::cout << "-[Report] : Color * Array size : " << pointer << std::endl;

	/*Passing Color * buff to Image's Instance::Color * Buffer through Image::setData() */
	const Color * cnst_buff = buff;
	setData(cnst_buff);


	return true; /*Final path returning true<Everything went good>*/
}



bool Image::save(const std::string & filename, const std::string & format) {



	return true;
}












