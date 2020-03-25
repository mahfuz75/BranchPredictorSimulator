#include "BranchPredictor.h"

BranchPredictor::BranchPredictor()
{
    //ctor
}
BranchPredictor::BranchPredictor(unsigned int BP_TYPE, unsigned int M2)
{
    //ctor
    this->BP_TYPE = BP_TYPE;
    this->M2 = M2;

    this->K = 0;
    this->M1 = 0;
    this->N = 0;

    init();
    //this->PREDICTION_TABLE = map <unsigned int, unsigned int> ();
}
BranchPredictor::BranchPredictor(unsigned int BP_TYPE, unsigned int M1, unsigned int N)
{
    //ctor
    this->BP_TYPE = BP_TYPE;
    this->M1 = M1;
    this->N = N;

    this->K = 0;
    this->M2 = 0;

    init();
    //this->PREDICTION_TABLE = map <unsigned int, unsigned int> ();
}
BranchPredictor::BranchPredictor(unsigned int BP_TYPE, unsigned int K, unsigned int M1, unsigned int N, unsigned int M2)
{
    //ctor
    this->BP_TYPE = BP_TYPE;
    this->K = K;
    this->M1 = M1;
    this->N = N;
    this->M2 = M2;

    init();
    //this->PREDICTION_TABLE = map <unsigned int, unsigned int> ();
}

BranchPredictor::~BranchPredictor()
{
    //dtor
}
