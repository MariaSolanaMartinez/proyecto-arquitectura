#include "imagesoa.hpp"
#include <iostream>
#include <fstream>
#include <list>
#include <cmath>
using namespace std;
Color::Color()
    : r(0), g(0), b(0)
{}
Color::Color(float r, float g, float b)
    :r(r), g(g), b(b)
{}
Color::~Color(){}

Image::Image(int width, int height)
    :i_width(width), i_height(height), i_colors(vector<Color>(width*height))
    {}

Image::~Image(){}

Color Image::GetColor(int x, int y) const
{
    return i_colors[y * i_width + x];
}

void Image::SetColor(const Color& color, int x, int y)
{
    i_colors[y * i_width + x].r = color.r;
    i_colors[y * i_width + x].g = color.g;
    i_colors[y * i_width + x].b = color.b;
}

void Image::Read(const char* path){
    ifstream f;
    f.open(path, ios::in | ios:: binary);

    if (!f.is_open()) {
        cout << "File could not be opened" << endl;
        return;
    }
    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;

    unsigned char fileHeader[fileHeaderSize];
    f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

    unsigned char informationHeader[informationHeaderSize];
    f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
    i_width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
    i_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

    i_colors.resize(i_width * i_height);

    const int paddingAmount = ((4-(i_width *3)%4)%4);

    for (int y = 0; y < i_height; y++){
        for (int x = 0; x < i_width; x++){
            unsigned char color[3];
            f.read(reinterpret_cast<char*>(color), 3);
            i_colors[y * i_width + x].r = static_cast<float>(color[2]) / 255;
            i_colors[y * i_width + x].g = static_cast<float>(color[1]) / 255;
            i_colors[y * i_width + x].b = static_cast<float>(color[0]) / 255;
        }
        f.ignore(paddingAmount);
    }
    f.close();
    cout << "File read" << endl;
}
void Image::Mono(const char* path){
    ifstream f;
    f.open(path, ios::in | ios:: binary);

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;

    unsigned char fileHeader[fileHeaderSize];
    f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

    unsigned char informationHeader[informationHeaderSize];
    f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    i_width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
    i_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

    i_colors.resize(i_width * i_height);

    const int paddingAmount = ((4-(i_width *3)%4)%4);
    float nr;
    float ng;
    float nb;
    float cr;
    float cg;
    float cb;
    float tl;
    float gm;
    float g;
    for (int y = 0; y < i_height; y++){
        for (int x = 0; x < i_width; x++) {
            unsigned char color[3];
            f.read(reinterpret_cast<char *>(color), 3);
            nr = i_colors[y * i_width + x].r = static_cast<float>(color[2]) / 255;
            ng = i_colors[y * i_width + x].g = static_cast<float>(color[1]) / 255;
            nb = i_colors[y * i_width + x].b = static_cast<float>(color[0]) / 255;
            if (nr <= 0.04045 or ng <= 0.04045 or nb <= 0.04045){
                cr = nr / 12.92;
                cg = ng / 12.92;
                cb = nb / 12.92;
            }
            else{
                cr = pow(((nr + 0.055)/1.055), 2.4);
                cg = pow(((ng + 0.055)/1.055), 2.4);
                cb = pow(((nb + 0.055)/1.055), 2.4);
            }
            tl = 0.2126 * cr + 0.7152 * cg + 0.0722 * cb;
            if (tl <= 0.0031308){
                gm = 12.92 * tl;
            }
            else{
                gm = 1.055 * pow(tl, (1/2.4)) - 0.055;
            }
            g = gm * 255;
            Image::SetColor(Color(g, g, g), x, y);
        }
        f.ignore(paddingAmount);
    }
    f.close();
    cout << "File read" << endl;
}
void Image::Histo(const char *path){
    ofstream f;
    f.open(path, ios::out | ios::binary);
    if (!f.is_open())
    {
        cout << "File could not be opened\n";
        return;
    }
}
void Image::Export(const char* path) const
{
    ofstream f;
    f.open(path, ios::out | ios::binary);
    if (!f.is_open())
    {
        cout << "File could not be opened\n";
        return;
    }
    unsigned char bmpPad[3] = {0,0,0};
    const int paddingAmount = ((4-(i_width *3)%4)%4);

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize + informationHeaderSize + i_width * i_height + paddingAmount * i_height;

    unsigned char fileHeader[fileHeaderSize];
    // file type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    // file size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    // reserved 1 (not used)
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    // reserved 2 (not used)
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    // pixel data offset
    fileHeader[10] = fileHeaderSize + informationHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char informationHeader[informationHeaderSize];

    // header size
    informationHeader[0] = informationHeaderSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;
    // image width
    informationHeader[4] = i_width;
    informationHeader[5] = i_width >> 8;
    informationHeader[6] = i_width >> 16;
    informationHeader[7] = i_width >> 24;
    // image height
    informationHeader[8] = i_height;
    informationHeader[9] = i_height >> 8;
    informationHeader[10] = i_height >> 16;
    informationHeader[11] = i_height >> 24;
    // planes
    informationHeader[12] = 1;
    informationHeader[13] = 0;
    // bits per pixel(RGB)
    informationHeader[14] = 24;
    informationHeader[15] = 0;
    // compression (no compression)
    informationHeader[16] = 0;
    informationHeader[17] = 0;
    informationHeader[18] = 0;
    informationHeader[19] = 0;
    // image size (no compression)
    informationHeader[20] = 0;
    informationHeader[21] = 0;
    informationHeader[22] = 0;
    informationHeader[23] = 0;
    // x pixels per meter (not specified)
    informationHeader[24] = 0;
    informationHeader[25] = 0;
    informationHeader[26] = 0;
    informationHeader[27] = 0;
    // y pixels per meter (not specified)
    informationHeader[28] = 0;
    informationHeader[29] = 0;
    informationHeader[30] = 0;
    informationHeader[31] = 0;
    // total colors (color palette not used)
    informationHeader[32] = 0;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;
    // important colors (generally ignored)
    informationHeader[36] = 0;
    informationHeader[37] = 0;
    informationHeader[38] = 0;
    informationHeader[39] = 0;

    f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    for (int y = 0; y < i_height; y++){
        for (int x = 0; x < i_width; x++){
            unsigned char r = static_cast<unsigned char>(GetColor(x, y).r * 255.0f);
            unsigned char g = static_cast<unsigned char>(GetColor(x, y).g * 255.0f);
            unsigned char b = static_cast<unsigned char>(GetColor(x, y).b * 255.0f);

            unsigned char color[] = {b, g, r};

            f.write(reinterpret_cast<char*>(color), 3);
        }
        f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
    }
    f.close();
    cout << "file created\n";
}






//----------------------------//
/*void histo(){
    Mat image = imread("../../../in/ballon.bmp");
    histo(image.rows, image.cols, CV_8UC1);
    for(size_t i = 0; i < image.rows; i++)
    {
        for(size_t j = 0; j < image.cols; j++) {
            Vec3b pixel = image.at<Vec3b>(i, j);
            uchar B = pixel[0];
            uchar G = pixel[1];
            uchar R = pixel[2];
            if B = 0 
        }
    }
    imshow(mono);
}

void mono1(){
    Mat image = imread("../../../in/ballon.bmp");
    Mat mono(image.rows, image.cols, CV_8UC1);
    for(size_t i = 0; i < image.rows; i++)
    {
        for(size_t j = 0; j < image.cols; j++) {
            Vec3b pixel = image.at<Vec3b>(i, j);
            uchar B = pixel[0];
            uchar G = pixel[1];
            uchar R = pixel[2];
            mono.at<uchar>(i, j)=(B, G, R)/3;
        }
    }
    imshow(mono);
}

int main(){
    ofstream file;
    file.open("C:/ruta/archivos/histograma.hst");
    file << histo;
    file.close();
    return 0;
};*/