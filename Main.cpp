#include <iostream>
#include "Image.h"
#include "ppm\ppm.h"
#include <string>
#include <string.h>
#include <algorithm>

using namespace std;


int main(int argc, char *argv[]) {

	string filename;

	if (argc == 2) {
		if (argv[1] != nullptr) {
			try {
				filename = argv[1];
			}
			catch (const char e) {
				cerr << "Exception Occured while trying to read arguments!" << endl;
				return 0;}	
		}
		else { cerr << "Invalid Argument" << endl; return 0; }
	}
	else {

		cout << "File name of the Image to load : ";
		cin >> filename;
		cout << endl;
	}

	/*Extracting format from Filename*/
	size_t pos = filename.find(".");
	string format = filename.substr(pos+1);
	transform(format.begin(), format.end(), format.begin(), ::tolower);
	//filename = "images/" + filename;
	/* - - - - - */
	
	imaging::Image img;
	img.load(filename, format);

	imaging::Color def_color(1.0f, 1.0f, 1.0f);

	/* ___v1.0___ : Transforming Image to Neg without using getRawData()\setData() 
	 
	for (int i = 0; i < img.getHeight(); i++) {
		for (int j = 0; j < img.getWidth(); j++) {

			//img.setPixel(i, j, def_color - img.getPixel(i, j));
			/*___v1.2___ 
			imaging::Color pixel = img.getPixel(i, j);
			img.setPixel(i, j, imaging::Color(1.0f - pixel[0], 1.0f - pixel[1], 1.0f - pixel[2]));
			
		}
	}

	/*-----------------------------------------------------------------------------*/

	/* ___v2.0___ : Transforming Image to Neg by using getDataPtr()\setDataPtr() [FASTER]
	  */
	imaging::Color * c_ar = img.getRawDataPtr();
	int size = img.getHeight()*img.getWidth();

	for (int i = 0; i < size; i++) {

		c_ar[i] = def_color - c_ar[i];
	}
	const imaging::Color * c_const_ar = c_ar;
	img.setData(c_const_ar);
	
	/*--------------------------------------------------*/
  

	filename = filename.substr(0, pos);
	filename = filename + "_neg.ppm";
	format = "ppm";

	if (!img.save(filename, format)) {
		system("pause");
		return -1;
	}

	system("pause");

	return 0;
}
