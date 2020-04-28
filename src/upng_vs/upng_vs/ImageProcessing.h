#include <iostream>
#include <fstream>
#include <string>
#include <lodepng.h>
#include <chrono>
#include <future>
#include <map>
#include "cmath";
#include <iostream>
#include <vector>
#include "Color.h"

using namespace std;

class ImageProcessing 
{
public :
	static bool fileExists(const std::string& name);
	static std::vector<unsigned char> decodeImage(const char* filename, unsigned int &w, unsigned int &h);
	static void encodeImage(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height);

	static void DecodePixels(vector<Color*>& out, vector<unsigned char>* inputImagePtr, int imageSize, bool enableProgressBar);
};