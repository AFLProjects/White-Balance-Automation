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
	enum Environement { Green, Red, Blue,Black,White,Neutral,ERROR };
	/*<-----------FILE SECTION------------->*/

	/*Check if file exists*/
	static bool fileExists(const std::string& name);

	/*Decode png data from file*/
	static std::vector<unsigned char> decodeImage(const char* filename, unsigned int &w, unsigned int &h);

	/*Save PNG file using data*/
	static void encodeImage(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height);

	/*<-----------EDITING SECTION------------->*/
	/*Edit Image*/
	static void  EditImage(string path);


	static Environement GetImageEnvironement(Color& avrgColor, Color& avrgColorNoGray);

	/*Convert vector<unsigned char> to vector<Color>*/
	static void DecodePixels(Color out[], vector<unsigned char>& inputImagePtr, int imageSize, int speedUpAmout);

	/*Sort pixels by brigthness using look up table where index = R+G+B*/
	static void SortPixels(vector<vector<Color*>>& outLookupTable, int(&outColCount)[766], Color imageCols[], int imageSize, int speedUpAmout);

	/*Find average white color using brightest pixels, using HSV->RGB , RGB->HSV conversions*/
	static void FindWhiteColor(Color& outWhiteColor, vector<vector<Color*>>& sortedColorsLookupTable, int imageSize, int speedUpAmout, int& outWhiteLimit);

	/*Apply changes to image*/
	static void ApplyChanges(vector<unsigned char>& outImgPtr, Color imageCols[], Color& whiteRef, int imageSize, ImageProcessing::Environement& outEnvironement);

	/*Find saturation*/
	static void FindSaturation(Color image[], float& outSaturation, int imageSize, int speedupAmout);

	/*Apply changes to image*/
	static void ApplySaturation(vector<unsigned char>& outImgPtr, int imageSize, float saturation, ImageProcessing::Environement environement);


};