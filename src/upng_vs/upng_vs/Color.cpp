#include "stdafx.h"
#include "Color.h"
#include "cmath";

#pragma region Constructors
Color::Color(float r, float g, float b, bool XYZ = false)
{
	if (!XYZ) 
	{
		SetRGB(r, g, b);
		GetXYZFromRGB();
	}
	else 
	{
		SetXYZ(r, g, b);
		GetRGBFromXYZ();
	}
}

Color::Color() 
{
	SetRGB(0, 0, 0);
	GetXYZFromRGB();
}
#pragma endregion

#pragma region RGB
void Color::SetRGB(unsigned char r, unsigned char g, unsigned char b)
{
	this->R = r;
	this->G = g;
	this->B = b;
}

unsigned char& Color::GetRGB(char index)
{
	if (index == 0)
		return this->R;
	if (index == 1)
		return this->G;
	if (index == 2)
		return this->B;
}
#pragma endregion

#pragma region RGB
void Color::SetXYZ(float x, float y, float z)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
}

double&  Color::GetXYZ(char index)
{
	if (index == 0)
		return this->X;
	if (index == 1)
		return this->Y;
	if (index == 2)
		return this->Z;
}
#pragma endregion

#pragma region Conversions
/*Convert from RGB to XYZ Color Space*/
void Color::GetXYZFromRGB()
{
	double r = this->R / 255.0;
	double g = this->G / 255.0;
	double b = this->B / 255.0;

	r = ((r > 0.04045) ? pow((r + 0.055) / 1.055, 2.4) : (r / 12.92)) * 100.0;
	g = ((g > 0.04045) ? pow((g + 0.055) / 1.055, 2.4) : (g / 12.92)) * 100.0;
	b = ((b > 0.04045) ? pow((b + 0.055) / 1.055, 2.4) : (b / 12.92)) * 100.0;

	this->X = r * 0.4124564 + g * 0.3575761 + b * 0.1804375;
	this->Y = r * 0.2126729 + g * 0.7151522 + b * 0.0721750;
	this->Z = r * 0.0193339 + g * 0.1191920 + b * 0.9503041;
}


/*Convert from XYZ to RGB Color Space*/
void Color::GetRGBFromXYZ()
{
	double x = this->X / 100.0;
	double y = this->Y / 100.0;
	double z = this->Z / 100.0;

	double r = x * 3.2404542 + y * -1.5371385 + z * -0.4985314;
	double g = x * -0.9692660 + y * 1.8760108 + z * 0.0415560;
	double b = x * 0.0556434 + y * -0.2040259 + z * 1.0572252;

	r = ((r > 0.0031308) ? (1.055*pow(r, 1 / 2.4) - 0.055) : (12.92*r)) * 255.0;
	g = ((g > 0.0031308) ? (1.055*pow(g, 1 / 2.4) - 0.055) : (12.92*g)) * 255.0;
	b = ((b > 0.0031308) ? (1.055*pow(b, 1 / 2.4) - 0.055) : (12.92*b)) * 255.0;

	this->R = r;
	this->G = g;
	this->B = b;

}

float Color::getTemperature()
{
	/*normalized chromaticity values*/
	float x = X / (X + Y + Z);
	float y = Y / (X + Y + Z);

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

int RGB_565(int r, int g, int b) {
	return (b & 0x1F) | ((g & 0x3F) << 5) | ((r & 0x1F) << 11);
}
#pragma endregion