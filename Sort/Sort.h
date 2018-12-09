#pragma once
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <Windows.h>

using namespace std;

#define INT_RANDN 35
#define SWAP(a,b,t) (t)=(a);(a)=(b);(b)=(t)

void print(vector<unsigned> num);
vector<unsigned> RandGen(int n);
void qsortMain(vector<unsigned> &num, int low, int high);
unsigned qsortPart(vector<unsigned> &num, int low, int high);
void msortMain(vector<unsigned> &num, int from, int to, vector<unsigned> &res);
void msortMerge(vector<unsigned> &num, int from, int to, vector<unsigned> &res);