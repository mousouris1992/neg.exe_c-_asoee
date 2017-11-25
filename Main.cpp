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

	imaging::Image img;
	img.load(filename, format);

	/* v1----->Transforming Image to Neg 1.0v from main without copying getRawData() etc.*/

	int size = img.getHeight()*img.getWidth();
	//imaging::Color * color_data = img.getRawDataPtr();


	/*for (int i = 0; i < img.getHeight(); i++) {
		for (int j = 0; j < img.getWidth(); j++) {

			imaging::Color pixel = img.getPixel(i, j);
			img.setPixel(i, j, imaging::Color(1.0f - pixel[0], 1.0f - pixel[1], 1.0f - pixel[2])); //--v1.2 na xrhsimopoihsoume Color.+/- gia tis pra3eis.

		}
	}

	imaging::Color * color_data = img.getRawDataPtr();
	
	/*for (int i = 0; i < size; i++) {
		for (int j = 0; j < 3; j++) {
			cout << color_data[i][j] << endl;
		}
		cout << "Press any key to continue";
		system("pause");
	}
	*/

	/*v2------->Transforming image to neg through copying ar from getRawData but not using Images.methods() this way*/


	



	int a;
	cin >> a;


	return 0;
}