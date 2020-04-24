
/*Multi color space color*/
class MSColor
{
	public:

		float R, G, B;			/*RGB Color Space*/
		float X, Y, Z;			/*XYZ Color Space*/

		enum BaseColorSpace { RGB, XYZ }baseColorSpace;

		MSColor(float h, float i, float j, BaseColorSpace bcs) { setColor(h, i, j, bcs); }

		void GetXYZFromRGB();
		void GetRGBFromXYZ();
		void setColor(float h, float i, float j, BaseColorSpace bcs);

		float getTemperature();

};