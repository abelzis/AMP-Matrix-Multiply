// AMPTestClient.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include "AMPTestLib.h"
#include <iostream>
#include <time.h>
using namespace concurrency;
using std::vector;

int const size = 50000000;	//define array aCPP, bCPP, sumCPP size

int aCPP[size];
int bCPP[size];
int sumCPP[size];

//Adding two arrays into third in parallel function
void CppAmpMethod() {
	int x = 100;	//matrix values between 1 and and x

	srand(time(NULL));	//randomness seed

	double start = clock();	//begin time

	for (int i = 0; i < size; i++)
	{
		aCPP[i] = 1 + rand() % x;
		bCPP[i] = 1 + rand() % x;
		//std::cout << "aCPP[" << i << "] = " << aCPP[i] << "\n";
		//std::cout << "bCPP[" << i << "] = " << bCPP[i] << "\n";
	}

	//std::cout << "\n\n\n";



	// Create C++ AMP objects.
	array_view<const int, 1> a(size, aCPP);
	array_view<const int, 1> b(size, bCPP);
	array_view<int, 1> sum(size, sumCPP);
	sum.discard_data();

	parallel_for_each(
		// Define the compute domain, which is the set of threads that are created.
		sum.extent,
		// Define the code to run on each thread on the accelerator.
		[=](index<1> idx) restrict(amp) {
		sum[idx] = a[idx] + b[idx];
	}
	);
	double end = clock();	//end time

	// Print the results. The expected output is "7, 9, 11, 13, 15".
	//for (int i = 0; i < size; i++) {
		//std::cout << sum[i] << "\n";
	//}

	std::cout << "Parallelization took " << (end - start) / CLOCKS_PER_SEC << " sec. time.\n";
}

//add two arrays into third linearly function
void CppNormalMethod()
{
	int x = 100;	//between 1 and x

	srand(time(NULL));

	double start = clock();

	for (int i = 0; i < size; i++)
	{
		aCPP[i] = 1 + rand() % x;
		bCPP[i] = 1 + rand() % x;
		//std::cout << "aCPP[" << i << "] = " << aCPP[i] << "\n";
		//std::cout << "bCPP[" << i << "] = " << bCPP[i] << "\n";
	}

	for (int i = 0; i < size; i++)
		sumCPP[i] = aCPP[i] + bCPP[i];

	double end = clock();
	std::cout << "Linear calculations took " << (end - start) / CLOCKS_PER_SEC << " sec. time.\n";
}

//matrix multiplication function
void MultiplyMatrixes(vector<int>& vA, vector<int>& vB, vector<int>& vC, int M, int N, int P)
{
	array_view<const int, 2> a(M, N, vA), b(N, P, vB);	//special type concurrency::array_view (similar to vector)
	array_view<int, 2> c(M, P, vC);
	c.discard_data();
	//begin parallelism
	parallel_for_each(c.extent, [=](index<2> idx) restrict(amp)	//using GPU
	{
		int row = idx[0]; int col = idx[1];
		int sum = 0;
		for (int i = 0; i < b.extent[0]; i++)
			sum += a(row, i) * b(i, col);	//matrix multiplication
		c[idx] = sum;
	});
	//end parallelism
	c.synchronize();	//sincronize data

	//std::cout << "c:\n";
	//for (int i = 0; i < M; i++)
	//{
	//	for (int j = 0; j < N; j++)
	//	{
	//		std::cout << c(i*M, j) << " ";
	//	}
	//	std::cout << "\n";
	//}
 }


//matrix initialization function
void MatrixInitialize()
{
   // Rows and columns for matrix
   const int M = 4000;
   const int N = 4000;
   const int P = 4000;

   // Create storage for a matrix of above size
   vector<int> vA(M * N);
   vector<int> vB(N * P);

   std::cout << "Matrix A size: " << M << "x" << N << "\n";
   std::cout << "Matrix B size: " << N << "x" << P << "\n";
   std::cout << "Matrix C size: " << M << "x" << P << "\n";


   // Populate matrix objects
   int i = 0;
   std::cout << "Initializing matrixes...\n";
   double start_init = clock();	//begin initialization of matrixes timer
   std::generate(vA.begin(), vA.end(), [&i]() {return i++; });	//matrix vA values range from 0 to -i
   std::generate(vB.begin(), vB.end(), [&i]() {return i--; });	//matrix vB values range from 0 to +i
   std::cout << "Time taken: " << (clock() - start_init) / CLOCKS_PER_SEC << " sec\n";	//end and print time taken

   //std::cout << "vA:\n";
   //for (int i = 0; i < M; i++)
   //{
	  // for (int j = 0; j < N; j++)
	  // {
		 //  std::cout << vA[i*M + j] << " ";
	  // }
	  // std::cout << "\n";
   //}

   //std::cout << "vB:\n";
   //for (int i = 0; i < M; i++)
   //{
	  // for (int j = 0; j < N; j++)
	  // {
		 //  std::cout << vB[i*M + j] << " ";
	  // }
	  // std::cout << "\n";
   //}

   // Output storage for matrix calculation
   vector<int> vC(M * P);

   std::cout << "Multiplying matrixes...\n";
   double start_mult = clock();	//start multiplication timer
   MultiplyMatrixes(vA, vB, vC, M, N, P);
   std::cout << "Done. Time taken: " << (clock() - start_mult) / CLOCKS_PER_SEC << " sec\n";	//end and print time taken
 }




int main()
{
	EnumerateAccelerators();	//return available CPU and GPU devices and accelerators

	while (1)
	{
		char r = getchar();
		if (r == 'g')
			CppAmpMethod();	//add two array's values into third linearly
		if (r == 'n')
			CppNormalMethod();	//add two array's values into third in parallel
		if (r == 'd')
			MatrixInitialize();	//multiply matrixes
		if (r == 'e')
			return 0;
		//system("pause");
	}
	return 0;
}