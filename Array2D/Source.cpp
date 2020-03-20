#include <iostream>
//#include <math.h>
//#include <vector>
#include "Array2D.h"

void main(int argc, char * argv)
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

	std::cout << "Test end\n";
#pragma endregion

	std::cout << "Press any key to continue." << std::endl;
	std::cin.sync();
	std::cin.get();
}
