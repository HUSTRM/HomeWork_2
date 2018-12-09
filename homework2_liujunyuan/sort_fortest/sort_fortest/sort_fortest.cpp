/*--------------------------------------------------------------------------------------------------------------------------------------
Copyright (C),2018---,   HUST Liu
 File name:sort_fortest.cpp
 Author: liu       Version: 1     Date:2018.12.5
 Description:
using Merge ,Quickly Sort to sort a range of numbers.
using:function_sort.h
-----------------------------------------------------------------------------------------------------------------------------------------
 Others:NONE
-----------------------------------------------------------------------------------------------------------------------------------------
 History  as folwing :
 NONE
  --------------------------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "function_sort.h"
using namespace std;
int main()
{
	double numbers[1000] ;
	double num[1000];
	double num1[1000];
	for (int flag = 0; flag != 1000; flag++)
	{
		double flag1 = (double)flag;
		numbers[flag] = 600 * cos(flag);
		cout << numbers[flag] << endl;
	}
	Merg(numbers,num,0,999,1000);
	Quickly(numbers, num1, 0, 999);
	for (int flag = 0; flag != 1000; flag++)
	{
		cout << num[flag] << endl;
		cout << num1[flag] << endl;
	}
	getchar();
	return 0;
}