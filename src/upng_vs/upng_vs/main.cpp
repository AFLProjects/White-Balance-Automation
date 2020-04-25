#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <lodepng.h>

#include "MSColor.h";

using namespace std;

#pragma warning(disable:4996)

/*SETTINGS*/
bool fileExists(const std::string& name) {
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

struct Settings
{
public:
	/*
	Index - Property
	0 - Color Balance
	1 - White Intensity
	2 - Black Intensity
	3 - Shadow Intensity
	4 - Vividity Min
	5 - Vividity Max
	*/
	float data[6];

	Settings(string path)
	{
		std::ifstream file(path);
		std::string str;
		int k = 0;
		while (std::getline(file, str)) {
			data[k] = std::stof(str);
			k++;
		}
	}
	Settings() { memset(data, -1, sizeof(data)); }

	void DisplaySettings()
	{
		cout << "\nColor Balance : " << data[0] << endl;
		cout << "White Intensity : " << data[1] << endl;
		cout << "Black Intensity : " << data[2] << endl;
		cout << "Shadow Intensity : " << data[3] << endl;
		cout << "Vividity Min : " << data[4] << endl;
		cout << "Vividity Max : " << data[5] << endl << endl;
	}
};
/*SETTINGS END*/


/*IMAGES*/
std::vector<unsigned char> decodeImage(const char* filename) {
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	return image;
}

void encodeImage(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
	unsigned error = lodepng::encode(filename, image, width, height);
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}
/*IMAGES END*/

/*Colors*/
int RGB_565(int r, int g, int b) {
	return (b & 0x1F) | ((g & 0x3F) << 5) | ((r & 0x1F) << 11);
}


Settings currentSettings = Settings("settings.ini");
int main() {

	/*<==FAST IO==>*/
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	/*<==TITLE==>*/
	const char *title =
		"     ----     `---.      .---.       .---.      .---`       `-----..`            `--------.            .://:-`          "
		"    `MMMM`    -MMMm      -NMMM:     -NMMM:      hMMM/       oMMMMMMMMNh/`        :MMMMMMMMd         /hNMMMMMMMms-       "
		"    `MMMM`    -MMMm       :MMMN.   `mMMM+       hMMM/       oMMMmssymMMMN:       :MMMNssss+       .dMMMMmhyhNMMMMy      "
		"    `MMMM`    -MMMm        oMMMd`  hMMMs        hMMM/       oMMMs    +MMMN.      :MMMm`````      `mMMMh.     :mMMMy     "
		"    `MMMM`    -MMMm         yMMMy oMMMh         hMMM/       oMMMs     mMMM+      :MMMMMMMMo      /MMMN        :MMMM`    "
		"    `MMMM`    :MMMm          dMMMyMMMd`         hMMM/       oMMMs    `NMMM:      :MMMNyyyy/      -MMMM-       oMMMm     "
		"     mMMMs`  .hMMMy          `mMMMMMN.          hMMM/       oMMMs  .+mMMMh       :MMMm            sMMMNs:.`./hMMMM:     "
		"     -NMMMMNNMMMMy`           .NMMMN-           hMMM/       oMMMMMMMMMMm+        :MMMMMMMMd        /mMMMMMMMMMMMh-      "
		"      `/sdmNmmho.              :hhh:            shhh:       /hhhhhhhs+-          -hhhhhhhhs          -ohdmNmdy+.        ";
	cout << title << "\n by Adrian Fluder" << endl;

	/*<==Commands==>*/
	const char *commands =
		"\nCommands :               "
		"\n\t-settings              //Shows the settings"
		"\n\t-settings -s           //Set setting manually"
		"\n\t-settings -a		    //Set the settings automatically from videos in <dir>"
		"\n\t-edit					//Edits the videos/images in <dir>\n\n";
	cout << commands << endl;

	while (true)
	{
		cout << ">>";
		std::string command;
		std::getline(std::cin, command);

		if (command == "settings")
		{
			if (!fileExists("settings.ini"))
			{
				std::ofstream file("settings.ini");
				file << "1\n100\n0\n50\n50\n70" << std::endl;
				file.close();
				currentSettings = Settings("settings.ini");
			}
			currentSettings.DisplaySettings();
		}
		else if (command == "settings -s")
		{
			string cb, wi, bi, si, vmi, vma;
			cout << "\nColor Balance : " << endl;
			std::getline(std::cin, cb);
			cout << "White Intensity : " << endl;
			std::getline(std::cin, wi);
			cout << "Black Intensity : " << endl;
			std::getline(std::cin, bi);
			cout << "Shadow Intensity : " << endl;
			std::getline(std::cin, si);
			cout << "Vividity Min : " << endl;
			std::getline(std::cin, vmi);
			cout << "Vividity Max : " << endl;
			std::getline(std::cin, vma);
			std::string output = cb + "\n" + wi + "\n" + bi + "\n" + si + "\n" + vmi + "\n" + vma;

			std::ofstream file("settings.ini");
			file << output << std::endl;
			file.close();

			cout << "\nNew Settings" << endl;
			currentSettings = Settings("settings.ini");
			currentSettings.DisplaySettings();
		}
		else if (command == "settings -a")
		{
			cout << "Not implemented!\n\n";
		}
		else if (command == "edit")
		{
			std::string path = "";
			cout << "\File Path : ";
			std::getline(std::cin, path);
			/*Decode*/
			std::vector<unsigned char> image = decodeImage(path.c_str());
			std::vector<unsigned char> *imgPtr = &image;

			float w = 200.0;
			float h = 300.0;
			float imageSize = w*h;

			float averageR = 0;
			float averageG = 0;
			float averageB = 0;
			float *ptr;

			/*Concentration of colors*/
			int rCount[255];
			int bCount[255];

			memset(&rCount, 0, sizeof(rCount));
			memset(&bCount, 0, sizeof(bCount));

			for (size_t i = 0; i < imageSize*4; i+=4)
			{
				
				ptr = &averageR;
				*ptr += (*imgPtr)[i] / imageSize;

				ptr = &averageG;
				*ptr += (*imgPtr)[i+1] / imageSize;

				ptr = &averageB;
				*ptr += (*imgPtr)[i + 2] / imageSize;

				/*Add to corresponding color*/
				rCount[(*imgPtr)[i]] += 1;
				bCount[(*imgPtr)[i+2]] += 1;
			}

			/*Reference value for color adaptation*/
			int rMax = 0;
			int bMax = 0;

			float changesR = 0;
			float changesB = 0;

			/*Evolution of contrentration of colors*/
			float deltaR[255];
			float deltaB[255];

			memset(&deltaR, 0, sizeof(rCount));
			memset(&deltaB, 0, sizeof(bCount));

			for (int i = 251; i > 1; i--)
			{
				if (rCount[i + 4] != 0) {
					float currChangeR = rCount[i] / rCount[i + 4];
					deltaR[i] = changesR / (253 - i);
					changesR += currChangeR;
				}

				if (bCount[i + 4] != 0) {
					float currChangeB = bCount[i] / bCount[i + 4];
					deltaB[i] = changesB / (253 - i);
					changesB += currChangeB;
				}
				
			}

			/*Find high changing points to find reference value for adaptation*/
			for (int i = 251; i > 1; i--)
			{
				if (deltaR[i] > deltaR[i-1] && deltaR[i] > deltaR[i - 2] && deltaR[i] > deltaR[i - 3] && deltaR[i] > deltaR[i - 4])
				{
					printf("\R:%i", i);
					rMax = i;
					break;
				}
			}

			/*Find high changing points to find reference value for adaptation*/
			for (int i = 251; i > 1; i--)
			{
				if (deltaB[i] > deltaB[i - 1] && deltaB[i] > deltaB[i - 2] && deltaB[i] > deltaB[i - 3] && deltaB[i] > deltaB[i - 4])
				{
					printf("\B:%i", i);
					bMax = i;
					break;
				}
			}
			/*
			int rMax = 0;
			int bMax = 0;
			int rcMax = 0;
			int bcMax = 0;
			for (int i = round(aR); i < 255; i++)
			{
				float scoreR = pow(i,3) * (rCount[i] / imageSize * 100.0)*((rCount[i] / imageSize * 100.0)/ (rCount[i-5] / imageSize * 100.0));
				float scoreB = pow(i,3) * (bCount[i] / imageSize * 100.0)*((bCount[i] / imageSize * 100.0) / (bCount[i-5] / imageSize * 100.0));

				if (scoreR > rcMax) { rcMax = scoreR;  rMax = i; }
				if (scoreB > bcMax) { bcMax = scoreB;  bMax = i; }
				printf("\nFor B:%i C:%f%\n R*C = %f", i, bCount[i] / imageSize, scoreR);
			}
			*/
			/*
			aR = (aR / (255.0)) * 100.0;
			aG = (aG / (255.0)) * 100.0;
			aB = (aB / (255.0)) * 100.0;*/
		//	MSColor imageAvrgCol = MSColor(aR, aG, aB, MSColor::BaseColorSpace::RGB);


			printf("\nAverage color R:%f G:%f B:%f\n", averageR, averageG, averageR);
			printf("\nMax colors R:%i B:%i\n", rMax,bMax);
			//printf("\nMax color R:%f B:%f\n", mR,mB);

			/*Apply adaptation by taking average between r value and b value*/
			for (size_t i = 0; i < imageSize * 4; i += 4)
			{
				int r = round((*imgPtr)[i] * (((rMax + bMax) / 2.0) / rMax));
				if (r >= 255) { r=254; }

				int b = round((*imgPtr)[i + 2] * (((rMax + bMax) / 2.0) / bMax));
				if (b >= 255) { b = 254; }
				(*imgPtr)[i] = r;
				(*imgPtr)[i + 2] = b;
			}

			encodeImage(("updated_" + path).c_str(), *imgPtr, w, h);
		}
		else if (command == "col")
		{
			MSColor red = MSColor(255.0, 235.0, 12.0, MSColor::BaseColorSpace::RGB);
			printf("\nR:%f G:%f B:%f\n", red.R, red.G, red.B);
			printf("\nX:%f Y:%f Z:%f\n", red.X, red.Y, red.Z);
			printf("\nTemperature : %f\n\n",red.getTemperature());
			
		}
	}

	std::getchar();
	return 0;

}

