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
void ImageProcessing::DecodePixels(vector<Color*>& out, vector<unsigned char>* inputImagePtr, int imageSize, int speedUpAmout)
{
	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

	int ExtractingWorkFinishedOld = 0; /*Progress bar value old*/
	int ExtractingWorkFinished = 0; /*Progress bar value*/

	/*Convert all the colors to Color class*/
	int p = 4 * speedUpAmout;
	for (int i = 0; i < (imageSize * 4) - 1; i += p)
	{
		/*Color extracting*/
		Color *col = new Color((float)((*inputImagePtr)[i]), (float)((*inputImagePtr)[i + 1]), (float)((*inputImagePtr)[i + 2]), Color::ColorType::RGB);
		out[i / 4] = col;
		/*Color extracting end*/

		/*Progress bar*/
		ExtractingWorkFinishedOld = ExtractingWorkFinished;
		ExtractingWorkFinished = (int)round((i / (imageSize * 4.0))*22.0);

		if (ExtractingWorkFinishedOld != ExtractingWorkFinished) {
			std::cout << "Extracting colors (" << ExtractingWorkFinished / 22.0*100.0 << "%)[";
			int workNotFinished = 22 - ExtractingWorkFinished;

			std::cout << string(ExtractingWorkFinished, '#');
			std::cout << string(workNotFinished, '-');

			std::cout << "]\r";
		}
		/*Progress bar end*/

	}

	/*Progress bar & time of execution*/
	auto elapsed_extracting = std::chrono::high_resolution_clock::now() - start;
	float extractionTime = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_extracting).count() / 1000.0;

	int ExtractingkBytesProcessed = sizeof(vector<Color*>) + (sizeof(Color) * out.size() / 1000.0);
	float ExtractingkBytesPerSec = ExtractingkBytesProcessed / extractionTime;

	std::cout << "[" << extractionTime << "s][" << ExtractingkBytesPerSec << "kB/s] " << "Extracting colors (100%)[##########################]" << endl;

}


/*Sort pixels by brigthness using look up table where index = R+G+B*/
void ImageProcessing::SortPixels(vector<vector<Color*>>& outLookupTable, int(&outColCount)[766], vector<Color*>& imageCols, int imageSize,int speedUpAmout)
{
	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

	/*Progress bar values*/
	int SortingWorkFinishedOld = 0;
	int SortingWorkFinished = 0;

	Color col;

	for (int i = 0; i < imageSize; i+=speedUpAmout)
	{
		/*Get col*/
		col = *(imageCols[i]);
		int index = (int)(col.data[0] + col.data[1] + col.data[2]); /*Find index R+G+B*/
		outLookupTable[index].push_back(imageCols[i]); /*Push color*/
		/*Tutaj spóbuje zrobić array i nie robić pusz_back bo jest wolny*/

		/*Add color count*/
		outColCount[index] += 1;

		/*Progess bar*/
		SortingWorkFinishedOld = SortingWorkFinished;
		SortingWorkFinished = (int)round(((i*1.0) / (imageSize * 1.0))*22.0);

		if (SortingWorkFinishedOld != SortingWorkFinished) {
			std::cout << "Sorting colors (" << SortingWorkFinished / 22.0*100.0 << "%)[";
			int workNotFinished = 22 - SortingWorkFinished;

			std::cout << string(SortingWorkFinished, '#');
			std::cout << string(workNotFinished, '-');

			std::cout << "]\r";
		}
		/*Progress bar end*/
	}

	/*Progess bar & time of exec*/
	auto elapsed_sorting = std::chrono::high_resolution_clock::now() - start;
	float sortingTime = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_sorting).count() / 1000.0);

	int SortingkBytesProcessed = (sizeof(vector<vector<Color*>>) + sizeof(Color*) * imageCols.size()) / 1000.0;
	float SortingBytesPerSec = SortingkBytesProcessed / sortingTime;

	std::cout << "[" << sortingTime << "s][" << SortingBytesPerSec << "kB/s] " << "Sorting colors (100%)[##########################]" << endl;
	/*Progess bar & time of exec end*/
}

void ImageProcessing::SortPixels(vector<vector<unsigned char*>>& outLookupTable, int(&outColCount)[766], vector<unsigned char>& imageCols, int imageSize)
{
	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

	/*Progress bar values*/
	int SortingWorkFinishedOld = 0;
	int SortingWorkFinished = 0;

	for (int i = 0; i < imageSize; i++)
	{
		/*Get col*/
		//Color col = *(imageCols[4*i]);
	//	int index = (int)(col.data[0] + col.data[1] + col.data[2]); /*Find index R+G+B*/

		/*Push color*/
		int index = imageCols[4 * i] + imageCols[(4 * i) + 1] + imageCols[(4 * i) + 2];

		outLookupTable[index].push_back(&(imageCols[4 * i]));
		outLookupTable[index].push_back(&(imageCols[(4 * i) + 1]));
		outLookupTable[index].push_back(&(imageCols[(4 * i) + 2]));

		 /*Tutaj spóbuje zrobić array i nie robić pusz_back bo jest wolny*/

													   /*Add color count*/
		outColCount[index] += 1;

		/*Progess bar*/
		SortingWorkFinishedOld = SortingWorkFinished;
		SortingWorkFinished = (int)round(((i*1.0) / (imageSize * 1.0))*22.0);

		if (SortingWorkFinishedOld != SortingWorkFinished) {
			std::cout << "Sorting colors (" << SortingWorkFinished / 22.0*100.0 << "%)[";
			int workNotFinished = 22 - SortingWorkFinished;

			std::cout << string(SortingWorkFinished, '#');
			std::cout << string(workNotFinished, '-');

			std::cout << "]\r";
		}
		/*Progress bar end*/
	}

	/*Progess bar & time of exec*/
	auto elapsed_sorting = std::chrono::high_resolution_clock::now() - start;
	float sortingTime = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_sorting).count() / 1000.0);

	int SortingkBytesProcessed = (sizeof(vector<vector<Color*>>) + sizeof(Color*) * imageCols.size()) / 1000.0;
	float SortingBytesPerSec = SortingkBytesProcessed / sortingTime;

	std::cout << "[" << sortingTime << "s][" << SortingBytesPerSec << "kB/s] " << "Sorting colors (100%)[##########################]" << endl;
	/*Progess bar & time of exec end*/
}

/*Find average white color using brightest pixels, using HSV->RGB , RGB->HSV conversions*/
void ImageProcessing::FindWhiteColor(Color& outWhiteColor, vector<vector<Color*>>& sortedColorsLookupTable, int imageSize,int speedUpAmout)
{
	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

	/*Count of "white" pixels i want to produce white reference pixel -> 0.8%*/
	int whiteCount = round(0.8 / 100.0*imageSize)*speedUpAmout;
	int currentCount = 0;

	/*Progess bar values*/
	int WhiteRefWorkFinishedOld = 0;
	int WhiteRefWorkFinished = 0;

	/*Go over each sum R+G+B*/
	for (int i = 765; i > -1; i--)
	{
		/*If we haven't achieved need white count*/
		if (currentCount < whiteCount)
		{
			/*Get the count of colors with that sum*/
			int size = sortedColorsLookupTable[i].size();
			if (size > 0) /*If there are*/
			{
				/*Go over each one of them*/
				for (int j = 0; j < size; j++)
				{
					if (currentCount < whiteCount)
					{
						Color col = *(sortedColorsLookupTable[i][j]);
						currentCount++;

						/*Convert to HSV color space*/
						col.RGBtoHSV();

						/*Average white color*/
						outWhiteColor.data[0] += col.data[0] / whiteCount;
						outWhiteColor.data[1] += col.data[1] / whiteCount;
						outWhiteColor.data[2] += col.data[2] / whiteCount;

						/*Back to RGB*/
						col.HSVtoRGB();

						/*Progress bar*/
						WhiteRefWorkFinishedOld = WhiteRefWorkFinished;
						WhiteRefWorkFinished = (int)round(((currentCount*1.0) / (whiteCount * 1.0))*22.0);

						if (WhiteRefWorkFinishedOld != WhiteRefWorkFinished) {
							std::cout << "Calculating Avrg. White color(HSV) (" << WhiteRefWorkFinished / 22.0*100.0 << "%)[";
							int workNotFinished = 22 - WhiteRefWorkFinished;

							std::cout << string(WhiteRefWorkFinished, '#');
							std::cout << string(workNotFinished, '-');

							std::cout << "]\r";
						}
						/*Progress bar end*/
					}
					else
					{
						break;
					}
				}
			}
		}
	}


	/*Progress bar*/
	
	auto elapsed_whiteref = std::chrono::high_resolution_clock::now() - start;
	float whiterefTime = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_whiteref).count() / 1000.0);

	std::cout << "[" << whiterefTime << "s] " << "Calculating Avrg. White color(HSV) (100%)[##########################]" << endl;

	outWhiteColor.HSVtoRGB();
	outWhiteColor = Color(1 / (outWhiteColor.data[0] / 255.0), 1 / (outWhiteColor.data[1] / 255.0), 1 / (outWhiteColor.data[2] / 255.0), Color::ColorType::RGB);

	/*Progress bar end*/

}

/*Find average white color using brightest pixels, using HSV->RGB , RGB->HSV conversions*/
void ImageProcessing::FindWhiteColor(Color& outWhiteColor, vector<vector<unsigned char*>>& sortedColorsLookupTable, int imageSize)
{
	auto start = std::chrono::high_resolution_clock::now(); /*timer*/
									/*Count of "white" pixels i want to produce white reference pixel -> 0.8%*/
	int whiteCount = round(0.8 / 100.0*imageSize);
	int currentCount = 0;

	/*Progess bar values*/
	int WhiteRefWorkFinishedOld = 0;
	int WhiteRefWorkFinished = 0;

	/*Go over each sum R+G+B*/
	for (int i = 765; i > -1; i--)
	{
		/*If we haven't achieved need white count*/
		if (currentCount < whiteCount)
		{
			/*Get the count of colors with that sum*/
			int size = sortedColorsLookupTable[i].size() / 3; /*3 for RGB*/
			if (size > 0) /*If there are*/
			{
				/*Go over each one of them*/
				for (int j = 0; j < size; j++)
				{
					if (currentCount < whiteCount)
					{
						unsigned char R  = *(sortedColorsLookupTable[i][3 * j]);
						unsigned char G = *(sortedColorsLookupTable[i][(3 * j) + 1]);
						unsigned char B = *(sortedColorsLookupTable[i][(3 * j) + 2]);
						currentCount++;

						/*Convert to HSV color space*/
						Color col = Color(R, G, B,Color::ColorType::RGB);
						col.RGBtoHSV();

						/*Average white color*/
						outWhiteColor.data[0] += col.data[0] / whiteCount;
						outWhiteColor.data[1] += col.data[1] / whiteCount;
						outWhiteColor.data[2] += col.data[2] / whiteCount;

						/*Back to RGB*/
						col.HSVtoRGB();
						//delete &col;

						/*Progress bar*/
						WhiteRefWorkFinishedOld = WhiteRefWorkFinished;
						WhiteRefWorkFinished = (int)round(((currentCount*1.0) / (whiteCount * 1.0))*22.0);

						if (WhiteRefWorkFinishedOld != WhiteRefWorkFinished) {
							std::cout << "Calculating Avrg. White color(HSV) (" << WhiteRefWorkFinished / 22.0*100.0 << "%)[";
							int workNotFinished = 22 - WhiteRefWorkFinished;

							std::cout << string(WhiteRefWorkFinished, '#');
							std::cout << string(workNotFinished, '-');

							std::cout << "]\r";
						}
						/*Progress bar end*/
					}
					else
					{
						break;
					}
				}
			}
		}
	}


	/*Progress bar*/

	auto elapsed_whiteref = std::chrono::high_resolution_clock::now() - start;
	float whiterefTime = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_whiteref).count() / 1000.0);

	std::cout << "[" << whiterefTime << "s] " << "Calculating Avrg. White color(HSV) (100%)[##########################]" << endl;

	outWhiteColor.HSVtoRGB();
	outWhiteColor = Color(1 / (outWhiteColor.data[0] / 255.0), 1 / (outWhiteColor.data[1] / 255.0), 1 / (outWhiteColor.data[2] / 255.0), Color::ColorType::RGB);

	/*Progress bar end*/

}

/*Apply changes to image*/
void ImageProcessing::ApplyChanges(vector<unsigned char>* outImgPtr, vector<Color*>& imageCols, Color& whiteRef, int imageSize) 
{
	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

	/*Progess bar values*/
	int ApplyWorkFinishedOld = 0;
	int ApplyWorkFinished = 0;

	for (int i = 0; i < imageSize; i++)
	{
		/*
		Color* col;
		col = imageCols[i];

		
		if (whiteRef.data[0] > 3)
			whiteRef.data[0] = 3;
		if (whiteRef.data[1] > 3)
			whiteRef.data[1] = 3;
		if (whiteRef.data[2] > 3)
			whiteRef.data[2] = 3;

		(*col).data[0] *= whiteRef.data[0];
		(*col).data[1] *= whiteRef.data[1];
		(*col).data[2] *= whiteRef.data[2];

		if ((*col).data[0] >= 255)
			(*col).data[0] = 255;

		if ((*col).data[1] >= 255)
			(*col).data[1] = 255;

		if ((*col).data[2] >= 255)
			(*col).data[2] = 255;

		(*outImgPtr)[i * 4] = (char)round((*col).data[0]);
		(*outImgPtr)[(i * 4) + 1] = (char)round((*col).data[1]);
		(*outImgPtr)[(i * 4) + 2] = (char)round((*col).data[2]);*/

		if (whiteRef.data[0] > 3)
			whiteRef.data[0] = 3;
		if (whiteRef.data[1] > 3)
			whiteRef.data[1] = 3;
		if (whiteRef.data[2] > 3)
			whiteRef.data[2] = 3;

		int R = (int)round((*outImgPtr)[i * 4] * whiteRef.data[0]);
		int G = (int)round((*outImgPtr)[(i * 4) + 1] * whiteRef.data[1]);
		int B = (int)round((*outImgPtr)[(i * 4) + 2] * whiteRef.data[2]);

		if (R > 255)
			R = 255;
		if (G > 255)
			G = 255;
		if (B > 255)
			B = 255;

		(*outImgPtr)[i * 4] = (char)R;
		(*outImgPtr)[(i * 4) + 1] = (char)G;
		(*outImgPtr)[(i * 4) + 2] = (char)B;

		/*Progess bar*/
		ApplyWorkFinishedOld = ApplyWorkFinished;
		ApplyWorkFinished = (int)round(((i*1.0) / (imageSize * 1.0))*22.0);

		if (ApplyWorkFinishedOld != ApplyWorkFinished) {
			std::cout << "Applying changes (" << ApplyWorkFinished / 22.0*100.0 << "%)[";
			int workNotFinished = 22 - ApplyWorkFinished;

			std::cout << string(ApplyWorkFinished, '#');
			std::cout << string(workNotFinished, '-');

			std::cout << "]\r";
		}
		/*Progress bar end*/
	}

	/*Progess bar*/
	auto elapsed_apply = std::chrono::high_resolution_clock::now() - start;
	float applyTime = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_apply).count() / 1000.0);

	std::cout << "[" << applyTime << "s] " << "Applying changes (100%)[##########################]" << endl;
	/*Progress bar end*/
	
}

/*Apply changes to image*/
void ImageProcessing::ApplyChanges(vector<unsigned char>* outImgPtr, vector<unsigned char>& imageCols, Color& whiteRef, int imageSize)
{
	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

															/*Progess bar values*/
	int ApplyWorkFinishedOld = 0;
	int ApplyWorkFinished = 0;

	for (int i = 0; i < imageSize; i++)
	{
		//Color* col;

		/*col = imageCols[i];*/

		/*If white color multiplication coefficient exceeds x3 , go for x3*/

		/*Apply white ref*/


		/*
		(*col).data[0] *= whiteRef.data[0];
		(*col).data[1] *= whiteRef.data[1];
		(*col).data[2] *= whiteRef.data[2];*/

		/*If 255 is exeeded, change it*/
		/*
		if ((*col).data[0] >= 255)
			(*col).data[0] = 255;

		if ((*col).data[1] >= 255)
			(*col).data[1] = 255;

		if ((*col).data[2] >= 255)
			(*col).data[2] = 255;*/

		unsigned char* pR = &(*outImgPtr)[i * 4];
		unsigned char* pG = &(*outImgPtr)[(i * 4) + 1];
		unsigned char* pB = &(*outImgPtr)[(i * 4) + 2];

		int R_ = round(*pR * whiteRef.data[0]);
		int G_ = round(*pG * whiteRef.data[1]);
		int B_ = round(*pB * whiteRef.data[2]);

		if (R_ > 255)
			R_ = 255;
		if (G_ > 255)
			G_ = 255;
		if (B_ > 255)
			B_ = 255;

		/*Apply values to initial image ptr*/
		*pR = (char)R_;
		*pG = (char)G_;
		*pB = (char)B_;

		/*Progess bar*/
		ApplyWorkFinishedOld = ApplyWorkFinished;
		ApplyWorkFinished = (int)round(((i*1.0) / (imageSize * 1.0))*22.0);

		if (ApplyWorkFinishedOld != ApplyWorkFinished) {
			std::cout << "Applying changes (" << ApplyWorkFinished / 22.0*100.0 << "%)[";
			int workNotFinished = 22 - ApplyWorkFinished;

			std::cout << string(ApplyWorkFinished, '#');
			std::cout << string(workNotFinished, '-');

			std::cout << "]\r";
		}
		/*Progress bar end*/
	}

	/*Progess bar*/
	auto elapsed_apply = std::chrono::high_resolution_clock::now() - start;
	float applyTime = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_apply).count() / 1000.0);

	std::cout << "[" << applyTime << "s] " << "Applying changes (100%)[##########################]" << endl;
	/*Progress bar end*/

}
#pragma endregion
