#include "Array2D.h"

Array2D::Array2D(int _rows, int _columns)
{
	content = new double*[_rows];
	for (int i = 0; i < _rows; i++)
	{
		content[i] = new double[_columns];
	}

	columns = _columns;
	rows = _rows;
	SetEntireArrayToFixedValue(0);
}

Array2D::Array2D()
{
	rows = 0;
	columns = 0;
	content = NULL;
}

Array2D::Array2D(const Array2D & sourceArr)
{
	////std::cout << "Copy_Constructor_called @ " << this <<"from source " << &sourceArr << std::endl;
	////std::cout << " Setting rows and columns" << std::endl;
	//rows = sourceArr.Rows();
	//columns = sourceArr.Columns();
	////std::cout << "Testing source content for NULL" << std::endl;
	//if (sourceArr.content == NULL)
	//{
	//	std::cout << "WARNING! Attempting to copy an Array2D with unintialized content.\n"; //test
	//	content = NULL;
	//	return;
	//}
	////std::cout << "initializing and copying contents" << std::endl;
	////content = new double*[rows];
	////for (int i = 0; i < rows; i++)
	////	content[i] = new double[columns];
	////*content = *sourceArr.content;
	//content = new double*[rows];
	//for (int i = 0; i < rows; i++)
	//{
	//	content[i] = new double[columns];
	//	for (int j = 0; j < columns; j++)
	//	{
	//		content[i][j] = sourceArr.GetValue(i, j);
	//	}
	//}
	////std::cout << "end of Copy_Constructor @ " << this << std::endl;

	content = NULL;
	*this = sourceArr;
}

Array2D::Array2D(std::vector<std::vector<double>> & sourceVec)
{
	content = NULL;
	*this = sourceVec;
}

Array2D::~Array2D()
{
	DeleteContent();
}

Array2D Array2D::operator*(const Array2D & arr2)
{
	return MultiplyArrays(*this, arr2);
}

Array2D Array2D::operator*(const double & scalar)
{
	return MultiplayArrayWithScalar(*this, scalar);
}

Array2D Array2D::operator+(const Array2D & arr2)
{
	return AddArrays(*this, arr2, +1);
}

Array2D Array2D::operator-(const Array2D & arr2)
{
	return AddArrays(*this, arr2, -1);
}

void Array2D::operator=(const Array2D & sourceArr)
{
	//Before assigning a new conent to current instance of an object, we must first delete its current content, if it exists. The existence check is already done inside DeleteContent().
	DeleteContent();

	rows = sourceArr.Rows();
	columns = sourceArr.Columns();

	//In this implementation, we accept that we could assign an "empty" object to this one. We set content = NULL because that's how we establish this object to be empty.
	if (sourceArr.content == NULL)
	{
		std::cout << "WARNING! Attempting to copy an Array2D with unintialized content.\n"; //test
		content = NULL;
		return;
	}

	content = new double*[rows];
	for (int i = 0; i < rows; i++)
	{
		content[i] = new double[columns];

		for (int j = 0; j < columns; j++)
		{
			content[i][j] = sourceArr.GetValue(i, j);
		}
	}
}

void Array2D::operator=(std::vector<std::vector<double>>& sourceVec)
{
	//The assignment from a vector of vectors of doubles has a worst-case-scenario assumption that not all of the secondary vectors are of same length (no guarantees of that), so, we
	//create a matrix of number of rows equal to length of first vector, and of columns equal to the largest the sub-vectors, we copy values from the vector<vector> array as expected,
	//but we pad the cells in shorter rows with zeroes. E.g. for vector of 3 sub-vectors, where sub-vector1 = {1, 2, 3}, sub-vector2 = {1, 2}, and sub-vector3 = {1, 2, 3, 4}, the result
	//matrix is:
	//	1	2	3	0
	//	1	2	0	0
	//	1	2	3	4

	//Before assigning a new conent to current instance of an object, we must first delete its current content, if it exists. The existence check is already done inside DeleteContent().
	DeleteContent();

	//First, we determine the number of columns = largest row size (longest sub-vector)
	int maxRowSize = 0;

	for (std::vector<std::vector<double>>::iterator it = sourceVec.begin(); it != sourceVec.end(); ++it)
	{
		int currentRowSize = it->size();
		if (currentRowSize > maxRowSize)
			maxRowSize = currentRowSize;
	}

	rows = sourceVec.size();
	columns = maxRowSize;

	if (rows == 0 || columns == 0) //In case the Vector matrix we're working with is empty.
	{
		content = NULL;
		return;
	}

	content = new double *[rows];
	for (int i = 0; i < rows; i++)
		content[i] = new double[columns];

	SetEntireArrayToFixedValue(0.0f);

	int currentRow = 0, currentColumn = 0;

	for (std::vector<std::vector<double>>::iterator it = sourceVec.begin(); it != sourceVec.end(); ++it)
	{
		for (std::vector<double>::iterator it2 = it->begin(); it2 != it->end(); ++it2)
		{
			content[currentRow][currentColumn] = *it2;
			currentColumn++;
		}
		currentColumn = 0;
		currentRow++;
	}
}

void Array2D::operator/=(const Array2D & sourceArr)
{
	*this = InvertArray(sourceArr);
}

double & Array2D::operator()(int _row, int _column)
{
	return content[_row][_column];
}

bool Array2D::SetValue(int _row, int _column, double value)
{
	if (content == NULL							//Checking whether this object is empty. Making an assumption that initializing the first level of the content is automatically followed by init of sublevel.
		|| _row >= rows || _column >= columns)	//Checking out-of-bound writes.
		return false;
		
	content[_row][_column] = value;
	return true;
}

double Array2D::GetValue(int _row, int _column) const
{
	if (_row >= rows || _column >= columns)	//Checking out-of-bound reads
	{
		std::cout << "WARNING! Attempting to read a value out of array's bounds!" << std::endl;
		return 0.0f;	//TODO would cause logic errors, figure out a more gracefull way to do this. Exceptions? Asserts?
	}

	if (content == NULL)	//Checking reads from empty content
	{
		std::cout << "WARNING! Attempting to read a value off a non-initialized array!" << std::endl;
		return 0.0; //TODO same as above
	}

	return content[_row][_column];
}

int Array2D::Rows() const
{
	return rows;
}

int Array2D::Columns() const
{
	return columns;
}

void Array2D::SetEntireArrayToFixedValue(double value)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			content[i][j] = value;
		}
	}
}

Array2D Array2D::GetSubMatrix(unsigned int beginRow, unsigned int noOfRows, unsigned int beginColumn, unsigned int noOfColumns)
{
	if (beginRow + noOfRows > rows || beginColumn + noOfColumns > columns)
	{
		std::cout << "WARNING! Attempting to extract submatrix with a range outside the original matrix bounds" << std::endl;
		return Array2D();
	}

	Array2D subMatrix(noOfRows, noOfColumns);

	for (int i = beginRow; i < beginRow + noOfRows; i++)
	{
		for (int j = beginColumn; j < beginColumn + noOfColumns; j++)
		{
			subMatrix.SetValue(i - beginRow, j - beginColumn, content[i][j]);
		}
	}

	return subMatrix;
}

Array2D Array2D::Transpose()
{
	return TransposeArray(*this);
}

Array2D Array2D::Invert()
{
	return InvertArray(*this);
}

bool Array2D::SwapRows(unsigned int firstRow, unsigned int secondRow)
{
	//Check whether firstRow or secondRow are OOB, return fals if so.
	if (firstRow > rows || secondRow > rows)
	{
		std::cout << "ERROR! Attempting to swap rows out of array's bounds." << std::endl;
		return false;
	}

	for (int j = 0; j < columns; j++)
	{
		double tempValue = content[secondRow][j]; //backup second row value
		content[secondRow][j] = content[firstRow][j];
		content[firstRow][j] = tempValue;
	}
	return true;
}

Array2D Array2D::Unity(int dimension)
{
	Array2D uMatrix(dimension, dimension);
	
	for (int i = 0; i < dimension; i++)
		uMatrix.SetValue(i, i, 1.0f);

	return uMatrix;
}

bool Array2D::AreOfSameSize(const Array2D & arr1, const Array2D & arr2)
{
	if (arr1.Rows() != arr2.Rows() || arr1.Columns() != arr2.Columns())
	{
		throw UNEQUAL_ARRAYS;
		return false;
	}
	else
		return true;
}

bool Array2D::AreMultipliable(const Array2D & arr1, const Array2D & arr2)
{
	if (arr1.Columns() != arr2.Rows())
	{
		throw UNMULT_ARRAYS;
		return false;
	}
	else
		return true;
}

bool Array2D::IsInvertible(Array2D arr)
{
	//TODO implement singular/degenerate array checks here

	if (arr.Rows() != arr.Columns())
	{
		throw NONINVERT_ARRAY;
		return false;
	}
	else
		return true;
}

bool Array2D::AreJoinable(const Array2D & arr1, const Array2D & arr2, bool testHorizontally)
{
	if (testHorizontally) //testing for horizontal merging, row count must be equall.
	{
		if (arr1.Rows() == arr2.Rows())
			return true;
		else
		{
			throw UNMERGABLE_ARRAYS;
			return false;
		}
	}
	else //testing for vertical merging, column count must be equall.
	{
		if (arr1.Columns() == arr2.Columns())
			return true;
		else
		{
			throw UNMERGABLE_ARRAYS;
			return false;
		}
	}
}

Array2D Array2D::MergeArrays(const Array2D & arr1, const Array2D & arr2)
{
	try
	{
		AreJoinable(arr1, arr2, true);
	}
	catch (int exceptionInt)
	{
		std::cout << "Caught Exception: " << exceptionInt << std::endl;
		if (exceptionInt == UNMERGABLE_ARRAYS)
		{
			std::cout << "ERROR! Attempting to merge array of different heights" << std::endl;
			return Array2D();
		}
	}

	Array2D result(arr1.Rows(), arr1.Columns() + arr2.Columns());

	for (int i = 0; i < result.Rows(); i++)
	{
		for (int j = 0; j < arr1.Columns(); j++) //loop over the western (left) half of the array.
			result.SetValue(i, j, arr1.GetValue(i, j));

		for (int j = arr1.Columns(); j < arr1.Columns() + arr2.Columns(); j++) //loop over the easter (right) half of the array.
			result.SetValue(i, j, arr2.GetValue(i, j - arr1.Columns()));
	}

	return result;
}

void Array2D::DisplayArrayInCLI(int displayPrecision)
{
	if (content == NULL)
	{
		std::cout << "WARNING! Array content not initialized.\nCannnot display array content." << std::endl;
		return;
	}

	std::cout << "- - - - - - - - - - - - - -" << std::endl;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			std::cout << std::fixed << std::setw(displayPrecision + 4) << std::setprecision(displayPrecision) << content[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "- - - - - - - - - - - - - -" << std::endl;
}

Array2D Array2D::MultiplyArrays(const Array2D & arr1, const Array2D & arr2)
{
	try
	{
		AreMultipliable(arr1, arr2);
	}
	catch (int exceptionInt)
	{
		std::cout << "Caught Exception: " << exceptionInt << std::endl;
		if (exceptionInt == UNMULT_ARRAYS)
		{
			std::cout << "ERROR! Attempting to multiply array of " << arr1.Columns() << "columns with an array of " << arr2.Rows() << " rows." << std::endl;
			return Array2D();  //really need to figure out how to make this thing more gracefull.
		}
	}

	Array2D result(arr1.Rows(), arr2.Columns());

	for (int i = 0; i < arr1.Rows(); i++)
	{
		for (int j = 0; j < arr2.Columns(); j++)
		{
			double cellValue = 0.0f;

			for (int k = 0; k < arr1.Columns(); k++)
				cellValue += arr1.GetValue(i, k) * arr2.GetValue(k, j);

			result.SetValue(i, j, cellValue);
		}
	}

	return result;

}

Array2D Array2D::MultiplayArrayWithScalar(const Array2D & arr1, const double scalar)
{
	Array2D result = arr1;

	for (int i = 0; i < result.Rows(); i++)
	{
		for (int j = 0; j < result.Columns(); j++)
		{
			result.SetValue(i, j, result.GetValue(i, j) * scalar);
		}
	}

	return result;
}

Array2D Array2D::AddArrays(const Array2D & arr1, const Array2D & arr2, int opSign)
{
	//TODO consider dividing opSign by the square of opSign, so the opSign is always either +1 or -1. Caveats: extra line for something that shouldn't be exposed outside of object to begin with.

	try 
	{
		AreOfSameSize(arr1, arr2);
	}
	catch (int exceptionInt)
	{
		std::cout << "Caught Exception: " << exceptionInt << std::endl;
		if (exceptionInt == UNEQUAL_ARRAYS)
		{
			std::cout << "ERROR! Attempting to add arrays of different sizes." << std::endl;
			return Array2D();  
		}
	}
	
	Array2D result(arr1.Rows(), arr1.Columns());

	for (int i = 0; i < arr1.Rows(); i++)
	{
		for (int j = 0; j < arr1.Columns(); j++)
		{
			double value = arr1.GetValue(i, j) + opSign * arr2.GetValue(i, j);
			result.SetValue(i, j, value);
		}
	}
	
	return result;
}

Array2D Array2D::InvertArray(const Array2D & sourceArr, MatrixInversionMethod method)
{
	try
	{
		IsInvertible(sourceArr);
	}
	catch (int exceptionInt)
	{
		std::cout << "Caught Exception: " << exceptionInt << std::endl;
		if (exceptionInt == NONINVERT_ARRAY)
		{
			std::cout << "ERROR! Attempting to invert a non-invertible array." << std::endl;
			return Array2D();  
		}
	}

	switch (method)
	{
	case Gauss_Jordan:
		return GausJordanElimination(sourceArr);
		break; //TODO Is this break necessary?
	default:
		break;
	}

	return Array2D();
}

Array2D Array2D::TransposeArray(const Array2D & sourceArr)
{
	if (sourceArr.content == NULL)
	{
		std::cout << "ERROR! Attempting to transpose a non-initialized Array2D" <<std::endl;
		return Array2D();
	}

	Array2D result (sourceArr.Columns(), sourceArr.Rows());

	for (int i = 0; i < result.Rows(); i++)
	{
		for (int j = 0; j < result.Columns(); j++)
		{
			result.SetValue(i, j, sourceArr.GetValue(j, i));
		}
	}

	return result;
}

Array2D Array2D::GausJordanElimination(const Array2D & sourceArr)
{
	Array2D result(sourceArr.Rows(), sourceArr.Columns());
	Array2D augmentedArr = MergeArrays(sourceArr, Unity(sourceArr.Rows())); //augmentedArr is the augment matrix, which is the original matrix with a unity array attached to its right.

	//TODO Swapping rows when the first pivot value is zero

	//For an array of n*n size, n steps are needed to get the inverse.
	for (int step = 0; step < result.Rows(); step++) //the variable "step" here will be our pivot row, and by virtue of being a squared array, our pivot column as well.
	{
		double pivotValue = augmentedArr.GetValue(step, step);  //the value of th pivot cell is always on the diagonal.
		
		//mFactor is the value that, when multiplied with our pivot row and substracted from current row, should help reduce it towards zero (except diagonal value)
		//we extract mFactors before the loops because, their cell values will change inside the loops at first iterations, but we'll still be needing them for remaining iterations.
		double * mFactors = new double[result.Rows()]; 
		for (int i = 0; i < augmentedArr.Rows(); i++)
			mFactors[i] = augmentedArr.GetValue(i, step);

		for (int j = 0; j < augmentedArr.Columns(); j++)
		{
			double newColumnValueAtPivotRow = augmentedArr.GetValue(step, j) / pivotValue;
			augmentedArr.SetValue(step, j, newColumnValueAtPivotRow); //the pivot row value is adjusted before iterating over other rows.

			for (int i = 0; i < augmentedArr.Rows(); i++) //we then iterate over other rows, using the if-statement bellow to avoid modifying our pivot row.
			{
				if (i != step)
				{
					double newValueAtOtherRow = augmentedArr.GetValue(i, j) - (mFactors[i] * newColumnValueAtPivotRow);
					augmentedArr.SetValue(i, j, newValueAtOtherRow);
				}
			}
		}
		delete mFactors;
	}

	//extract result from augmentedArr
	result = augmentedArr.GetSubMatrix(0, result.Rows(), sourceArr.Columns(), result.Columns());

	return result;
}

void Array2D::DeleteContent()
{
	if (content != NULL)
	{
		for (int i = 0; i < rows; i++)
		{
			if (content[i] != NULL)
			{
				delete content[i];
			}
		}
		delete[] content;
	}
	columns = 0;
	rows = 0;
}