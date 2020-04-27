#include "stdafx.h"
#include "Color.h"
#include "cmath";

using namespace std;

#pragma region Constructors
Color::Color(float a, float b, float c, ColorType type)
{
	this->data[0] = a;
	this->data[1] = b;
	this->data[2] = c;
	this->type = type;
}

Color::Color() 
{
	this->data[0] = 0;
	this->data[1] = 0;
	this->data[2] = 0;
	this->type = ColorType::RGB;
}
#pragma endregion

#pragma region Constructors

#pragma endregion

float Color::min(float a, float b) { return (b < a) ? b : a; }
float Color::max(float a, float b) { return (b > a) ? b : a; }

#pragma region Conversions

void Color::RGBtoHSV() {

	float fR = this->data[0] / 255.0;
	float fG = this->data[1] / 255.0;
	float fB = this->data[2] / 255.0;
	float fH = 0;
	float fS = 0;
	float fV = 0;

	float fCMax = max(max(fR, fG), fB);
	float fCMin = min(min(fR, fG), fB);
	float fDelta = fCMax - fCMin;

	if (fDelta > 0) {
		if (fCMax == fR) {
			fH = 60.0 * (fmod(((fG - fB) / fDelta), 6.0));
		}
		else if (fCMax == fG) {
			fH = 60.0 * (((fB - fR) / fDelta) + 2.0);
		}
		else if (fCMax == fB) {
			fH = 60.0 * (((fR - fG) / fDelta) + 4.0);
		}

		if (fCMax > 0) {
			fS = fDelta / fCMax;
		}
		else {
			fS = 0;
		}

		fV = fCMax;
	}
	else {
		fH = 0;
		fS = 0;
		fV = fCMax;
	}

	if (fH < 0) {
		fH = 360.0 + fH;
	}

	this->data[0] = fH;
	this->data[1] = fS;
	this->data[2] = fV;
}

void Color::HSVtoRGB() {

	float fR = 0;
	float fG = 0;
	float fB = 0;
	float fH = this->data[0];
	float fS = this->data[1];
	float fV = this->data[2];

	float fC = fV * fS;
	float fHPrime = fmod(fH / 60.0, 6);
	float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
	float fM = fV - fC;

	if (0 <= fHPrime && fHPrime < 1) {
		fR = fC;
		fG = fX;
		fB = 0;
	}
	else if (1 <= fHPrime && fHPrime < 2) {
		fR = fX;
		fG = fC;
		fB = 0;
	}
	else if (2 <= fHPrime && fHPrime < 3) {
		fR = 0;
		fG = fC;
		fB = fX;
	}
	else if (3 <= fHPrime && fHPrime < 4) {
		fR = 0;
		fG = fX;
		fB = fC;
	}
	else if (4 <= fHPrime && fHPrime < 5) {
		fR = fX;
		fG = 0;
		fB = fC;
	}
	else if (5 <= fHPrime && fHPrime < 6) {
		fR = fC;
		fG = 0;
		fB = fX;
	}
	else {
		fR = 0;
		fG = 0;
		fB = 0;
	}

	fR += fM;
	fG += fM;
	fB += fM;

	this->data[0] = fR * 255.0;
	this->data[1] = fG * 255.0;
	this->data[2] = fB * 255.0;
}

/*Convert from RGB to XYZ Color Space*/
void Color::RGBtoXYZ()
{
	double r = this->data[0] / 255.0;
	double g = this->data[1] / 255.0;
	double b = this->data[2] / 255.0;

	r = ((r > 0.04045) ? pow((r + 0.055) / 1.055, 2.4) : (r / 12.92)) * 100.0;
	g = ((g > 0.04045) ? pow((g + 0.055) / 1.055, 2.4) : (g / 12.92)) * 100.0;
	b = ((b > 0.04045) ? pow((b + 0.055) / 1.055, 2.4) : (b / 12.92)) * 100.0;

	this->data[0] = r * 0.4124564 + g * 0.3575761 + b * 0.1804375;
	this->data[1] = r * 0.2126729 + g * 0.7151522 + b * 0.0721750;
	this->data[2] = r * 0.0193339 + g * 0.1191920 + b * 0.9503041;
}


/*Convert from XYZ to RGB Color Space*/
void Color::XYZtoRGB()
{
	double x = this->data[0] / 100.0;
	double y = this->data[1] / 100.0;
	double z = this->data[2] / 100.0;

	double r = x * 3.2404542 + y * -1.5371385 + z * -0.4985314;
	double g = x * -0.9692660 + y * 1.8760108 + z * 0.0415560;
	double b = x * 0.0556434 + y * -0.2040259 + z * 1.0572252;

	r = ((r > 0.0031308) ? (1.055*pow(r, 1 / 2.4) - 0.055) : (12.92*r)) * 255.0;
	g = ((g > 0.0031308) ? (1.055*pow(g, 1 / 2.4) - 0.055) : (12.92*g)) * 255.0;
	b = ((b > 0.0031308) ? (1.055*pow(b, 1 / 2.4) - 0.055) : (12.92*b)) * 255.0;

	this->data[0] = r;
	this->data[1] = g;
	this->data[2] = b;
}

float Color::getTemperature()
{
	/*normalized chromaticity values*/
	float x = this->data[0] / (this->data[0] + this->data[1] + this->data[2]);
	float y = this->data[1] / (this->data[0] + this->data[1] + this->data[2]);

	float xe = 0.3366;
	float ye = 0.1735;
	float a0 = -949.86315;
	float a1 = 6253.80338;
	float t1 = 0.92159;
	float a2 = 28.70599;
	float t2 = 0.20039;
	float a3 = 0.00004;
	float t3 = 0.07125;
	float n = (x - xe) / (y - ye);
	
	float cct = a0 + (a1*_CMATH_::exp(-n / t1)) + (a2*_CMATH_::exp(-n / t2)) + (a3*_CMATH_::exp(-n / t2));

	return cct;
}


void Color::SwitchTo(ColorType type) 
{
	if (this->type == ColorType::RGB && type == ColorType::XYZ)
	{
		RGBtoXYZ();
		type = ColorType::XYZ;
	}
	else if (this->type == ColorType::RGB && type == ColorType::HSV)
	{
		RGBtoHSV();
		type = ColorType::HSV;
	}
	else if (this->type == ColorType::XYZ && type == ColorType::RGB)
	{
		XYZtoRGB();
		type = ColorType::RGB;
	}
	else if (this->type == ColorType::XYZ && type == ColorType::HSV)
	{
		XYZtoRGB();
		RGBtoHSV();
		type = ColorType::HSV;
	}
	else if (this->type == ColorType::HSV && type == ColorType::RGB)
	{
		HSVtoRGB();
		type = ColorType::RGB;
	}
	else if (this->type == ColorType::HSV && type == ColorType::XYZ)
	{
		HSVtoRGB();
		RGBtoXYZ();
		type = ColorType::XYZ;
	}
}
#pragma endregion