#include <iostream>
#include <fstream>
#include <string>
#include <regex>
//#include "BlockClass.cpp"

using namespace std;

int main(int argc, char* argv[]) //////////////Diff on line 2187//////////////////
{

    string oldPath = argv[1], newPath = argv[2], outPath = argv[3];

    ofstream outfile;
    ifstream oldfile;
    ifstream newfile;
    outfile.open(outPath);
    oldfile.open(oldPath);
    newfile.open(newPath);


    int prevnew = 0, prevold = 0;//, getlineCheckNew = 0, getlineCheckOld = 0;
    int currold = 0, currnew = 0;
    string line1, temp1, currentDllOld = "", currentDllNew = "", lastReadDll = "";
    string line2, temp2;
    int linecount = 0;
    int diffcount = 1, najden = 0;

    regex a("^[A-Za-z0-9\\_\\-]+\\+0x");
    regex b("\\+0x+[0-9a-fA-F]+");

    getline(oldfile, line1);
    getline(newfile, line2);

    temp1 = regex_replace(line1, a, "");
    temp2 = regex_replace(line2, a, "");
    currentDllNew = regex_replace(line2, b, "");
    currentDllOld = regex_replace(line1, b, "");

    prevold = stoi(temp1, 0, 16);
    prevnew = stoi(temp2, 0, 16);
    cout << line1 << "\n";
    bool flag = 0, align = 0;
    int consecutivecountfail = 0;
    int consecutivecountsuccess = 0;

    string previous1 = "";
    string previous2 = "";

    do {
        string moduleName1 = regex_replace(line1, b, "");
        string moduleName2 = regex_replace(line2, b, "");
        if (previous1.compare("") && previous2.compare("")) {
            if (moduleName2.compare(previous2) != 0 && moduleName2.compare(moduleName1) != 0) {
                previous1 = moduleName1;
                getline(oldfile, line1);
                moduleName1 = regex_replace(line1, b, "");
                linecount++;
                if (!najden) {
                    outfile << linecount << ": New dll not aligned\n";
                    outfile << "Module diff here: " <<diffcount++<<endl;
                }
                najden = 1;
            }
            else if (moduleName1.compare(previous1) != 0 && moduleName2.compare(moduleName1) != 0) {
                previous2 = moduleName2;
                getline(newfile, line2);
                moduleName2 = regex_replace(line2, b, "");
                if (!najden) {
                    outfile << linecount << ": Old dll not aligned\n";
                    outfile << "Module diff here: " << diffcount++ << endl;
                }
                najden = 1;
            }
            else {
                previous1 = moduleName1;
                previous2 = moduleName2;
                if (najden) {
                    outfile << linecount+1 << ": Corrected the file aligment!\n";
                    //outfile << "Old module instruction address: " << line1 << "\n New module instruction address: " << line2 << endl;
                    najden = 0;
                }
                getline(oldfile, line1);
                getline(newfile, line2);
                moduleName2 = regex_replace(line2, b, "");
                moduleName1 = regex_replace(line1, b, "");
                linecount++;
                
            }
        }
        else {
            if (moduleName1.compare(moduleName2)) cout << "First line Error!\n";
            previous1 = moduleName1;
            previous2 = moduleName2;
        }


        temp1 = regex_replace(line1, a, "");
        temp2 = regex_replace(line2, a, "");
        currold = stoi(temp1, 0, 16);
        currnew = stoi(temp2, 0, 16);
        
        int deltaNew = currnew - prevnew;
        int deltaOld = currold - prevold;

        if (deltaOld > 15 || deltaNew > 15 || deltaNew < 0 || deltaOld < 0 || najden);//check for 15byte offset. Ignore and compare next instrction
        else if ((deltaOld != deltaNew) && !flag && !najden) {
            //cout << "\n Difference found on line " << linecount << "\n";
            outfile << diffcount << ". instruction diff here.\n";

            diffcount++;
            flag = 1;
            consecutivecountsuccess = 0;
        }
        else{
            flag = 0;
            consecutivecountsuccess++;  //int overflow possible
        }
        if(!najden)outfile << line1 <<"\t\t"<< line2 << "\n";
        else outfile << line1 << "\t\t" << line2 << " - searching aligment\n";
        prevold = currold;
        prevnew = currnew;
    } while (!oldfile.eof() && !newfile.eof());
}
