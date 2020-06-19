#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

int main(int argc, char* argv[]) //////////////Diff on line 2187//////////////////b
{
    string oldpath = argv[1], newpath = argv[2], outpath = argv[3];

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
    regex b("\\+0x+[0-9a-fA-F]+");

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
