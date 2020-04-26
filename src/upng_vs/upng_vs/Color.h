class Color
{
public:

	enum ColorType { RGB, XYZ, HSV };

	float data[3];
	ColorType type;

	Color(float a, float b, float c, ColorType type);
	Color();

	void SwitchTo(ColorType type);

	float getTemperature();
private:
	void XYZtoRGB();
	void RGBtoXYZ();
	void HSVtoRGB();
	void RGBtoHSV();
	float min(float a, float b);
	float max(float a, float b);
};