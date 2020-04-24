#include "stdafx.h"
#include "MSColor.h"
#include "cmath";

/*Convert from RGB to XYZ Color Space*/
void MSColor::GetXYZFromRGB() 
{
	float var_R = (this->R / 255.0f);
	float var_G = (this->G / 255.0f);
	float var_B = (this->B / 255.0f);

	if (var_R > 0.04045) {
		var_R = _CMATH_::pow(((var_R + 0.055) / 1.055),2.4);
	}
	else {
		var_R = var_R / 12.92;
	}

	if (var_G > 0.04045) {
		var_G = _CMATH_::pow(((var_G + 0.055) / 1.055), 2.4);
	}
	else {
		var_G = var_G / 12.92;
	}

	if (var_B > 0.04045) {
		var_B = _CMATH_::pow(((var_B + 0.055) / 1.055), 2.4);
	}
	else {
		var_B = var_B / 12.92;
	}

	var_R *= 100.0;
	var_G *= 100.0;
	var_B *= 100.0;

	this->X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
	this->Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
	this->Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;
}


/*Convert from XYZ to RGB Color Space*/
void MSColor::GetRGBFromXYZ() 
{
	float var_X = this->X / 100.0;
	float var_Y = this->Y / 100.0;
	float var_Z = this->Z / 100.0;

	float var_R = var_X * 3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
	float var_G = var_X * -0.9689 + var_Y * 1.8758 + var_Z * 0.0415;
	float var_B = var_X * 0.0557 + var_Y * -0.2040 + var_Z * 1.0570;

	if (var_R > 0.0031308) 
	{ 
		var_R = 1.055 * (_CMATH_::pow(var_R, (1 / 2.4))) - 0.055;
	}
	else 
	{
		var_R = 12.92 * var_R;
	}

	if (var_G > 0.0031308)
	{
		var_G = 1.055 * (_CMATH_::pow(var_G, (1 / 2.4))) - 0.055;
	}
	else
	{
		var_G = 12.92 * var_G;
	}

	if (var_B > 0.0031308)
	{
		var_B = 1.055 * (_CMATH_::pow(var_B, (1 / 2.4))) - 0.055;
	}
	else
	{
		var_B = 12.92 * var_B;
	}

	this->R = var_R * 255;
	this->G = var_G * 255;
	this->B = var_B * 255;
}
