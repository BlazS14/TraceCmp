// TraceLogParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//test for commit

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

int main() //////////////Diff on line 2187//////////////////
{
    string oldpath = "..\\logold.txt", newpath = "..\\lognew.txt", outpath = "..\\out.txt";
    /*cout << "Unpatched trace file path: ";
    cin >> oldpath;
    cout << "\nPatched trace file path: ";
    cin >> newpath;
    cout << "\nOutput file path: ";
    cin >> outpath;*/

    ofstream outfile;
    ifstream oldfile;
    ifstream newfile;
    outfile.open(outpath);
    oldfile.open(oldpath);
    newfile.open(newpath);


    int prevnew = 0, prevold = 0;
    int currold = 0, currnew = 0;
    string line1, temp1;
    string line2, temp2;
    int linecount = 0;
    int diffcount = 1;

    regex a("^[A-Za-z0-9\\_\\-]+\\+0x");
    regex b("0x+[0-9a-fA-F]+");

    getline(oldfile, line1);
    getline(newfile, line2);
    temp1 = regex_replace(line1, a, "");
    temp2 = regex_replace(line2, a, "");
    prevold = stoi(temp1, 0, 16);
    prevnew = stoi(temp2, 0, 16);
    cout << line1 << "\n";
    bool flag = 0, align = 0;
    int consecutivecountfail = 0;
    int consecutivecountsuccess = 0;


    while (getline(oldfile, line1) && getline(newfile, line2)) {
        temp1 = regex_replace(line1, a, "");
        temp2 = regex_replace(line2, a, "");
        currold = stoi(temp1, 0, 16);
        currnew = stoi(temp2, 0, 16);

        if (currold - prevold > 15 || currnew - prevnew > 15 || currold - prevold < 1 || currnew - prevnew < 1);//check for 15byte offset. Ignore and compare next instrction
        else if (((currold - prevold) != (currnew - prevnew)) && !flag) {
            cout << "\n Difference found on line " << linecount << "\n";
            outfile << "\n/////" << diffcount << ". Difference HERE/////\n";
            diffcount++;
            flag = 1;
            consecutivecountsuccess = 0;
        }
        else{
            flag = 0;
            consecutivecountsuccess++;  //int overflow possible
        }
        outfile << line1 << "\n";
        linecount++;
        prevold = currold;
        prevnew = currnew;
    }


}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
