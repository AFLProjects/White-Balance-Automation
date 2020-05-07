#include "ImageProcessing.h"

#pragma warning(disable:4996)

#pragma region File Managing
/*Check if file exists*/
bool ImageProcessing::fileExists(const std::string& name) {
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

/*Decode png data from file*/
std::vector<unsigned char> ImageProcessing::decodeImage(const char* filename, unsigned int &w, unsigned int &h) {
	std::vector<unsigned char> image;
	unsigned error = lodepng::decode(image, w, h, filename);
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	return image;
}

/*Save PNG file using data*/
void ImageProcessing::encodeImage(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
	unsigned error = lodepng::encode(filename, image, width, height);
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}
#pragma endregion

#pragma region ImageEditing
/*Convert vector<unsigned char> to vector<Color>*/
void ImageProcessing::DecodePixels(Color out[], vector<unsigned char>& inputImagePtr, int imageSize, int speedUpAmout)
{
	auto start = std::chrono::high_resolution_clock::now();

	const int p = 4 * speedUpAmout;
	const int max = (imageSize * 4) - 1;

	for (int i = 0; i <  max;i += p)
		out[i >> 2] = Color(inputImagePtr[i],inputImagePtr[i + 1],inputImagePtr[i + 2]);

	/*Progress bar & time of execution*/
	auto elapsed_extracting = std::chrono::high_resolution_clock::now() - start;
	float extractionTime = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_extracting).count() / 1000.0;

	int ExtractingkBytesProcessed = sizeof(vector<Color*>) + (sizeof(Color) * imageSize / 1000.0);
	float ExtractingkBytesPerSec = ExtractingkBytesProcessed / extractionTime;

	std::cout << "[" << extractionTime << "s][" << ExtractingkBytesPerSec << "kB/s] " << "Extracting colors (100%)" << endl;
}


void ImageProcessing::SortPixels(vector<vector<Color*>>& outLookupTable, int(&outColCount)[766], Color imageCols[], int imageSize,int speedUpAmout)
{
	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

	Color col;

	for (int i = 0; i < imageSize; i+=speedUpAmout)
	{
		int index = imageCols[i].data[0] + imageCols[i].data[1] + imageCols[i].data[2];
		outLookupTable[index].push_back(&imageCols[i]);
		outColCount[index]++;
	}

	/*Progess bar & time of exec*/
	auto elapsed_sorting = std::chrono::high_resolution_clock::now() - start;
	float sortingTime = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_sorting).count() / 1000.0);

	int SortingkBytesProcessed = (sizeof(vector<vector<Color*>>) + sizeof(Color*) * imageSize) / 1000.0;
	float SortingBytesPerSec = SortingkBytesProcessed / sortingTime;

	std::cout << "[" << sortingTime << "s][" << SortingBytesPerSec << "kB/s] " << "Sorting colors (100%)" << endl;
	/*Progess bar & time of exec end*/
}

void ImageProcessing::FindWhiteColor(float* outWhiteColor, vector<vector<Color*>>& sortedColorsLookupTable, int imageSize,int speedUpAmout,int& outWhiteLimit)
{
	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

	/*Count of "white" pixels i want to produce white reference pixel -> 0.8%*/
	const int whiteCount = round(0.8 / 100.0*imageSize)*speedUpAmout;
	int currentCount = 0;
	bool whiteLimitContinue = true;

	for (int i = 765; i > -1; i--)
	{
		if (currentCount < whiteCount)
		{
			int size = sortedColorsLookupTable[i].size();
			if (size > 0)
			{
				for (int j = 0; j < size; j++)
				{
					if (currentCount < whiteCount)
					{
						float* HSV = (sortedColorsLookupTable[i][j])->RGBtoHSV();
						outWhiteColor[0] += HSV[0];
						outWhiteColor[1] += HSV[1];
						outWhiteColor[2] += HSV[2];
						currentCount++;
					}
					else
					{
						break;
					}
				}
			}
		}
	}

	outWhiteColor[0] /= (float)whiteCount;
	outWhiteColor[1] /= (float)whiteCount;
	outWhiteColor[2] /= (float)whiteCount;

	/*Progress bar*/
	
	auto elapsed_whiteref = std::chrono::high_resolution_clock::now() - start;
	float whiterefTime = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_whiteref).count() / 1000.0);

	std::cout << "[" << whiterefTime << "s] " << "Calculating Avrg. White color(HSV) (100%)" << endl;

	//outWhiteColor.HSVtoRGB();

	/*Progress bar end*/



}

void ImageProcessing::ApplyChanges(vector<unsigned char>& outImgPtr, Color imageCols[], float* whiteRef, int imageSize)
{
	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

	whiteRef = Color::HSVtoRGB(whiteRef);
	whiteRef[0] = 1.0 / (whiteRef[0] / 255.0);
	whiteRef[1] = 1.0 / (whiteRef[1] / 255.0);
	whiteRef[2] = 1.0 / (whiteRef[2] / 255.0);

	if (whiteRef[0] > 3)
		whiteRef[0] = 3;
	if (whiteRef[1] > 3)
		whiteRef[1] = 3;
	if (whiteRef[2] > 3)
		whiteRef[2] = 3;

	float _R_ = 1.0*whiteRef[0];
	float _G_ = 1.0*whiteRef[1];
	float _B_ = 1.0*whiteRef[2];

	for (int i = 0; i < imageSize; i++)
	{
		int R = (int)floor(((float)((outImgPtr)[i * 4]) *  _R_)+0.5);
		int G = (int)floor(((float)((outImgPtr)[(i * 4) + 1]) * _G_)+0.5);
		int B = (int)floor(((float)((outImgPtr)[(i * 4) + 2]) * _B_)+0.5);

		if (R > 255)
			R = 255;
		if (G > 255)
			G = 255;
		if (B > 255)
			B = 255;

		(outImgPtr)[i * 4] = (char)R;
		(outImgPtr)[(i * 4) + 1] = (char)G;
		(outImgPtr)[(i * 4) + 2] = (char)B;
	}

	/*Progess bar*/
	auto elapsed_apply = std::chrono::high_resolution_clock::now() - start;
	float applyTime = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_apply).count() / 1000.0);

	std::cout << "[" << applyTime << "s] " << "Applying changes (100%)" << endl;
	/*Progress bar end*/
}

void  ImageProcessing::EditImage(string path) {

	unsigned int w = 0;
	unsigned int h = 0;

	/*Decode image from file*/
	std::vector<unsigned char> image = ImageProcessing::decodeImage(path.c_str(), w, h);

	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

															/*Size of image in memory*/
	float sizeMb = 3.0 * 32.0 * w * h / 1000.0 / 1000.0;
	/*3x times because Color class has 3 floats so it is 96bit per col instead of 32bit , so 3 times more*/

	cout << "Image decoded from [" << path << "][w:" << w << ";h:" << h << "][" << sizeMb << "MB]\n\n";

	const int imageSize = w * h;

	int speedUp = 2; /*Loss of precision,by skipping pixels*/

					 /*Convert from char to Color class*/
	Color* imageCols = new Color[imageSize];
	ImageProcessing::DecodePixels(imageCols, image, imageSize, speedUp);

	/*Sort colors using lookup table where the index is R+G+B*/
	vector<vector<Color*>> sortedColorsLookupTable(766); /*Multiple color can have the same sum*/

														 /*Count of occurences for each sum value*/
	int colCount[766];
	memset(colCount, 0, sizeof(colCount));

	ImageProcessing::SortPixels(sortedColorsLookupTable, colCount, imageCols, imageSize, speedUp);

	/*Image white pixel*/
	int whiteLimitVal = 0;
	float *referenceWhite = new float[3]{ 0,0,0 };
	ImageProcessing::FindWhiteColor(referenceWhite, sortedColorsLookupTable, imageSize, speedUp, whiteLimitVal);

	cout << "White limit : " << whiteLimitVal << endl;

	ImageProcessing::ApplyChanges(image, imageCols, referenceWhite, imageSize);

	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

	printf("\nTotal time of execution(without decoding/encoding) : ");
	cout << milliseconds << "ms" << endl;

	cout << "Saving image...\n";
	ImageProcessing::encodeImage((path).c_str(), image, w, h);
	cout << "Image Saved!\n" << endl;
}

#pragma endregion
