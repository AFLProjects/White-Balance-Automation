class Color
{
private:

	unsigned char R, G, B;	
	double X, Y, Z;

	void GetXYZFromRGB();
	void GetRGBFromXYZ();

public:

	Color(float R, float G, float B, bool XYZ=false);
	Color();
		
	void SetRGB(unsigned char r, unsigned char g, unsigned char b);
	unsigned char& GetRGB(char index);

	void SetXYZ(float x, float y, float z);
	double& GetXYZ(char index);

	float getTemperature();
};