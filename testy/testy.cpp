#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>


using namespace std;
/*
string GetCurrentDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    string::size_type pos = string(buffer).find_last_of("\\/");
    return string(buffer).substr(0, pos);
}

string Read(string Path ) {
    string data;
    ifstream infile;// open a file in read mode.
    infile.open(Path);
    infile >> data;
    infile.close();// close the opened file.
    return data;
}

bool Write(string Path, string data) {
    ofstream outfile;// open a file in write mode.
    outfile.open(Path);
    outfile << data;// write inputted data into the file.
    outfile.close(); // close the opened file.
    if (Read(Path).compare(data) == 0)  return 1;
    else return 0;
}

BOOL FifoExists(LPCTSTR szPath,bool Fifo)
{
    DWORD dwAttrib = GetFileAttributes(szPath);
    if (Fifo) {
        if (dwAttrib == FILE_ATTRIBUTE_ARCHIVE) return 1;
        else return 0;
    }
    else {
        if (dwAttrib == FILE_ATTRIBUTE_DIRECTORY) return 1;
        else return 0;
    }

    //return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

*/

int main(void)
{
    string path;


    // Zápis do souboru.
    if (CreateDirectory( path.c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError())
    {
        path += "\\savefile.txt";
        string wdata = { "Radek,Lukas,Jarka" };
        bool zapis = Write(path, wdata);

        string rdata = Read(path);
        std::cout << rdata << endl;
    }
    else std::cout << "Failed to create directory." << endl;

    /*err = _dupenv_s(&pValue, &len, "PROGRAMDATA");
    path = pValue;
    std::cout << path << endl;

    err = _dupenv_s(&pValue, &len, "TEMP");
    path = pValue;
    std::cout << path << endl;

    std::cout << GetCurrentDirectory() << endl;*/


    


   

    return 0;
}