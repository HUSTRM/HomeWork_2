/*--------------------------------------------------------------------------------------------------------------------------------------
Copyright (C),2018---,   HUST Liu
 File name:sort_fortest.cpp
 Author: liu      Version: 1     Date:2018.12.5
 Description:
Merge Sort's function 
-----------------------------------------------------------------------------------------------------------------------------------------
 Others:NONE
 May Function List:Merge ,merge,change,Quickly
-----------------------------------------------------------------------------------------------------------------------------------------
 History  as folwing :
 NONE
  --------------------------------------------------------------------------------------------------------------------------------------*/
#pragma once
#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;

void merge(double input_array[], double output_array[], int min, int max, int mid) 
{
	int i = min;
	int k = min;	
	int j = mid + 1;
	while ((i <= mid) && (j <= max))
	{ 
		if (input_array[i] < input_array[j]) { output_array[k++] = input_array[i++]; 
		} 
		else
		{ 
			output_array[k++] = input_array[j++]; 
		} 
	}	
	while (i <= mid)
	{ 
		output_array[k++] = input_array[i++]; 
	}	
	while (j <= max) 
	{ 
		output_array[k++] = input_array[j++]; 
	} 
}


void Merg(double input_array[], double output_array[], int min, int max, int max_size) 
{
	int mid = (min + max) / 2;
	if (min == max) 
	{ 
		output_array[min] = input_array[min];
	}
	else 
	{ 
		int mid = (min + max) / 2;	
		double * output_array_space = (double*)malloc(sizeof(double) * max_size); 
		if (NULL != output_array_space)
		{ 
			Merg(input_array, output_array_space, min, mid, max_size);
			Merg(input_array, output_array_space, mid + 1, max, max_size);
			merge(output_array_space, output_array, min, max, mid);
		}	
		free(output_array_space); 
	} 
}

int change(double input_array[], int min, int max) 
{

	int i, j;
	double temp;
	i = min;
	j = max;

	while (i < j) 
    {
		while (i < j&&input_array[i] < input_array[j])
		{
			j--;
		}
		if (i < j)
		{
			temp = input_array[i];
			input_array[i] = input_array[j];
			input_array[j] = temp;
			i++;
		}
		while (i < j&&input_array[i] <= input_array[j])
		{
			i++;
		}
		if (i < j)
		{
			temp = input_array[i];
			input_array[i] = input_array[j];
			input_array[j] = temp;
			j--;
		}
	}
	return i;
}

void Quickly(double input_array[], double output_array[],int min, int max)
{
	int pivot;
	if (min < max)
	{
		pivot = change(input_array, min, max);
		Quickly(input_array, output_array,min, pivot - 1);
		Quickly(input_array, output_array,pivot + 1, max);
	}
	for (int l = 0; l != max + 1; l++)
	{
		output_array[l] = input_array[l];
	}
}
