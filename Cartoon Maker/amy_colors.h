#ifndef AMY_COLORS_H
#define AMY_COLORS_H

#include <math.h>

namespace sven {
struct rgb {
	float r,g,b;
	rgb() {}
	rgb(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
	bool operator<(const rgb &c) const {
		if(r<c.r) return true;
		if(r>c.r) return false;
		if(g<c.g) return true;
		if(g>c.g) return false;		
		return b<c.b;
	}
};

typedef struct luv {
	float l, u, v;
	luv() {}
	luv(float l, float u, float v) : l(l), u(u), v(v) {}

	void AddLuv(float L, float U , float V)
	{
		l = L;
		u = U;
		v = V;
	}
} luv;

const float one_third=(float)0.33333333333333333333333333333333333333333333333333333333333333;
}


inline float clamp(float x, float x_min, float x_max){
	 
	if (x < x_min) return(x_min);
	else if (x > x_max) return(x_max);
	else return(x);
}

struct amy_xyz {

	float X, Y, Z;

	amy_xyz(void) : X(0), Y(0), Z(0) {}

	amy_xyz(float X, float Y, float Z) : X(X), Y(Y), Z(Z) {}

	amy_xyz(const sven::rgb &c) {
		float B = c.b/255.;
		float G = c.g/255.;
		float R = c.r/255.;

		X = 0.412453f*R + 0.357580f*G + 0.180423f*B;
		Y = 0.212671f*R + 0.715160f*G + 0.072169f*B;
		Z = 0.019334f*R +  0.119193f*G + 0.950227f*B;

		const float Xn = 0.9513f;
		const float Yn = 1.000f;
		const float Zn = 1.0886f;

		X = X/Xn; Y = Y/Yn; Z = Z/Zn;
	}

	void AddXYZ(const sven::rgb &c)
	{
		float B = c.b/255.;
		float G = c.g/255.;
		float R = c.r/255.;

		X = 0.412453f*R + 0.357580f*G + 0.180423f*B;
		Y = 0.212671f*R + 0.715160f*G + 0.072169f*B;
		Z = 0.019334f*R +  0.119193f*G + 0.950227f*B;

		const float Xn = 0.9513f;
		const float Yn = 1.000f;
		const float Zn = 1.0886f;

		X = X/Xn; Y = Y/Yn; Z = Z/Zn;
	}
	void AddXYZ(FLOAT R, FLOAT G, FLOAT B)
	{
		X = 0.412453f*R + 0.357580f*G + 0.180423f*B;
		Y = 0.212671f*R + 0.715160f*G + 0.072169f*B;
		Z = 0.019334f*R +  0.119193f*G + 0.950227f*B;

		const float Xn = 0.9513f;
		const float Yn = 1.000f;
		const float Zn = 1.0886f;

		X = X/Xn; Y = Y/Yn; Z = Z/Zn;
	}

	sven::rgb to_rgb() {

		//for RGB [0,1] etc.
		float R = 3.240479f*X + -1.537150f*Y +  -0.498535f*Z;
		float G = -0.969256f*X +  1.875992f*Y +  0.041556f*Z;
		float B = 0.055648f*X +   -0.204043f*Y + 1.057311f*Z;
		R = clamp(R,0.0,1.0);
		G = clamp(G,0.0,1.0);
		B = clamp(B,0.0,1.0);
		
		return sven::rgb(static_cast<unsigned char>(R*255),
                                 static_cast<unsigned char>(G*255),
                                 static_cast<unsigned char>(B*255));
	}
};

struct amy_lab {
	float l,a,b;

	amy_lab() {}
	amy_lab(float l, float a, float b) : l(l), a(a), b(b) {}

	amy_lab(const sven::rgb &c) {
		amy_xyz temp(c);
		*this=amy_lab(temp);
	}
	amy_lab(amy_xyz &c) {
		using sven::one_third;

		float X_third = pow(c.X,(float)one_third);
		float Y_third = pow(c.Y,(float)one_third);
		float Z_third = pow(c.Z,(float)one_third);

		if (c.Y > 0.008856)
			l = (116.0f*(Y_third)) - 16.0f;
		else l = 903.3f*c.Y;

		a= 500.0f * ((X_third) - (Y_third));
		b= 200.0f * ((Y_third) - (Z_third));
	}

	sven::rgb to_rgb() {

		float P = (l+16.0)/116.0;

		// Define as constants
		float Xn = 0.9513f;
		float Yn = 1.000f;
		float Zn = 1.0886f;

		float X = Xn * pow(P+ (a/500),3);
		float Y = Yn * pow(P,3);
		float Z = Zn * pow(P - (b/200),3);

		return amy_xyz(X,Y,Z).to_rgb();
	}
	void fromXYZ(FLOAT X, FLOAT Y, FLOAT Z)	{
		using sven::one_third;

		float X_third = pow(X,(float)one_third);
		float Y_third = pow(Y,(float)one_third);
		float Z_third = pow(Z,(float)one_third);

		if (Y > 0.008856)
			l = (116.0f*(Y_third)) - 16.0f;
		else l = 903.3f*Y;

		a= 500.0f * ((X_third) - (Y_third));
		b= 200.0f * ((Y_third) - (Z_third));

	}

};

namespace UserDefinedColorspace
{
	class CImageColorspace
	{
	public:
		sven::rgb *pRgb;
		amy_xyz *pXyz;
		amy_lab *pLab;
		CSize size;
		
		CImageColorspace()
		{
			pRgb = NULL;
			pXyz = NULL;
			pLab = NULL;
		}

		HRESULT AddRGB(sven::rgb *sourceRgb, CSize size)
		{
			pRgb = new sven::rgb[size.cx * size.cy];
			this ->size = size;
			INT i = 0 , j = 0, cx = size.cx, cy = size.cy;
			DOUBLE tmpmax = 0;
			for(i = 0 ; i < cx ; i++)
			{
				for(j = 0 ; j < cy ; j++)
				{
					tmpmax += sourceRgb[i *cx +j].r;
				}
			}
			tmpmax /= cx * cy;

			for(i = 0 ; i < cx ; i++)
			{
				for(j = 0 ; j < cy;j++)
				{
					pRgb[i * cx + j].r = sourceRgb[i* cx + j].r;
					pRgb[i * cx + j].g = sourceRgb[i* cx + j].g;
					pRgb[i * cx + j].b = sourceRgb[i* cx + j].b;
				}
			}

			if(tmpmax > 1.5)
			{
				for(i = 0 ; i < cx  ; i++)
				{
					for(j = 0 ; j < cy ; j++)
					{
						pRgb[i * cx + j].r /= 255;
						pRgb[i * cx + j].g /= 255;
						pRgb[i * cx + j].b /= 255;
					}
				}
			}
			return	1;
		}

		HRESULT ConvertFromXYZToRGB()
		{
			INT i , j ;
			for(i = 0 ; i < size.cx ; i++)
			{
				for(j = 0 ; j < size.cy ; j++)
				{
					pRgb[i * size.cx + j].r = pXyz[i * size.cx + j].X * 3.240479 +
						pXyz[i * size.cx + j].Y *  -1.537150 +
						pXyz[i * size.cx + j].Z * -0.498535;

					pRgb[i * size.cx + j].g = pXyz[i * size.cx + j].X * -0.969256 +
						pXyz[i * size.cx + j].Y * 1.875992 +
						pXyz[i * size.cx + j].Z * 0.041556;

					pRgb[i * size.cx + j].b = pXyz[i * size.cx + j].X * 0.055648 +
						pXyz[i * size.cx + j].Y * -0.204043 +
						pXyz[i * size.cx + j].Z * 1.057311;
				}
			}
			return	1;
		}

		HRESULT ConvertFromXYZToRGB2()
		{
			INT i , j;
			for(i = 0 ; i < size.cx ; i++)
			{
				for(j = 0 ; j < size.cy ; j++)
				{
					DOUBLE * Clinear  = new DOUBLE[3];
					Clinear[0] = pXyz[i *size.cx + j].X *3.2410 -
						pXyz[i *size.cx + j].Y*1.5374 - pXyz[i *size.cx + j].Z*0.4986; // red

					Clinear[1] = (pXyz[i *size.cx + j].X * -1)*0.9692 + 
						pXyz[i *size.cx + j].Y*1.8760 - pXyz[i *size.cx + j].X*0.0416; // green

					Clinear[2] = pXyz[i *size.cx + j].X*0.0556 - 
						pXyz[i *size.cx + j].Y*0.2040 + pXyz[i *size.cx + j].Z*1.0570; // blue

					INT tmp ;
					for(tmp = 0; i<3; i++)
					{
						Clinear[i] = (Clinear[i]<=0.0031308)? 12.92*Clinear[i] : (
							1+0.055)* pow(Clinear[i], (1.0/2.4)) - 0.055;
					}

					pRgb[i*size.cx +j].r = (Clinear[0] * 255.0);
					pRgb[i*size.cx +j].g = (Clinear[1] * 255.0);
					pRgb[i*size.cx +j].b = (Clinear[2] * 255.0);
				}
			}
			return	1;
/*			return new RGB(
				Convert.ToInt32( Double.Parse(String.Format("{0:0.00}",
					Clinear[0]*255.0)) ),
				Convert.ToInt32( Double.Parse(String.Format("{0:0.00}",
					Clinear[1]*255.0)) ),
				Convert.ToInt32( Double.Parse(String.Format("{0:0.00}",
					Clinear[2]*255.0)) )
				);*/
		}

		HRESULT ConvertToXYZ()
		{
			if(pRgb == NULL)
				return -1;

			pXyz = new amy_xyz[size.cx * size.cy];
			INT i , j ;
			for(i = 0 ; i < size.cx ; i++)
			{
				for(j = 0 ; j < size.cy ; j++)
				{
					pXyz[i * size.cx + j].X = pRgb[i * size.cx + j].r * 0.412453 +
						pRgb[i * size.cx + j].g *  0.357580 +
						pRgb[i * size.cx + j].b * 0.180423;

					pXyz[i * size.cx + j].Y = pRgb[i * size.cx + j].r * 0.212671 +
						pRgb[i * size.cx + j].g * 0.715160 +
						pRgb[i * size.cx + j].b * 0.072169;

					pXyz[i * size.cx + j].Z = pRgb[i * size.cx + j].r * 0.019334 +
						pRgb[i * size.cx + j].g * 0.119193 +
						pRgb[i * size.cx + j].b * 0.950227;
				}
			}
			return 1;
		}

		HRESULT ConvertToXYZ2()
		{
			if(pRgb == NULL)
				return	-1;	

			INT i, j;
			DOUBLE *rLinear = new DOUBLE[size.cx * size.cy];
			DOUBLE *gLinear = new DOUBLE[size.cx * size.cy];
			DOUBLE *bLinear = new DOUBLE[size.cx * size.cy];
			for(i = 0; i < size.cx ; i++)
			{
				for(j = 0 ; j < size.cy ; j++)
				{
					rLinear[i*size.cx + j] = pRgb[i*size.cx + j].r / 255.0;
					gLinear[i*size.cx + j] = pRgb[i*size.cx + j].g / 255.0;
					bLinear[i*size.cx + j] = pRgb[i*size.cx + j].b / 255.0;
				}
			}

	    // convert to a sRGB form
//			DOUBLE r,g,b;
			for(i = 0 ; i < size.cx ; i++)
			{
				for(j = 0 ; j < size.cy ; j++)
				{
					pRgb[i*size.cx +j].r = (rLinear[size.cx * i+ j] > 0.04045)?
						pow((rLinear[size.cx * i+ j] + 0.055)/(1 + 0.055), 2.2)
							: (rLinear[size.cx * i+ j]/12.92) ;
					pRgb[i*size.cx +j].g = (gLinear[size.cx * i+j] > 0.04045)?
						pow((gLinear[size.cx * i + j] + 0.055)/(1 + 0.055), 2.2)
						: (gLinear[size.cx * i +j]/12.92) ;
					pRgb[i*size.cx +j].b = (bLinear[i*size.cx +j] > 0.04045)?
						pow((bLinear[i*size.cx +j] + 0.055)/(1 + 0.055), 2.2) 
						: (bLinear[i*size.cx +j]/12.92) ;
				}
			}

			// converts
			pXyz = new amy_xyz[size.cx * size.cy];
			for(i = 0 ; i < size.cx ; i++)
			{
				for(j = 0 ; j < size.cy ; j++)
				{
					pXyz[i*size.cx + j].X = (pRgb[i * size.cx +j].r*0.4124 + 
						pRgb[i * size.cx +j].g*0.3576 + pRgb[i * size.cx +j].r*0.1805);
					pXyz[i*size.cx + j].Y = (pRgb[i * size.cx +j].r*0.2126 + 
						pRgb[i * size.cx +j].g*0.7152 + pRgb[i * size.cx +j].r*0.0722);
					pXyz[i*size.cx + j].Z = (pRgb[i * size.cx +j].r*0.0193 + 
						pRgb[i * size.cx +j].g*0.1192 + pRgb[i * size.cx +j].r*0.9505);
				}
			}
		    return 1;
		}


		HRESULT ConvertToLab()
		{
		}

		sven::rgb *RGBof255()
		{
			sven::rgb *prgb = new sven::rgb[size.cx * size.cy];
			INT i , j ;
			for(i = 0 ; i <size.cx ; i++)
			{
				for( j = 0 ; j < size.cy ; j++)
				{
					prgb[i* size.cx + j].r = pRgb[i * size.cx + j].r * 255;
					prgb[i* size.cx + j].g = pRgb[i * size.cx + j].g * 255;
					prgb[i* size.cx + j].b = pRgb[i * size.cx + j].b * 255;
				}
			}

			return prgb;
		}

		HRESULT Amplifyby255()
		{
			INT i , j;
			for(i = 0 ; i < size.cx ; i++)
			{
				for(j = 0 ; j < size.cy ; j++)
				{
					pRgb[i * size.cx + j].r *= 255;
					pRgb[i * size.cx + j].g *= 255;
					pRgb[i * size.cx + j].b *= 255;
				}
			}
			return	1;
		}
		HRESULT Amplify()
		{
			INT i , j;
			DOUBLE max = 0.0;
			for(i = 0 ; i < size.cx ; i++)
			{
				for(j = 0 ; j < size.cy ; j++)
				{
					max += (pRgb[i * size.cx + j].r + pRgb[i * size.cx + j].g + pRgb[i * size.cx + j].b)/3;
				}
			}
			max /= (size.cx * size.cy);
			DOUBLE amplifying_coefficient = 255 / max;
			for(i = 0 ; i < size.cx ; i++)
			{
				for(j = 0 ; j < size.cy ; j++)
				{
					pRgb[i * size.cx + j].r *= amplifying_coefficient * 255;
					pRgb[i * size.cx + j].g *= amplifying_coefficient * 255;
					pRgb[i * size.cx + j].b *= amplifying_coefficient * 255;
				}
			}

			return	1;
		}
		
	};
};

#endif

/**
RGB to XYZ & XYZ to RGB

RGB values in a particular set of primaries can be transformed to and from CIE XYZ via a 3x3 matrix transform. These transforms involve tristimulus values, that is a set of three linear-light components that conform to the CIE color-matching functions. CIE XYZ is a special set of tristimulus values. In XYZ, any color is represented as a set of positive values.

To transform from XYZ to RGB (with D65 white point), the matrix transform used is [3]:

   [ R ]   [  3.240479 -1.537150 -0.498535 ]   [ X ]
   [ G ] = [ -0.969256  1.875992  0.041556 ] * [ Y ]
   [ B ]   [  0.055648 -0.204043  1.057311 ]   [ Z ].

The range for valid R, G, B values is [0,1]. Note, this matrix has negative coefficients. Some XYZ color may be transformed to RGB values that are negative or greater than one. This means that not all visible colors can be produced using the RGB system.

The inverse transformation matrix is as follows:

   [ X ]   [  0.412453  0.357580  0.180423 ]   [ R ] **
   [ Y ] = [  0.212671  0.715160  0.072169 ] * [ G ]
   [ Z ]   [  0.019334  0.119193  0.950227 ]   [ B ].
**/




