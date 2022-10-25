#include <iostream>
#include <vector>
using namespace std;
struct Color{
    float r, g, b;
    Color();
    Color(float r, float g, float b);
    ~Color();
};

class Image
{
public:
    Image(int width, int height);
    ~Image();

    Color GetColor(int x, int y) const;
    void SetColor(const Color& color, int x, int y);

    void Read(const char* path);
    void Mono(const char* path);
    void Histo(const char* path);
    void Export(const char* path)const;

private:
    int i_width;
    int i_height;
    vector<Color> i_colors;
};