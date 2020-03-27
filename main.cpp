#include <iostream>
#include <cstdio>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <sstream>
using namespace std;

#include "BranchPredictor.h"

#define OUTPUT_FILE "output.txt"
#define DEBUG_FILE "debug.txt"

bool DEBUG = false;

ifstream iFILE;
ofstream dFILE, oFILE;

unsigned int string2int(string s);
void show_help(string filename);
bool validate_parameters(int param_cnt, char **input_params);
unsigned int hex2dec(string hex);

int main(int argc, char **argv)
{
    //freopen(OUTPUT_FILE, "w", stdout);
    if(validate_parameters(argc, argv) == false)
    {
        show_help(argv[0]);
        return 0;
    }
    int M1, M2, N, K;
    char *branch_predictor_type = argv[1];

    BranchPredictor BP = BranchPredictor();

    char *TRACE_FILE;
    if(strcmp(argv[1], "bimodal") == 0)
    {
        BP = BranchPredictor(0, string2int(argv[2]));
        TRACE_FILE = argv[3];
    }
    else if(strcmp(argv[1], "gshare") == 0)
    {
        BP = BranchPredictor(1, string2int(argv[2]), string2int(argv[3]));
        TRACE_FILE = argv[4];
    }
    else
    {
        BP = BranchPredictor(2, string2int(argv[2]), string2int(argv[3]), string2int(argv[4]), string2int(argv[5]));
        TRACE_FILE = argv[6];
    }

    iFILE.open(TRACE_FILE);
    //oFILE.open(OUTPUT_FILE);

    if (iFILE.is_open())
    {
        cout << "COMMAND" << endl;
        if(strcmp(argv[1], "bimodal") == 0)
            cout << "./sim bimodal "<< argv[2] << " " << argv[3] << endl;
        else if(strcmp(argv[1], "gshare") == 0)
            cout << "./sim gshare "<< argv[2] << " " << argv[3] << " " << argv[4] << endl;
        else
            cout << "./sim hybrid "<< argv[2] << " " << argv[3] << " " << argv[4] << " " << argv[5] << " " << argv[6] << endl;
        cout << "OUTPUT" << endl;

        unsigned int trc_count = 0;
        unsigned int misprediction_count = 0;
        string hex_address, decision, prediction;
        unsigned int iAddress, index;
        while (!iFILE.eof() && iFILE >> hex_address >> decision)
        {
            iAddress = hex2dec(hex_address);
            //cout << iAddress << endl;
            //index = BP.GetIndex(iAddress);
            //cout << index << endl;
            prediction = BP.Predict(iAddress, decision);
            //cout << prediction << " " << decision << endl;
            if(prediction != decision)
                misprediction_count++;
            //BP.UpdatePredictor(index, decision);
            trc_count++;
        }
        double misprediction_rate = (misprediction_count / (double)trc_count) * 100;
        cout << "number of predictions:\t\t" << trc_count << endl;
        cout << "number of mispredictions:\t" << misprediction_count << endl;
        printf("misprediction rate:\t\t%.2lf%%\n", misprediction_rate);

        BP.PrintContents();
    }

    iFILE.close();
    //oFILE.close();

    return 0;
}

bool validate_parameters(int param_cnt, char **input_params)
{
    bool is_valid = true;
    if(!(param_cnt == 4 || param_cnt == 5 || param_cnt == 7))
    {
        cout << "Error: Invalid parameter count" << endl;
        return false;
    }
    if(!(strcmp(input_params[1], "bimodal") == 0 || strcmp(input_params[1], "gshare") == 0 || strcmp(input_params[1], "hybrid") == 0))
    {
        cout << "Error: Branch predictor type should be bimodal, gshare or hybrid" << endl;
        is_valid = false;
    }
    char *TRACE_FILE;
    if(strcmp(input_params[1], "bimodal") == 0)
    {
        if(param_cnt != 4)
            is_valid == false;
        if(string2int(input_params[2]) < 1)
        {
            cout << "Error: Invalid M2" <<endl;
            is_valid = false;
        }
        TRACE_FILE = input_params[3];
    }
    else if(strcmp(input_params[1], "gshare") == 0)
    {
        if(param_cnt != 5)
            is_valid == false;
        if(string2int(input_params[2]) < 1)
        {
            cout << "Error: Invalid M1" <<endl;
            is_valid = false;
        }
        if(string2int(input_params[3]) < 1)
        {
            cout << "Error: Invalid N" <<endl;
            is_valid = false;
        }
        TRACE_FILE = input_params[4];
    }
    else
    {
        if(param_cnt != 7)
            is_valid == false;
        if(string2int(input_params[2]) < 1)
        {
            cout << "Error: Invalid K" <<endl;
            is_valid = false;
        }
        if(string2int(input_params[3]) < 1)
        {
            cout << "Error: Invalid M1" <<endl;
            is_valid = false;
        }
        if(string2int(input_params[4]) < 1)
        {
            cout << "Error: Invalid N" <<endl;
            is_valid = false;
        }
        if(string2int(input_params[5]) < 1)
        {
            cout << "Error: Invalid M2" <<endl;
            is_valid = false;
        }
        TRACE_FILE = input_params[6];
    }
    ifstream iFILE;
    iFILE.open(TRACE_FILE);
    if (!iFILE)
    {
        cout << "ERROR: " << TRACE_FILE << " not exists" << endl;
        is_valid = false;
    }
    iFILE.close();

    return is_valid;
}

void show_help(string filename)
{
    cout << "\nFor 'bimodal' branch predictor" << endl;
    cout << "Usage: ./sim bimodal <M2> <trace_file>\n" << endl;
    cout << "For 'gshare' branch predictor" << endl;
    cout << "Usage: ./sim gshare <M1> <N> <trace_file>\n" << endl;
    cout << "For 'hybrid' branch predictor" << endl;
    cout << "Usage: ./sim hybrid <K> <M1> <N> <M2> <trace_file>\n" << endl;
    cout << "Where: " << endl;
    cout << "M1 - number of PC bits used to index the gshare table" << endl;
    cout << "M2 - number of PC bits used to index the bimodal table" << endl;
    cout << "N - number of global branch history register bits used to index the gshare table" << endl;
    cout << "K - number of PC bits used to index the chooser table" << endl;
}

unsigned int string2int(string s)
{
    unsigned int len = s.size();
    unsigned int res = 0;
    bool is_valid = true;
    for (unsigned int i=0; i<len; i++)
    {
        if (isdigit(s[i]))
            res = (res * 10 + s[i] - '0');
        else
            is_valid = false;
    }
    if (is_valid)
        return res;
    return -1;

}

unsigned int hex2dec(string hex)
{
    unsigned int dec;
    hex = "0x" + hex;
    stringstream ss;
    ss << std::hex << hex;
    ss >> dec;
    return dec;
}
