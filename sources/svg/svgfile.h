#ifndef SVGFILE_H_INCLUDED
#define SVGFILE_H_INCLUDED
#define HAUTEUR 600
#define LARGEUR 1380
#include <string>
#include <fstream>
#include <set>

constexpr char defcol[] = "black";

///code en provenance du site : https://www.programmingalgorithms.com/algorithm/hsl-to-rgb/cpp/
///auteur non mentionné et code modifié pour ajout de fonctions
class RGB
{
public:
	unsigned char R;
	unsigned char G;
	unsigned char B;

	RGB(unsigned char r, unsigned char g, unsigned char b)
	{
		R = r;
		G = g;
		B = b;
	}

	bool Equals(RGB rgb)
	{
		return (R == rgb.R) && (G == rgb.G) && (B == rgb.B);
	}

	unsigned char getR() { return R; };
	unsigned char getG() { return G; };
	unsigned char getB() { return B; };
};

class HSL
{
public:
	int H;
	float S;
	float L;

	HSL(int h, float s, float l)
	{
		H = h;
		S = s;
		L = l;
	}

	bool Equals(HSL hsl)
	{
		return (H == hsl.H) && (S == hsl.S) && (L == hsl.L);
	}
};

RGB HSLToRGB(HSL hsl);
float HueToRGB(float v1, float v2, float vH);

///code obtenu lors des TPS du S3 puis modifié
class Svgfile
{
    public:
        Svgfile(std::string _filename = "output.svg", int _width=1000, int _height=800);
        ~Svgfile();

        void addDisk(double x, double y, double r, std::string color=defcol);
        void addCircle(double x, double y, double r, double ep, std::string color=defcol);
        void addTriangle(double x1, double y1, double x2, double y2,
                         double x3, double y3, std::string colorFill,
                          double thickness, std::string colorStroke);
        void addTriangle(double x1, double y1, double x2, double y2,
                         double x3, double y3, std::string colorFill=defcol);
        void addRectangle(double x1, double y1, double x2, double y2,
                          double x3, double y3, double x4, double y4, std::string colorFill);
        void addRectangle(double x1, double y1, double width, double height, std::string color);
        void addRectangle(double x, double y, double width, double height, double rx, double ry, std::string color);
        void addLine(double x1, double y1, double x2, double y2, std::string color=defcol);
        void addArrow(double x1, double y1, double x2, double y2, double x3, double y3, std::string color);
        void addCross(double x, double y, double span, std::string color=defcol);

        void addText(double x, double y, std::string text, std::string color=defcol);
        void addText(double x, double y, double val, std::string color=defcol);

        void addGrid(double span=100.0, bool numbering=true, std::string color="lightgrey");

        static std::string makeRGB(int r, int g, int b);
        void addDisk(double x, double y, double r, RGB);

        /// Type non copiable
        Svgfile(const Svgfile&) = delete;
        Svgfile& operator=(const Svgfile&) = delete;

        static bool s_verbose;

    private:
        std::string m_filename;
        std::ofstream m_ostrm;
        int m_width;
        int m_height;

        // Pour éviter les ouverture multiples
        static std::set<std::string> s_openfiles;
};



#endif // SVGFILE_H_INCLUDED

