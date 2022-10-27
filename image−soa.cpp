#include <iostream>
#include <fstream>
#include "soa/imagesoa.hpp"

using namespace std;

bool indir_existe(const string& nombre)
{
    ifstream archivo(nombre.c_str());
    return archivo.good();
}
bool outdir_existe(const string& nombre)
{
    ifstream archivo(nombre.c_str());
    return archivo.good();
}
int main() {
    const int width = 640;
    const int height = 480;
    Image image(width, height);
    string in_path;
    cout << "Input path:\n";
    cin >> in_path;
    string out_path;
    cout << "Output path:\n";
    cin >> out_path;
    string oper;
    cout << "Operation:\n";
    cin >> oper;
    string copy("copy");
    string mono("mono");
    string histo("histo");
    string gauss("gauss");
    if(in_path.empty() or out_path.empty() or oper.empty())
    {
        cout << "Wrong format:\n";
        cout << "image in_path out_path oper\n";
        cout << "operation: copy, histo, mono, gauss\n";
    }
    if (oper not_eq copy or oper not_eq histo or oper not_eq gauss or oper not_eq mono)
    {
        cout << "Unexpected operation:" << oper << "\n";
        cout << "image in_path out_path oper\n";
        cout << "operation: copy, histo, mono, gauss\n";
    }
    if (indir_existe(in_path))
    {
        cout << "Cannot open directory [" << in_path <<"]\n";
        cout << "image in_path out_path oper\n";
        cout << "operation: copy, histo, mono, gauss\n";

    }
    if (outdir_existe(out_path))
    {
        cout << "Output directory [" << out_path <<"] does not exist\n";
        cout << "image in_path out_path oper\n";
        cout << "operation: copy, histo, mono, gauss\n";

    }
    if (oper == copy){
        Image Copy(0,0);
        Copy.Read("../../../in/balloon.bmp");
        Copy.Export("copy.bmp");
    }
    if (oper == mono){
        Image Mono(0,0);
        Mono.Mono("../../../in/balloon.bmp");
        Mono.Export("mono.bmp");
    }
    if (oper == histo){
        Image Histo(0,0);
        Histo.Histo("../../../in/balloon.bmp");
    }

    return 0;
}
