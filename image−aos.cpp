#include <iostream>
#include <fstream>

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
    string in_path;
    cin >> in_path;
    cout << "Input path: " << in_path << "\n";
    string out_path;
    cin >> out_path;
    cout << "Output path: " << out_path << "\n";
    string oper;
    cin >> oper;
    string copy("copy");
    string histo("histo");
    string gauss("gauss");
    if(in_path.empty() or out_path.empty() or oper.empty())
    {
        cout << "Wrong format:\n";
        cout << "image in_path out_path oper\n";
        cout << "operation: copy, histo, mono, gauss\n";
    }
    if (oper != copy or oper != histo or oper != gauss)
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

    return 0;
}