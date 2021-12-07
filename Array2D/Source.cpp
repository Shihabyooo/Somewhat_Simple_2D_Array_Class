#include <iostream>
//#include <math.h>
//#include <vector>
#include <chrono>

#include "Array2D.h"


int main(int argc, char * argv)
{
#pragma region testing the Array2D class
	std::cout << "Test begin\n";

	std::cout << "\n=========================================================================" << std::endl;
	std::cout << "Testing creating a 5x5 identity matrix: " << std::endl;
	Array2D identityMat = Array2D::Identity(5);
	identityMat.DisplayArrayInCLI();

	std::cout << "\n=========================================================================" << std::endl;
	std::cout << "Testing creating a 3x3 matrix and setting values through Array2D.SetValue(): " << std::endl;
	Array2D arr1(3, 3);
	arr1.SetValue(0, 0, 1.5f);		arr1.SetValue(0, 1, 0.2f);		arr1.SetValue(0, 2, 3.5f);
	arr1.SetValue(1, 0, 3.5f);		arr1.SetValue(1, 1, 5.76f);		arr1.SetValue(1, 2, 22.2f);
	arr1.SetValue(2, 0, 10.0f);		arr1.SetValue(2, 1, 0.05f);		arr1.SetValue(2, 2, 576.6f);
	arr1.DisplayArrayInCLI();

	std::cout << "\n=========================================================================" << std::endl;
	std::cout << "Testing Out-of-bounds access safety: " << std::endl;
	try
	{
		arr1[5][5] = 0.0; 
	}
	catch (const std::exception &exp)
	{
		std::cout << "Caught exceptions with message: " << exp.what() << std::endl;
	}

	std::cout << "\n=========================================================================" << std::endl;
	std::cout << "Testing array inversion through Array2D.Invert(): " << std::endl;
	Array2D arr1Inv = arr1.Invert();
	arr1Inv.DisplayArrayInCLI();

	std::cout << "\n=========================================================================" << std::endl;
	std::cout << "Testing array inversion through overloaded operator /= " << std::endl;
	Array2D arr1Inv_2;
	arr1Inv_2 /= arr1;
	arr1Inv_2.DisplayArrayInCLI();

	std::cout << "\n=========================================================================" << std::endl;
	std::cout << "Testing array multiplication, multiplying the 3x3 array above with its inverse (result should be 3x3 identity matrix):" << std::endl;
	Array2D arr1Mult = arr1 * arr1Inv;
	arr1Mult.DisplayArrayInCLI();

	std::cout << "\n=========================================================================" << std::endl;
	std::cout << "Testing array substraction, substracting the two inverse matrixes obtained above (result should be a zero matrix):" << std::endl;
	Array2D arr1Subst = arr1Inv - arr1Inv_2;
	arr1Subst.DisplayArrayInCLI();

	std::cout << "\n=========================================================================" << std::endl;
	std::cout << "Testing array assignment and read through overloaded double brackets [][]:" << std::endl;
	Array2D arr2(2, 2);
	
	for (int i = 0; i < 2; i++) //inputing any data based on iterator, just for quick testing.
	{
		arr2[i][0] = i + (double)i / 2.0f;
		arr2[i][1] = i + 2.0f;
	}	

	std::cout << "====== Reading values using: [][]:" << std::endl;
	for (int i = 0; i < 2; i++)
	{
		std::cout << std::fixed << std::setw(8) << std::setprecision(4) << arr2[i][0] << "\t" << arr2[i][1] << std::endl;
	}
	std::cout << "====== Content using built in display method" << std::endl;
	arr2.DisplayArrayInCLI();


	std::cout << "\n=========================================================================" << std::endl;
	std::cout << "Testing determinant calculation of the following array:" << std::endl;
	Array2D arr3(4, 4);
	arr3.SetValue(0, 0, 1.5f);		arr3.SetValue(0, 1, 0.2f);		arr3.SetValue(0, 2, 3.5f);		arr3.SetValue(0, 3, 3.5f);
	arr3.SetValue(1, 0, 3.5f);		arr3.SetValue(1, 1, 5.76f);		arr3.SetValue(1, 2, 22.2f);		arr3.SetValue(1, 3, 3.5f);
	arr3.SetValue(2, 0, 10.0f);		arr3.SetValue(2, 1, 0.05f);		arr3.SetValue(2, 2, 576.6f);	arr3.SetValue(2, 3, 3.5f);
	arr3.SetValue(3, 0, 3.7f);		arr3.SetValue(3, 1, 55.0f);		arr3.SetValue(3, 2, 22.2f);		arr3.SetValue(3, 3, 3.5f);
	arr3.DisplayArrayInCLI();

	std::cout << "\nThe determinant is: " << arr3.Determinant() << std::endl;

	std::cout << "\n=========================================================================" << std::endl;
	std::cout << "Testing equality:" << std::endl;
	std::cout << "===== Between the determenant test array and itself:  ";// << std::endl;
	if (arr3 == arr3)
		std::cout << "They are equall!" << std::endl;
	else
		std::cout << "!!!!!!! Something is wrong" << std::endl;

	std::cout << "===== Between the det-test array and itself after multiplication with 100:  ";// << std::endl;
	if (arr3 == (arr3 * 100.0f))
		std::cout << "!!!!!!! Something is wrong" << std::endl;
	else
		std::cout << "They are not equall" << std::endl;


	std::cout << "===== Between the det-test array and a part of itself (quarter) (testing != overload):  ";// << std::endl;
	if (arr3 != arr3.GetSubMatrix(0, 2, 0, 2))
		std::cout << "They are not equall!" << std::endl;
	else
		std::cout << "!!!!!!! Something is wrong" << std::endl;

	std::cout << "===== Between the det-test array and itself after multiplication with 1.001:  ";// << std::endl;
	if (arr3 == (arr3 * 1.01f))
		std::cout << "!!!!!!! Something is wrong" << std::endl;
	else
		std::cout << "They are not equall" << std::endl;

	std::cout << "\nTesting near-equality (nearly equal to):" << std::endl;
	std::cout << "===== Between the det-test array and itself after adding it to an equall sized array with all elements of 0.01f, with tolerance of 0.015:  ";// << std::endl;
	if (arr3.NearlyEquals((arr3 + Array2D(4, 4, 0.01f)), 0.015f))
		std::cout << "They are nearly equall!" << std::endl;
	else
		std::cout << "!!!!!!! Something is wrong" << std::endl;

	std::cout << "\n=========================================================================" << std::endl;
	std::cout << "Testing symmetry:" << std::endl;
	std::cout << "Of the det-test array:  ";// << std::endl;
	if (!arr3.IsSymmetric())
		std::cout << "Nope! Not symmetric." << std::endl;
	else
		std::cout << "!!!!!!! Something is wrong" << std::endl;

	std::cout << "Of the 5x5 identity array added to a 5x5 array of fixed value 10.0:  ";// << std::endl;
	Array2D arrSymTest = Array2D::Identity(5) + Array2D(5, 5, 10.0f);

	if (arrSymTest.IsSymmetric())
		std::cout << "Yes! It's symmetric." << std::endl;
	else
		std::cout << "!!!!!!! Something is wrong" << std::endl;

	std::cout << "Of the previous array after adding 0.01f to the bottom triangle only:  ";// << std::endl;
	Array2D arrSymTest2 = arrSymTest;
	for (int i = 1; i < 5; i++)
	{
		for (int j = 0; j < i; j++)
		{
			arrSymTest2[i][j] += 0.01f;
		}
	}

	if (!arrSymTest2.IsSymmetric())
		std::cout << "Nope! It's not symmetric." << std::endl;
	else
		std::cout << "!!!!!!! Something is wrong" << std::endl;

	std::cout << "Redoing with previous test with near-symmetry check and 0.015 tolerace:  ";// << std::endl;
	if (arrSymTest2.IsSymmetric(0.015f))
		std::cout << "Yes! It's symmetric." << std::endl;
	else
		std::cout << "!!!!!!! Something is wrong" << std::endl;

	std::cout << "\n=========================================================================" << std::endl;
	std::cout << "Testing overlaying of unequal sized arrays:" << std::endl;
	Array2D arr4(4, 4);
	std::cout << "Overlaying a 2x2 identity matrix with 0, 0 offsets:" << std::endl;
	arr4.Overlay(Array2D::Identity(2), 0, 0);
	arr4.DisplayArrayInCLI();
	
	std::cout << "Overlaying arr3 (the one used to test determinant) with 0, 1 offsets:" << std::endl;
	arr4.Overlay(arr3, 0, 1);
	arr4.DisplayArrayInCLI();

	std::cout << "\n=========================================================================" << std::endl;

	for (int size = 1; size <= 1000000; size = size * 10)
	{
		std::cout << "Testing init speed between normal and fast constructors for a " << size << "x" << size << " array:" << std::endl;
		std::chrono::high_resolution_clock::time_point startTime;
		std::chrono::microseconds durSlow, durFast;
		try
		{
			startTime = std::chrono::high_resolution_clock::now();
			Array2D bigArraySlow(size, size);
			durSlow = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime);

			startTime = std::chrono::high_resolution_clock::now();
			Array2D bigArrayFast(size, size, nullptr);
			durFast = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime);
		}
		catch (std::bad_alloc &exception)
		{
			std::cout << "Could not allocate this array. Too big? "
					<< pow(static_cast<double>(size), 2.0f) * sizeof(double) / 1024.0F / 1024.0F / 1024.0F << "GB."
					<< std::endl << std::endl;
			continue;
		}

		std::cout << "Slow init time: " << durSlow.count() << " microsecond." << std::endl;
		std::cout << "Fast init time: " << durFast.count() << " microsecond." << std::endl;
		std::cout << "Delta: " << durSlow.count() - durFast.count() << " microsecond. Or: "
			<< 100.0F * static_cast<double>(durSlow.count() - durFast.count()) / static_cast<double>(durSlow.count())
			<< "% faster" << std::endl;
		std::cout << std::endl;
	}

	std::cout << "\n=========================================================================" << std::endl;
	std::cout << "Testing LU decomposition of the following array:" << std::endl;
	Array2D arr5(5, 5);
	arr5.SetValue(0, 0, 2.55f);		arr5.SetValue(0, 1, 0.2f);		arr5.SetValue(0, 2, 85.5f);		arr5.SetValue(0, 3, 1.25f);		arr5.SetValue(0, 4, 85.5f);
	arr5.SetValue(1, 0, 3.5f);		arr5.SetValue(1, 1, 3.58f);		arr5.SetValue(1, 2, 22.2f);		arr5.SetValue(1, 3, 3.5f);		arr5.SetValue(1, 4, 3.5f);
	arr5.SetValue(2, 0, 1.05f);		arr5.SetValue(2, 1, 0.05f);		arr5.SetValue(2, 2, 2.5f);	arr5.SetValue(2, 3, 3.5f);		arr5.SetValue(2, 4, 1.5f);
	arr5.SetValue(3, 0, 3.7f);		arr5.SetValue(3, 1, 55.0f);		arr5.SetValue(3, 2, 22.2f);		arr5.SetValue(3, 3, 3.5f);		arr5.SetValue(3, 4, 3.5f);
	arr5.SetValue(4, 0, 2.2f);		arr5.SetValue(4, 1, 2.0f);		arr5.SetValue(4, 2, 2.0f);		arr5.SetValue(4, 3, 3.5f);		arr5.SetValue(4, 4, 5.0f);
	arr5.DisplayArrayInCLI();

	Array2D ** decomposition = arr5.DecomposeLU();
	if (decomposition != NULL)
	{
		std::cout << "Lower decomposition" << std::endl;
		decomposition[0]->DisplayArrayInCLI();

		std::cout << "Upper decomposition" << std::endl;
		decomposition[1]->DisplayArrayInCLI();

		std::cout << "Multipliciation of the decomposition subtracted from the original (should be a matrix of zeroes, since A = L * U)" << std::endl;
		Array2D luTest = (*decomposition[0] * *decomposition[1]) - arr5;
		luTest.DisplayArrayInCLI(2);

		delete decomposition[0];
		delete decomposition[1];
		delete[] decomposition;
	}
	else
	{
		std::cout << "Could not decompose the provided matrix" << std::endl;
	}

	std::cout << "Test end\n";
#pragma endregion

	std::cout << "Press any key to continue." << std::endl;
	std::cin.sync();
	std::cin.get();

	return 0;
}