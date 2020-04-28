#include "ImageProcessing.h"

#pragma warning(disable:4996)

#pragma region File Managing
bool ImageProcessing::fileExists(const std::string& name) {
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

std::vector<unsigned char> ImageProcessing::decodeImage(const char* filename, unsigned int &w, unsigned int &h) {
	std::vector<unsigned char> image;
	unsigned error = lodepng::decode(image, w, h, filename);
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	return image;
}

void ImageProcessing::encodeImage(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
	unsigned error = lodepng::encode(filename, image, width, height);
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}
#pragma endregion

#pragma region ImageEditing
void ImageProcessing::DecodePixels(vector<Color*>& out, vector<unsigned char>* inputImagePtr, int imageSize, bool enableProgressBar)
{
	int ExtractingWorkFinishedOld = 0;
	int ExtractingWorkFinished = 0;

	for (int i = 0; i < (imageSize * 4) - 1; i += 4)
	{
		Color *col = new Color((float)((*inputImagePtr)[i]), (float)((*inputImagePtr)[i + 1]), (float)((*inputImagePtr)[i + 2]), Color::ColorType::RGB);
		out[i / 4] = col;

		ExtractingWorkFinishedOld = ExtractingWorkFinished;
		ExtractingWorkFinished = (int)round((i / (imageSize * 4.0))*22.0);

		if (ExtractingWorkFinishedOld != ExtractingWorkFinished)
		{
			cout << "Extracting colors (" << ExtractingWorkFinished / 22.0*100.0 << "%)[";
			int workNotFinished = 22 - ExtractingWorkFinished;

			cout << string(ExtractingWorkFinished, '#');
			cout << string(workNotFinished, '-');

			cout << "]\r";
		}
	}
}
#pragma endregion
