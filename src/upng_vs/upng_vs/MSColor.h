
/*Multi color space color*/
class MSColor
{
	public:
		float R, G, B;			/*RGB Color Space*/
		float X, Y, Z;			/*XYZ Color Space*/
		enum BaseColorSpace { RGB, XYZ}baseColorSpace;

		MSColor(float h, float i, float j, BaseColorSpace bcs)
		{
			if (bcs == RGB) 
			{
				R = h;
				G = i;
				B = j;

				GetXYZFromRGB();
			}

			if (bcs == XYZ)
			{
				X = h;
				Y = i;
				Z = j;

				GetRGBFromXYZ();
			}

		}
		void GetXYZFromRGB();
		void GetRGBFromXYZ();
};