/*
Color class for easy color space manipulations between RGB, XYZ and HSL
*/
class Color
{
public:

	/*
	Color data
	0 : R,X,H
	1 : G,Y,S
	2 :	B,Z,L
	*/
	float data[3];

	/*Current color space*/
	enum ColorType { RGB, XYZ, HSV };
	ColorType ColorSpace;

	/*Constructors*/
	Color(float a, float b, float c, ColorType type);
	Color();

	/*Switch to any color space*/
	void SwitchTo(ColorType type);

	/*Direct switch between color spaces
	(You need to be sure in what space your in but it
	can be faster by avoiding the checking in SwitchTo function)
	*/
	void XYZtoRGB();
	void RGBtoXYZ();
	void HSVtoRGB();
	void RGBtoHSV();

	/*Outputs current color temperature(in any color space)*/
	float getTemperature();

private:
	float min(float a, float b);
	float max(float a, float b);
	
};