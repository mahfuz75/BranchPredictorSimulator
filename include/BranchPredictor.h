#include <iostream>
#include <cmath>
#include <map>
using namespace std;
#ifndef BRANCHPREDICTOR_H
#define BRANCHPREDICTOR_H

#define MAX_ADDRESS_SIZE 32

class BranchPredictor
{
    public:
        BranchPredictor();
        BranchPredictor(unsigned int BP_TYPE, unsigned int M2);
        BranchPredictor(unsigned int BP_TYPE, unsigned int M1, unsigned int N);
        BranchPredictor(unsigned int BP_TYPE, unsigned int K, unsigned int M1, unsigned int N, unsigned int M2);
        virtual ~BranchPredictor();

        unsigned int BP_TYPE;
        unsigned int M1;
        unsigned int M2;
        unsigned int K;
        unsigned int N;
        unsigned int GLOBAL_BRANCH_HISTORY;
        map < unsigned int, unsigned int > BIMODAL_TABLE;
        map < unsigned int, unsigned int > GSHARE_TABLE;
        map < unsigned int, unsigned int > CHOOSER_TABLE;
        void init()
        {
            unsigned int i, j;

            BIMODAL_TABLE = map <unsigned int, unsigned int> ();
            j = (unsigned int)pow(2, M2);
            for(i=0; i<j; i++)
                BIMODAL_TABLE[i] = 4;

            GSHARE_TABLE = map <unsigned int, unsigned int> ();
            j = (unsigned int)pow(2, M1);
            for(i=0; i<j; i++)
                GSHARE_TABLE[i] = 4;

            j = (unsigned int)pow(2, K);
            for(i=0; i<j; i++)
                CHOOSER_TABLE[i] = 1;

            GLOBAL_BRANCH_HISTORY = 0;
        }
        string Predict(unsigned int iAddress, string decision)
        {
            unsigned int b_index, g_index, c_index;
            string prediction;
            if(BP_TYPE == 0)
            {
                b_index = GetBimodalIndex(iAddress);
                if(BIMODAL_TABLE[b_index] >= 4)
                    prediction = "t";
                else
                    prediction = "n";

                UpdateBimodalPredictor(b_index, decision == "t" ? true : false);
                return prediction;
            }
            else if(BP_TYPE == 1)
            {
                //unsigned int outcome;
                g_index = GetGshareIndex(iAddress);
                if(GSHARE_TABLE[g_index] >= 4)
                    prediction = "t";
                else
                    prediction = "n";

                UpdateGsharePredictor(g_index, decision == "t" ? true : false);
                GLOBAL_BRANCH_HISTORY = (decision == "t") ? ((GLOBAL_BRANCH_HISTORY >> 1) | (1 << (N - 1))) : ((GLOBAL_BRANCH_HISTORY >> 1) | (0 << (N - 1)));
                return prediction;
                //GLOBAL_BRANCH_HISTORY = (GLOBAL_BRANCH_HISTORY >> 1) | (outcome << (N - 1));
            }
            else
            {
                b_index = GetBimodalIndex(iAddress);
                g_index = GetGshareIndex(iAddress);
                c_index = GetChooserIndex(iAddress);
                string g_prediction, b_prediction;
                if(GSHARE_TABLE[g_index] >= 4)
                    g_prediction = "t";
                else
                    g_prediction = "n";

                if(BIMODAL_TABLE[b_index] >= 4)
                    b_prediction = "t";
                else
                    b_prediction = "n";

                if(CHOOSER_TABLE[c_index] >= 2)
                {
                    prediction = g_prediction;
                    UpdateGsharePredictor(g_index, decision == "t" ? true : false);
                }
                else
                {
                    prediction = b_prediction;
                    UpdateBimodalPredictor(b_index, decision == "t" ? true : false);
                }
                GLOBAL_BRANCH_HISTORY = (decision == "t") ? ((GLOBAL_BRANCH_HISTORY >> 1) | (1 << (N - 1))) : ((GLOBAL_BRANCH_HISTORY >> 1) | (0 << (N - 1)));

                //Update chooser
                if(decision == g_prediction && decision != b_prediction)
                    CHOOSER_TABLE[c_index] = min((int)CHOOSER_TABLE[c_index] + 1, 3);
                else if(decision != g_prediction && decision == b_prediction)
                    CHOOSER_TABLE[c_index] = max((int)CHOOSER_TABLE[c_index] - 1, 0);

                return prediction;
            }
            //}
        }
        void UpdateBimodalPredictor(unsigned int index, bool outcome)
        {
            if(outcome)
                BIMODAL_TABLE[index] = min((int)BIMODAL_TABLE[index] + 1, 7);
            else
                BIMODAL_TABLE[index] = max((int)BIMODAL_TABLE[index] - 1, 0);
        }
        void UpdateGsharePredictor(unsigned int index, bool outcome)
        {
            if(outcome)
                GSHARE_TABLE[index] = min((int)GSHARE_TABLE[index] + 1, 7);
            else
                GSHARE_TABLE[index] = max((int)GSHARE_TABLE[index] - 1, 0);
        }
        unsigned int GetBimodalIndex(unsigned int iAddress)
        {
            return (iAddress << (MAX_ADDRESS_SIZE - M2 - 2)) >> (MAX_ADDRESS_SIZE - M2);
        }
        unsigned int GetGshareIndex(unsigned int iAddress)
        {
            unsigned int index = (iAddress << (MAX_ADDRESS_SIZE - M1 - 2)) >> (MAX_ADDRESS_SIZE - M1);
            return GLOBAL_BRANCH_HISTORY ^ index;
        }
        unsigned int GetChooserIndex(unsigned int iAddress)
        {
            return (iAddress << (MAX_ADDRESS_SIZE - K - 2)) >> (MAX_ADDRESS_SIZE - K);
        }
        void PrintContents()
        {
            unsigned int i, j;
            if(BP_TYPE == 0)
            {
                cout << "FINAL BIMODAL CONTENTS"<< endl;
                j = (unsigned int)pow(2, M2);
                for(i=0; i<j; i++)
                    cout << i << '\t' << BIMODAL_TABLE[i] << '\n';
            }
            else if(BP_TYPE == 1)
            {
                cout << "FINAL GSHARE CONTENTS"<< endl;
                j = (unsigned int)pow(2, M1);
                for(i=0; i<j; i++)
                    cout << i << '\t' << GSHARE_TABLE[i] << '\n';
            }
            else
            {
                cout << "FINAL CHOOSER CONTENTS"<< endl;
                j = (unsigned int)pow(2, K);
                for(i=0; i<j; i++)
                    cout << i << '\t' << CHOOSER_TABLE[i] << '\n';

                cout << "FINAL GSHARE CONTENTS"<< endl;
                j = (unsigned int)pow(2, M1);
                for(i=0; i<j; i++)
                    cout << i << '\t' << GSHARE_TABLE[i] << '\n';

                cout << "FINAL BIMODAL CONTENTS"<< endl;
                j = (unsigned int)pow(2, M2);
                for(i=0; i<j; i++)
                    cout << i << '\t' << BIMODAL_TABLE[i] << '\n';
            }
        }

    protected:

    private:
};

#endif // BRANCHPREDICTOR_H
