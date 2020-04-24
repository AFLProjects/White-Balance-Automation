#include "stdafx.h"
#include "MSColor.h"
#include "cmath";

/*Convert from RGB to XYZ Color Space*/
void MSColor::GetXYZFromRGB() 
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
void MSColor::GetRGBFromXYZ() 
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

/*Set color and update all color spaces*/
void MSColor::setColor(float h, float i, float j, BaseColorSpace bcs)
{
	if (bcs == RGB)
	{
		this->R = h;
		this->G = i;
		this->B = j;

		GetXYZFromRGB();
	}

	if (bcs == XYZ)
	{
		this->X = h;
		this->Y = i;
		this->Z = j;

		GetRGBFromXYZ();
	}
}

float MSColor::getTemperature()
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
