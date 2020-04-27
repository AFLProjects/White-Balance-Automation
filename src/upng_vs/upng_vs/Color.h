class Color
{
public:

	float data[3];

	enum ColorType { RGB, XYZ, HSV };
	ColorType type;

	Color(float a, float b, float c, ColorType type);
	Color();

	void SwitchTo(ColorType type);

	void XYZtoRGB();
	void RGBtoXYZ();
	void HSVtoRGB();
	void RGBtoHSV();

	float getTemperature();

private:
	float min(float a, float b);
	float max(float a, float b);
	
};