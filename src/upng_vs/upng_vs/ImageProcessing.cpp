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
	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

	int ExtractingWorkFinishedOld = 0; /*Progress bar value old*/
	int ExtractingWorkFinished = 0; /*Progress bar value*/

	/*Convert all the colors to Color class*/
	const int p = 4 * speedUpAmout;
	const int max = (imageSize * 4) - 1;

	for (int i = 0; i <  max;i += p)
		out[i >> 2] = Color((float)((inputImagePtr)[i]), (float)((inputImagePtr)[i + 1]), (float)((inputImagePtr)[i + 2]), Color::ColorType::RGB);

	/*Progress bar & time of execution*/
	auto elapsed_extracting = std::chrono::high_resolution_clock::now() - start;
	float extractionTime = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_extracting).count() / 1000.0;

	int ExtractingkBytesProcessed = sizeof(vector<Color*>) + (sizeof(Color) * imageSize / 1000.0);
	float ExtractingkBytesPerSec = ExtractingkBytesProcessed / extractionTime;

	std::cout << "[" << extractionTime << "s][" << ExtractingkBytesPerSec << "kB/s] " << "Extracting colors (100%)" << endl;

}

ImageProcessing::Environement ImageProcessing::GetImageEnvironement(Color& avrgColor,Color& avrgColorNoGray)
{
	/*Environement*/
	ImageProcessing::Environement environement = ImageProcessing::Environement::ERROR;

	/*Check evironement with all colors*/
	float R = avrgColor.data[0];
	float G = avrgColor.data[1];
	float B = avrgColor.data[2];

	if (R+G+B <= 150)
		environement =  ImageProcessing::Environement::Black;
	else if (R+G+B >= 500)
		environement = ImageProcessing::Environement::White;
	else if(abs(R-G) <= 20 && abs(G - B) <= 20 && abs(R - B) <= 20)
		environement = ImageProcessing::Environement::Neutral;
	else if(R>=G&&R>=B)
		environement = ImageProcessing::Environement::Red;
	else if (B >= G && B >= R)
		environement = ImageProcessing::Environement::Blue;
	else if (G >= R && G >= B)
		environement = ImageProcessing::Environement::Green;
	else 
		environement = ImageProcessing::Environement::ERROR;

	/*if environement was strictly white or black return */
	if (environement == ImageProcessing::Environement::Black || environement == ImageProcessing::Environement::White)
		return environement;
	else /*Else analyse the environement without gray parts*/
	{
		int R = (int)round(avrgColorNoGray.data[0]*1.0);
		int G = (int)round(avrgColorNoGray.data[1]*1.0);
		int B = (int)round(avrgColorNoGray.data[2]*1.0);

		if (R + G + B <= 150)
			environement = ImageProcessing::Environement::Black;
		else if (R + G + B >= 500)
			environement = ImageProcessing::Environement::White;
		else if (abs(R - G) <= 20 && abs(G - B) <= 20 && abs(R - B) <= 20)
			environement = ImageProcessing::Environement::Neutral;
		else if (R >= G && R >= B)
			environement = ImageProcessing::Environement::Red;
		else if (B >= G && B >= R)
			environement = ImageProcessing::Environement::Blue;
		else if (G >= R && G >= B)
			environement = ImageProcessing::Environement::Green;
		else
			environement = ImageProcessing::Environement::ERROR;
	}

	return environement;
}


/*Sort pixels by brigthness using look up table where index = R+G+B*/
void ImageProcessing::SortPixels(vector<vector<Color*>>& outLookupTable, int(&outColCount)[766], Color imageCols[], int imageSize,int speedUpAmout)
{
	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

	/*Progress bar values*/
	int SortingWorkFinishedOld = 0;
	int SortingWorkFinished = 0;

	Color col;

	for (int i = 0; i < imageSize; i+=speedUpAmout)
	{
		int index = (int)(imageCols[i].data[0] + imageCols[i].data[1] + imageCols[i].data[2]);
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

/*Find average white color using brightest pixels, using HSV->RGB , RGB->HSV conversions*/
void ImageProcessing::FindWhiteColor(Color& outWhiteColor, vector<vector<Color*>>& sortedColorsLookupTable, int imageSize,int speedUpAmout,int& outWhiteLimit)
{
	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

	/*Count of "white" pixels i want to produce white reference pixel -> 0.8%*/
	const int whiteCount = round(0.8 / 100.0*imageSize)*speedUpAmout;
	const int whitep5Count = round((0.5 / 100.0)*imageSize);
	int currentCount = 0;
	bool whiteLimitContinue = true;

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
						/*
						if (currentCount >= whitep5Count && whiteLimitContinue)
						{
							outWhiteLimit = i;
							whiteLimitContinue = false;
						}*/

						sortedColorsLookupTable[i][j]->RGBtoHSV();
						outWhiteColor.data[0] += sortedColorsLookupTable[i][j]->data[0];
						outWhiteColor.data[1] += sortedColorsLookupTable[i][j]->data[1];
						outWhiteColor.data[2] += sortedColorsLookupTable[i][j]->data[2];
						sortedColorsLookupTable[i][j]->HSVtoRGB();

						currentCount++;

						/*Progress bar*/
						/*
						WhiteRefWorkFinishedOld = WhiteRefWorkFinished;
						WhiteRefWorkFinished = (int)round(((currentCount*1.0) / (whiteCount * 1.0))*22.0);

						if (WhiteRefWorkFinishedOld != WhiteRefWorkFinished) {
							std::cout << "Calculating Avrg. White color(HSV) (" << WhiteRefWorkFinished / 22.0*100.0 << "%)[";
							int workNotFinished = 22 - WhiteRefWorkFinished;

							std::cout << string(WhiteRefWorkFinished, '#');
							std::cout << string(workNotFinished, '-');

							std::cout << "]\r";
						}*/
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

	outWhiteColor.data[0] /= whiteCount;
	outWhiteColor.data[1] /= whiteCount;
	outWhiteColor.data[2] /= whiteCount;

	/*Progress bar*/
	
	auto elapsed_whiteref = std::chrono::high_resolution_clock::now() - start;
	float whiterefTime = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_whiteref).count() / 1000.0);

	std::cout << "[" << whiterefTime << "s] " << "Calculating Avrg. White color(HSV) (100%)[##########################]" << endl;

	outWhiteColor.HSVtoRGB();
	outWhiteColor = Color(1 / (outWhiteColor.data[0] / 255.0), 1 / (outWhiteColor.data[1] / 255.0), 1 / (outWhiteColor.data[2] / 255.0), Color::ColorType::RGB);

	/*Progress bar end*/

}



/*Apply changes to image*/
void ImageProcessing::ApplyChanges(vector<unsigned char>& outImgPtr, Color imageCols[], Color& whiteRef, int imageSize,ImageProcessing::Environement& outEnvironement)
{
	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

	/*Progess bar values*/
	int ApplyWorkFinishedOld = 0;
	int ApplyWorkFinished = 0;

	/*Two average colors,one with and the other without gray parts of the image, to get a really feeling of thye image's dominating color*/
	Color avrgColor = Color(0, 0, 0, Color::ColorType::RGB);
	Color avrgColorNoGray = Color(0, 0, 0, Color::ColorType::RGB);

	/*Amount non gray parts to make the average*/
	int countNoGray = 0;

	if (whiteRef.data[0] > 3)
		whiteRef.data[0] = 3;
	if (whiteRef.data[1] > 3)
		whiteRef.data[1] = 3;
	if (whiteRef.data[2] > 3)
		whiteRef.data[2] = 3;

	for (int i = 0; i < imageSize; i++)
	{
		int R = (int)round((outImgPtr)[i * 4] * whiteRef.data[0]);
		int G = (int)round((outImgPtr)[(i * 4) + 1] * whiteRef.data[1]);
		int B = (int)round((outImgPtr)[(i * 4) + 2] * whiteRef.data[2]);

		if (R > 255)
			R = 255;
		if (G > 255)
			G = 255;
		if (B > 255)
			B = 255;

		imageCols[i].data[0] = R;
		imageCols[i].data[1] = G;
		imageCols[i].data[2] = B;

		(outImgPtr)[i * 4] = (char)R;
		(outImgPtr)[(i * 4) + 1] = (char)G;
		(outImgPtr)[(i * 4) + 2] = (char)B;

		avrgColor.data[0] += R;
		avrgColor.data[1] += G;
		avrgColor.data[2] += B;

		/*if non gray*/
		if (!((abs(R - G) <= 20 && abs(G - B) <= 20 && abs(R - B) <= 20))) 
		{
			avrgColorNoGray.data[0] += R;
			avrgColorNoGray.data[1] += G;
			avrgColorNoGray.data[2] += B;

			countNoGray++;
		}


		/*Progess bar*/
		/*
		ApplyWorkFinishedOld = ApplyWorkFinished;
		ApplyWorkFinished = (int)round(((i*1.0) / (imageSize * 1.0))*22.0);

		if (ApplyWorkFinishedOld != ApplyWorkFinished) {
			std::cout << "Applying changes (" << ApplyWorkFinished / 22.0*100.0 << "%)[";
			int workNotFinished = 22 - ApplyWorkFinished;

			std::cout << string(ApplyWorkFinished, '#');
			std::cout << string(workNotFinished, '-');

			std::cout << "]\r";
		}*/
		/*Progress bar end*/
	}

	/*average*/
	avrgColor.data[0] /= imageSize*1.0;
	avrgColor.data[1] /= imageSize * 1.0;
	avrgColor.data[2] /= imageSize * 1.0;

	avrgColorNoGray.data[0] /= countNoGray;
	avrgColorNoGray.data[1] /= countNoGray;
	avrgColorNoGray.data[2] /= countNoGray;

	/*return environement*/
	outEnvironement = GetImageEnvironement(avrgColor,avrgColorNoGray);

	/*Progess bar*/
	auto elapsed_apply = std::chrono::high_resolution_clock::now() - start;
	float applyTime = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_apply).count() / 1000.0);

	std::cout << "[" << applyTime << "s] " << "Applying changes (100%)[##########################]" << endl;
	/*Progress bar end*/
	
}

void ImageProcessing::FindSaturation(Color image[], float& outSaturation,int imageSize,int speedupAmout)
{

	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

	Color col;

	int WorkFinishedOld = 0;
	int WorkFinished = 0;

	int count = 0;

	for (int i = 0; i < imageSize; i += speedupAmout)
	{

		float R = image[i].data[0];
		float G = image[i].data[1];
		float B = image[i].data[2];

		/*if vivid color*/
		if (!((abs(R - G) <= 40 && abs(G - B) <= 40 && abs(R - B) <= 40)))
		{
			col.RGBtoHSV();
			outSaturation += col.data[1]; /*get saturation*/
			col.HSVtoRGB();
			count++;
		}
		/*
		WorkFinishedOld = WorkFinished;
		WorkFinished = (int)round(((i*1.0) / (imageSize*1.0))*22.0);

		if (WorkFinishedOld != WorkFinished) {
			std::cout << "Finding saturation (" << WorkFinished / 22.0*100.0 << "%)[";
			int workNotFinished = 22 - WorkFinished;

			std::cout << string(WorkFinished, '#');
			std::cout << string(workNotFinished, '-');

			std::cout << "]\r";
		}*/
	}

	/*average saturation*/
	outSaturation /= count;

	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	float Time = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() / 1000.0);

	std::cout << "[" << Time << "s] " << "Fiding saturation (100%)[##########################]" << endl;
}

void ImageProcessing::ApplySaturation(vector<unsigned char>& outImgPtr, int imageSize, float saturation,ImageProcessing::Environement environement)
{

	auto start = std::chrono::high_resolution_clock::now(); /*timer*/

	int WorkFinishedOld = 0;
	int WorkFinished = 0;

	for (int i = 0; i < imageSize; i++)
	{
		int R = (int)round((outImgPtr)[i * 4]);
		int G = (int)round((outImgPtr)[(i * 4) + 1]);
		int B = (int)round((outImgPtr)[(i * 4) + 2]);

		if (R > 255)
			R = 255;
		if (G > 255)
			G = 255;
		if (B > 255)
			B = 255;

		Color col = Color(R*1.0, G*1.0, B*1.0, Color::ColorType::RGB);

		float targetSaturation = saturation;

		if (environement == ImageProcessing::Environement::Green)
			targetSaturation = 0.6; /*These constant values will be changed and polished later*/
		else if (environement == ImageProcessing::Environement::Blue)
			targetSaturation = 0.56; /*These constant values will be changed and polished later*/
		else if (environement == ImageProcessing::Environement::Red)
			targetSaturation = 0.48; /*These constant values will be changed and polished later*/

		col.RGBtoHSV();
		/*apply saturation change*/
		float satCoefficient = targetSaturation / saturation;
		col.data[1] *= satCoefficient; /*Saturation*/

		if (col.data[1] >= 1)
			col.data[1] = 1;

		col.HSVtoRGB();

		R = (int)round(col.data[0]);
		G = (int)round(col.data[1]);
		B = (int)round(col.data[2]);

		if (R > 255)
			R = 255;
		if (G > 255)
			G = 255;
		if (B > 255)
			B = 255;

		(outImgPtr)[i * 4] = (char)R;
		(outImgPtr)[(i * 4) + 1] = (char)G;
		(outImgPtr)[(i * 4) + 2] = (char)B;

		/*
		WorkFinishedOld = WorkFinished;
		WorkFinished = (int)round(((i*1.0) / (imageSize*1.0))*22.0);

		if (WorkFinishedOld != WorkFinished) {
			std::cout << "Applying saturation (" << WorkFinished / 22.0*100.0 << "%)[";
			int workNotFinished = 22 - WorkFinished;

			std::cout << string(WorkFinished, '#');
			std::cout << string(workNotFinished, '-');

			std::cout << "]\r";
		}*/
	}

	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	float Time = (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() / 1000.0);

	std::cout << "[" << Time << "s] " << "Applying saturation (100%)[##########################]" << endl;
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

	int speedUp = 1; /*Loss of recision,by skipping pixels*/

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
	Color referenceWhite = Color(0, 0, 0, Color::ColorType::HSV);
	ImageProcessing::FindWhiteColor(referenceWhite, sortedColorsLookupTable, imageSize, speedUp, whiteLimitVal);

	cout << "White limit : " << whiteLimitVal << endl;

	ImageProcessing::Environement imageEnvironement = ImageProcessing::Environement::Neutral;

	/*Apply changes using the found white reference color*/
	ImageProcessing::ApplyChanges(image, imageCols, referenceWhite, imageSize, imageEnvironement);

	/*
	float saturation = 0;
	ImageProcessing::FindSaturation(imageCols, saturation, imageSize, speedUp);
	cout << "Saturation : " << saturation << endl;


	ImageProcessing::ApplySaturation(imgPtr, imageSize, saturation, imageEnvironement);*/

	auto elapsed = std::chrono::high_resolution_clock::now() - start;
	long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

	printf("\nTotal time of execution(without decoding/encoding) : ");
	cout << milliseconds << "ms" << endl;


	cout << "Saving image...\n";

	/*Save image*/
	ImageProcessing::encodeImage((path).c_str(), image, w, h);

	cout << "Image Saved!\n" << endl;

}

#pragma endregion
