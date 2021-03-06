#pragma on
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iomanip>
using namespace std;
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
//=========================================================================

#define POPULATION 50					//母體數量
#define PC 0.82							//交配機率Prob Crossover
#define PM 0.5							//突變機率Prob Mutation
#define GENERATION 200000				//代數
#define SIZE 30
#define CHOOSE_PATH 

//===================================================================================================
//Oliver30
int oliver30[30][2] = { { 87, 7 },{ 91,38 },{ 83,46 },{ 71,44 },{ 64,60 },{ 68,69 },{ 83,69 },{ 87,76 },{ 74,78 },{ 71,71 },
						{ 58,69 },{ 54,62 },{ 51,67 },{ 37,84 },{ 41,94 },{ 2,99 } ,{ 7 ,64 },{ 22,60 },{ 25,62 },{ 18,54 },
						{ 4, 50 },{ 13,40 },{ 18,40 },{ 24,42 },{ 25,38 },{ 41,26 },{ 45,21 },{ 44,35 },{ 58,35 },{ 62,32 } };
//Eil51
int Eil51[51][2] = { { 37,52 },{ 49,49 },{ 52,64 },{ 20,26 },{ 40,30 },{ 21,47 },{ 17,63 },{ 31,62 },{ 52,33 },{ 51,21 },
{ 42,41 },{ 31,32 },{ 5,25 } ,{ 12,42 },{ 36,16 },{ 52,41 },{ 27,23 },{ 17,33 },{ 13,13 },{ 57,58 },
{ 62,42 },{ 42,57 },{ 16,57 },{ 8,52 } ,{ 7,38 } ,{ 27,68 },{ 30,48 },{ 43,67 },{ 58,48 },{ 58,27 },
{ 37,69 },{ 38,46 },{ 46,10 },{ 61,33 },{ 62,63 },{ 63,69 },{ 32,22 },{ 45,35 },{ 59,15 },{ 5,6 },
{ 10,17 },{ 21,10 },{ 5,64 } ,{ 30,15 },{ 39,10 },{ 32,39 },{ 25,32 },{ 25,55 },{ 48,28 },{ 56,37 },
{ 30,40 } };
//Eil76
int Eil76[76][2] = { { 22,22 },{ 36,26 },{ 21,45 },{ 45,35 },{ 55,20 },{ 33,34 },{ 50,50 },{ 55,45 },{ 26,59 },{ 40,66 },
{ 55,65 },{ 35,51 },{ 62,35 },{ 62,57 },{ 62,24 },{ 21,36 },{ 33,44 },{ 9,56 },{ 62,48 } ,{ 66,14 },
{ 44,13 },{ 26,13 },{ 11,28 },{ 7,43 },{ 17,64 },{ 41,46 },{ 55,34 },{ 35,16 },{ 52,26 },{ 43,26 },
{ 31,76 },{ 22,53 },{ 26,29 },{ 50,40 },{ 55,50 },{ 54,10 },{ 60,15 },{ 47,66 },{ 30,60 },{ 30,50 },
{ 12,17 },{ 15,14 },{ 16,19 },{ 21,48 },{ 50,30 },{ 51,42 },{ 50,15 },{ 48,21 },{ 12,38 },{ 15,56 },
{ 29,39 },{ 54,38 },{ 55,57 },{ 67,41 },{ 10,70 },{ 6,25 } ,{ 65,27 },{ 40,60 },{ 70,64 },{ 64,4 },
{ 36,6 },{ 30,20 } ,{ 20,30 },{ 15,5 },{ 50,70 },{ 57,72 },{ 45,42 },{ 38,33 },{ 50,4 },{ 66,8 },
{ 59,5 },{ 35,60 } ,{ 27,24 },{ 40,20 },{ 40,37 },{ 40,40 } };

//======================================
void CalFitnessValue();
void CalDistance();
void RandomPath();
void ReproductionTS();
void ReproductionRWS();
void CrossoverPMX();
void CrossoverHGA();
void Mutation();
void UpdateBestPath(int);
void InitOpencvConfig();
void drawBest(int);