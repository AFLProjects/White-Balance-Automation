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
	/*<-----------FILE SECTION------------->*/

	/*Check if file exists*/
	static bool fileExists(const std::string& name);

	/*Decode png data from file*/
	static std::vector<unsigned char> decodeImage(const char* filename, unsigned int &w, unsigned int &h);

	/*Save PNG file using data*/
	static void encodeImage(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height);

	/*<-----------EDITING SECTION------------->/

	/*Convert vector<unsigned char> to vector<Color>*/
	static void DecodePixels(vector<Color*>& out, vector<unsigned char>* inputImagePtr, int imageSize, int speedUpAmout);

	/*Sort pixels by brigthness using look up table where index = R+G+B*/
	static void SortPixels(vector<vector<Color*>>& outLookupTable, int(&outColCount)[766], vector<Color*>& imageCols, int imageSize, int speedUpAmout);

	/*Find average white color using brightest pixels, using HSV->RGB , RGB->HSV conversions*/
	static void FindWhiteColor(Color& outWhiteColor, vector<vector<Color*>>& outLookupTable, int imageSize, int speedUpAmout);

	/*Apply changes to image*/
	static void ApplyChanges(vector<unsigned char>* outImgPtr, vector<Color*>& imageCols, Color& whiteRef, int imageSize);

	/*Find saturation*/
	static void FindSaturation(vector<Color*>& image, float& outSaturation, int imageSize, int speedupAmout);

	/*Apply changes to image*/
	static void ApplySaturation(vector<unsigned char>* outImgPtr,int imageSize, float targetSaturation, float saturation);
};