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
		std::cout << "~[Error] /getPixel()/ : Requested [x][y] {R,G,B} is out of expected [" << width<<"] x ["<<height << "] bounds !\n          -Returning default Color{0.0f,0.0f,0.0f} !" << std::endl;
		return Color(0.0f, 0.0f, 0.0f);
	}
	
	else if(buffer == NULL){
		std::cerr << "~[Error] /getPixel()/ : Error occured while trying to access buffer[x][y] !" << std::endl << std::endl;
		return Color(0.0f, 0.0f, 0.0f);
	}

	return buffer[x*width + y];
	
	/* ----------YLOPOIHSH_ALEX--------------  

	if (x > (this->height) || y > (this->width)) std::cerr << "Error, x and y coordinates out of bounds.\n";
	else return buffer[this->height*(x)+y]; 
        }

	*/
}

void Image::setPixel(unsigned int x, unsigned int y, Color & value) {

	if (x*width + y >= height * width) {
		std::cout << "~[Error] /setPixel()/ : Requested [x][y] {R,G,B} is out of expected [" << width << "] x [" << height << "] bounds !\n          -Aborting procedure !" << std::endl;
	}
	else if (buffer == NULL) {
		std::cerr << "~[Error] /setPixel()/ : Error occured while trying to access buffer[x][y] !" << std::endl << std::endl;
	}
	else {
		buffer[x*width + y] = value;
	}


	/* ----------YLOPOIHSH_ALEX--------------  

	if (x >(this->height) || y >(this->width)) std::cerr << "Error, x and y coordinates out of bounds.\n";
	imaging::Image::getPixel(x, y)[0] = value[0]; // buffer.r = value.r
	imaging::Image::getPixel(x, y)[1] = value[1]; // buffer.g = value.g
	imaging::Image::getPixel(x, y)[2] = value[2]; // buffer.b = value.b
    
	*/

}

void Image::setData(const Color * & data_ptr) {


	if (width*height <= 0) {
		std::cerr << "~[Error] /setData()/ : Error while trying to initialise Color * Image::Buffer , [width]x[height] <= 0 " << std::endl;
	}
	else if (buffer == NULL) { /* Color * buffer should already be allocated */
		std::cerr << "~[Error] /setData()/ : Error while trying to initialise Color * Image::Buffer , Image::Buffer has not been allocated ! " << std::endl;
	}

	else {

		for (unsigned int i = 0; i <width*height; i++) {
			buffer[i] = data_ptr[i];
		}
		std::cout << "-[Report] : Color * Buffer initialised succesfully!" << std::endl<<std::endl;
	}

	/* ---- PROHGOUMENH YLOPOIHSH_setData() ----
	
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

	*/
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

//COPY CONSTRUCTOR
Image::Image(const Image &src) {
	/*epeidh einai const to Image , den mporoume na xrhsimopoihsoume getDataPtr , gia na paroume ta stoixeia tou
	Image src k na ta antigrapsoume ston diko mas buffer, opote anagkastika xrhsimopoioyme thn me8odo setPixel k getPixel poy
	einai const me9odoi , k den vgazoun error sto compile.*/
	
	this->width = src.getWidth();
	this->height = src.getHeight();
        
	buffer = new Color[width*height];
	for (unsigned int x = 0; x <height; x++) {
		for (unsigned int y = 0; y < width; y++) {
			this->setPixel(x, y, src.getPixel(x,y));
		}
	}

}

//Destructor
Image::~Image() {
	/* To afhnw se erwthmatiko giati den pistevw oti einai anagkaio na mhdenizoyme to width/height.
	this->width = 0;
	this->height = 0;
	*/
        
	//Prepei na diagrapsoume ta stoixeia tou buffer giati einai dynamika allocated sto heap.
	// k meta vazoume ton buffer -> NULL gia logous sigourias , den einai anagkaio.
	delete[] this->buffer;
	buffer = NULL;
}

bool Image::load(const std::string & filename, const std::string & format) {

	/*Checking if File has the 'ppm' Extension*/
	if (format!="ppm") {
		std::cerr << "~[Error] /load()/ : File is not a supportable Format" << std::endl;
		return false;
	}

	std::ifstream infile;
	/*Trying to open File*/

	
	infile.open(filename, std::ifstream::binary);
	/*Checking if file is open!*/
	if (!infile.is_open()) {
		std::cerr << "~[Error] /load()/ : Error occured while trying to open file .\nFile maybe doesn't exist!" << std::endl;
		return false;
	}

	/*----Wiping out Image's data before Initialization------- ???? --*/
	
	/*----To pros9esa auto epeidh sta sxolia ths Load() anaferei oti ean to antikeimeno Image einai ontws na ginei
	initialised, dhladh plhrei tis proupo8eseis k einai valid (exei format ppm , yparxei to Image.ppm poy suzhteitai na ginei
	load klp..) , leei oti prepei prwta ta stoixeia tou na ginoyn wipe prin to initialization . Fantazomai oti auto ennoei,
	ka9ws den yparxoun alla fields tou Image , gia logous asfaleias pi9anotata to zhtaei auto , wste ean einai na kaneis 2 fores
	load() ap'to idio antikeimeno Image, na mhn ginei kapoio mperdema me ta prohgoumena stoixeia tou Image.
	*/
	width = 0;
	height = 0;
	buffer = NULL;
	/*-----------*/


	/*Reading_Header*/
	unsigned int counter = 0;
	unsigned int pointer = 0;
	std::string line;

	char** header = new char*[4];
	
	/*Counting to 4 'spaces' or '\n' */
	std::cout << std::endl;
	
	while (counter < 4) {
		/*Skiping comments*/
		if (infile.good()) {
			if (infile.peek() == '#') {
			while (infile.peek() != ' ' || infile.peek() != '\n') infile.get(); 
			counter++;
		} ////

			if (infile.peek() != ' ' && infile.peek() != '\n') {
				line += infile.get();
			}
			else {
				/*Edw prin, eftiaxna ena neo pointer se char se ka9e loop toy while , gia na me voh8hsei na kanw
				copy ta stoixeia tou sto header[i] , katalava oti den xreiazetai , epeidh k to header[i] sthn ousia
				einai char * , epeidh o header einai char** , opote htan peritto k oxi mono.Ekatsa k diavasa ti paizei
				otan se ena loop kaneis auto to pramma : Class * variable = new Class(); Auto poy sumvainei einai oti
				se ka9e loop , dhmiourgeitai kainourgio block mnhmhs sto heap sto opoio 9a koitazei ekei pleon o pointer,
				XWRIS omws na apodesmeuetai to prohgoumeno block mnhmhs sto opoio koitaze, opote htan kakisth ylopoihsh
				ek merous moy xwris na to gnwrizw .*/
		
				infile.get();
				header[pointer] = new char[line.length() + 1];
				strcpy_s(header[pointer], line.length() + 1, line.c_str());
				std::cout <<"- Header["<<pointer<<"] : "<< header[pointer] << std::endl; //Printing Header
				counter++;
				pointer++;
				line = "";
			}

		}
		else {
			std::cout<<std::endl;
			std::cerr << "~[Error] /load()/ : Error Occured while reading Header" << std::endl;
			return false;
		}
		
	}
	std::cout << std::endl;

	/*Checking if File is a P6 Format*/
	try {
		if (strcmp(header[0],"P6")!=0) {
			std::cerr << "~[Error] /load()/ : File is not a <P6.ppm> Format" << std::endl;
			return false;
		}
	}
	catch (const char err) {
		std::cerr << "*[Exception] /load()/ : Exception Occured while trying to read Header" << std::endl;
		return false;
	}
	

	/*Casting Header[] to Integer to validate it's data*/
	int iheader[3];
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
		case 0: if (iheader[i] <=0) { std::cerr << "~[Error] /load()/ : Image's Width didn't get initialised or it's [0]!" << std::endl; return false;} break;
		case 1: if (iheader[i] <=0) { std::cerr << "~[Error] /load()/ : Image's Height didn't get initialised or it's [0]!" << std::endl; return false;}break;
		case 2: if (iheader[i] != 255) { std::cerr << "~[Error] /load()/ : Image's Max Value for Color isn't 255!" << std::endl; return false; } break;

		}

	}

	width = iheader[0];
	height = iheader[1];

	std::cout << "-[Report] : Image's width = " << width<<std::endl<< "-[Report] : Image's height = " << height << std::endl;

	/* ~Deleting header**~ */
	for (unsigned int i = 0; i < 4; i++) {
		delete[] header[i];
	}
	delete[] header;
	// ~


	/*readPPM() now can get called*/
	float * read_array = imaging::ReadPPM(filename.c_str(),(iheader),(iheader+1));


	if (read_array == nullptr) { /*Checking if ReadPPM() returned -> nullptr */
		std::cerr << "~[Error] /load()/ : Error occured while trying to read Image data" << std::endl;
		return false;
	}
	std::cout << "-[Report] : float * Array succesfully initialised !" << std::endl;

	

	/*------Initialising Image::Color * buffer / from float * read_array that holds the Image data------*/
	
	/* epeidh ekana 1000 allages k exw 3exasei ti sunevaine akrivws se ka9e block , nomizw
	edw oti ftiaxname enan diko mas color * buff ston opoio pernousame ta dedomena toy float array
	poy mas epestrefe h readPPM() , kai epeita kalousame thn setData() kai etsi pernousame ta data
	tou dikoy mas Color array ston buffer ths Image .
	
	Katalava oti enai teleiws la9os efoson exoume amesh epikoiwnnia me ton buffer efoson eimaste mesa stis
	me9odous ths klasshs Image, k etsi mporoume apla na ton kanoyme initialise apo edw.
	Epishs , auto zhtouse k o idios o Papaiwnaoy kata 99% , dioti sta sxolia ths setData() anaferei oti
	pairnei enan ''External Buffer'' kai ton antigrafei ston internal buffer, kai epishs leei :
	
	"The member function ASSUMES that the input buffer is of a size compatible with the internal storage of the
		*  Image object and that the data buffer has been already allocated. If the image buffer is not allocated or the
		*  width or height of the image are 0, the method should exit immediately."
		
	-Dhladh h setData() den exei kanena dikaiwma na orisei ton buffer ths image , opws kaname emeis ekei poy
	legame : buffer = new Color[width x height]; . THewrei dedomeno oti exei ginei hdh allocated o buffer k
	apla pernaei ta dedomena tou external buffer ston buffer ths Image.
	*/
	
	buffer = new Color[width * height];
	
	pointer = 0;
	counter = 0;
	int size = 3 * (width)*(height);
	float temp_ar[3]{ -1.0f,-1.0f,-1.0f };


	for (int i = 0; i < size; i++) {

		temp_ar[counter] = read_array[i];
		
		if (counter < 2) {
			counter++;
		}
		else {
			
			buffer[pointer] = Color(temp_ar[0], temp_ar[1], temp_ar[2]);
			pointer++;
			counter = 0;
		}
	}

	/* Checking Color Array bounds */
	if (pointer-1 >= width * height) {
		std::cerr << "~[Error] /load()/ : Image::Color * buffer got out of expected bounds !" << std::endl;
		std::cout << "~[Error] /load()/ : Image::Color * buffer's expected bounds : < " << width * height << std::endl;
		std::cout << "~[Error] /load()/ : Image::Color * buffer's actual bounds :" << pointer << std::endl;
		return false;
	}
	std::cout << "-[Report] : Image::Color * buffer's size : " << pointer << std::endl;

	/* ~Deleting float * read_array~ */
	delete[] read_array;
	// ~
	
	/*Final path_returning true<Everything went good>*/
	return true; 
	
}



bool Image::save(const std::string & filename, const std::string & format) {

	if (format != "ppm") {
		std::cerr << "~[Error] /save()/ : Error , Format is not a <ppm> , cannot save file !" << std::endl;
		return false;
	}
	else if (buffer == NULL) {
		std::cerr << "~[Error] /save()/ : Error , Image::Color * buffer points to <NULL> !" << std::endl;
		return false;
	}

	/* Casting Color * buffer to float * array */
	float * f_array = new float[width*height * 3];

	unsigned int pointer = 0;
	for (unsigned int i = 0; i < width*height; i++) {

		for (unsigned int j = pointer; j < pointer + 3; j++) {
			f_array[j] = buffer[i][j - pointer]*255.0f;
		}
		pointer += 3;

	}


	const float * write_array = f_array;
	bool save;
	/*Calling WritePPM() once the float * array is ready */
	save = imaging::WritePPM(write_array, width, height, filename.c_str());

	/* ~Deleting f_array~ */
	/*Btw , apo oti katalava mesa apo peirmatismoys, an exeis 2 pointers pou deixnoun sto idio block mnhmhs,
	opoio k ap'ta duo na diagrapseis , einai to idio pramma. Opote eite diagrapsoume to wirte_array eite to f_array
	einai to idio pramma .
	*/
	
	delete[] f_array; 
	// ~


	if (save) {
		std::cout << "-[Report] : Saving file procedure as a <ppm> format , succesfully completed ! " << std::endl;
		std::cout << "- Image dimensions are [" << width << "]x[" << height << "]" << std::endl;
		return true;
	}
	std::cerr << "~[Error] /save()/ : Saving file procedure ended with errors !" << std::endl << std::endl << std::endl;
	return false;
	
	/*-------------------YLOPOIHSH_ALEX------------------
	/*Checking if File has the 'ppm' Extension

	if (format != "ppm") {
		std::cerr << "File is not a supportable Format" << std::endl;
		return false;
	}
	/*------------ Turning buffer to negative ----------------------

	const imaging::Color white(1, 1, 1); // A Color object that will be used to do the thing {1,1,1}-pixel thing. RGB={1,1,1} means this pixel is white
	for (unsigned int i = 0; i < height; i++) {
		for (unsigned int j = 0; j < width; j++) {
			setPixel(i, j, white - getPixel(i, j));
		}
	}

	/*------------------ from Color buffer to float array -------------------------
	float* farray = new float[height*width * 3];
	for (unsigned int i = 0; i < height*width; i += 3) {
		for (unsigned int counter = 0; counter < 3; counter++) {
			farray[i + counter] = buffer[i][counter];
		}
	}
	const float* write_array = &farray[0];

	/*------------- WritePPM can now get called-----------------------------------
	const char* c = filename.c_str();
	imaging::WritePPM(write_array, width, height, c);
	return true;
	
	*/
}












