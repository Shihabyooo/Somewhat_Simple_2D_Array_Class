#include "Array2D.h"

Array2D::Array2D()
{
	rows = 0;
	columns = 0;
	content = NULL;
}

Array2D::Array2D(size_t _rows, size_t _columns)
{
	Alloc(_rows, _columns);

	columns = _columns;
	rows = _rows;
	SetEntireArrayToFixedValue(0.0f);
}

Array2D::Array2D(size_t _rows, size_t _columns, std::nullptr_t flag)
{
	Alloc(_rows, _columns);

	columns = _columns;
	rows = _rows;
}

Array2D::Array2D(size_t _rows, size_t _columns, double defaultValue)
{
	//Array2D(_rows, _columns);
	Alloc(_rows, _columns);

	columns = _columns;
	rows = _rows;

	SetEntireArrayToFixedValue(defaultValue);
}

Array2D::Array2D(const Array2D & sourceArr)
{
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
		content = NULL;
		return;
	}

	Alloc(rows, columns);

	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < columns; j++)
			content[i][j] = sourceArr.GetValue(i, j);
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
	size_t maxRowSize = 0;

	for (std::vector<std::vector<double>>::iterator it = sourceVec.begin(); it != sourceVec.end(); ++it)
	{
		size_t currentRowSize = it->size();
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

	Alloc(rows, columns);

	size_t currentRow = 0, currentColumn = 0;

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

bool Array2D::operator==(const Array2D arr2)
{
	if (rows != arr2.Rows() || columns != arr2.Columns())
		return false;

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			if (content[i][j] != arr2.GetValue(i, j))
				return false;
		}
	}
}

bool Array2D::operator!=(const Array2D arr2)
{
	return !(*this == arr2);
}

void Array2D::operator/=(const Array2D & sourceArr)
{
	*this = InvertArray(sourceArr);
}

double & Array2D::operator()(size_t _row, size_t _column)
{
	return content[_row][_column];
}

void Array2D::SetValue(size_t _row, size_t _column, double value)
{
	if (content == NULL							//Checking whether this object is empty. Making an assumption that initializing the first level of the content is automatically followed by init of sublevel.
		|| _row >= rows || _column >= columns)	//Checking out-of-bound writes.
		throw std::out_of_range("ERROR! Column or Row value out of range or content set to NULL");

	content[_row][_column] = value;
}

double Array2D::GetValue(size_t _row, size_t _column) const
{
	if (content == NULL							//Checking whether this object is empty. Making an assumption that initializing the first level of the content is automatically followed by init of sublevel.
		|| _row >= rows || _column >= columns)	//Checking out-of-bound writes.
		throw std::out_of_range("ERROR! Column or Row value out of range or content set to NULL");

	return content[_row][_column];
}

size_t Array2D::Rows() const
{
	return rows;
}

size_t Array2D::Columns() const
{
	return columns;
}

void Array2D::SetEntireArrayToFixedValue(double value)
{
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			content[i][j] = value;
		}
	}
}

Array2D Array2D::GetSubMatrix(size_t beginRow, size_t noOfRows, size_t beginColumn, size_t noOfColumns)
{
	
	if (beginRow + noOfRows > rows || beginColumn + noOfColumns > columns)  //TODO consider rearranging the arguments for this method to have the pivot coord
																			//first then lengths.
	{
		std::cout << "ERROR! Attempting to extract submatrix with a range outside the original matrix bounds" << std::endl;
		return Array2D();
	}
	if (noOfRows < 1 || noOfColumns < 1)
	{
		std::cout << "ERROR! Attempting to extract submatrix with null size in one both dimensions." << std::endl;
		return Array2D();
	}

	Array2D subMatrix(noOfRows, noOfColumns);

	for (size_t i = beginRow; i < beginRow + noOfRows; i++)
	{
		for (size_t j = beginColumn; j < beginColumn + noOfColumns; j++)
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

double Array2D::Determinant()
{
	return CalculateDeterminant(*this);
}

void Array2D::SwapRows(size_t firstRow, size_t secondRow)
{
	//Check whether firstRow or secondRow are OOB, return false if so.
	if (firstRow > rows || secondRow > rows)
	{
		//std::cout << "ERROR! Attempting to swap rows out of array's bounds." << std::endl;
		throw std::out_of_range("ERROR! Attempting to swap columns outside the range of the array");
	}

	for (size_t j = 0; j < columns; j++)
	{
		double tempValue = content[secondRow][j]; //backup second row value
		content[secondRow][j] = content[firstRow][j];
		content[firstRow][j] = tempValue;
	}

}

void Array2D::Overlay(const Array2D & arr2, size_t rowOffset, size_t columnOffset)
{
	if (content == NULL) //if this Array2D is uninitialized, this method acts as a simple assignment.
	{
		*this = arr2;
	}

	size_t _rows = rows > arr2.Rows() + rowOffset ? arr2.Rows() + rowOffset : rows;
	size_t _columns = columns > arr2.Columns() + columnOffset ? arr2.Columns() + columnOffset : columns;

	for (size_t i = rowOffset; i < _rows; i++)
	{
		for (size_t j = columnOffset; j < _columns; j++)
			content[i][j] += arr2.GetValue(i - rowOffset, j - columnOffset);
	}
}

Array2D ** Array2D::DecomposeLU()
{
	return DecomposeLU(*this);
}

Array2D ** Array2D::DecomposeLUP()
{
	return DecomposeLUP(*this);
}

bool Array2D::NearlyEquals(const Array2D & arr, float tolerance)
{
	return Array2D::AreNearlyEquall(*this, arr, tolerance);
}

bool Array2D::IsSymmetric()
{
	return IsSymmetric(*this);
}

bool Array2D::IsSymmetric(float tolerance)
{
	return IsSymmetric(*this, tolerance);
}

Array2D Array2D::Identity(size_t dimension)
{
	Array2D uMatrix(dimension, dimension);
	
	for (size_t i = 0; i < dimension; i++)
		uMatrix.SetValue(i, i, 1.0f);

	return uMatrix;
}

bool Array2D::AreOfSameSize(const Array2D & arr1, const Array2D & arr2)
{
	if (arr1.Rows() != arr2.Rows() || arr1.Columns() != arr2.Columns())
		return false;
	else
		return true;
}

bool Array2D::AreMultipliable(const Array2D & arr1, const Array2D & arr2)
{
	if (arr1.Columns() != arr2.Rows())
		return false;
	else
		return true;
}

bool Array2D::IsSquared(const Array2D & arr1)
{
	if (arr1.Columns() != arr1.Rows())
		return false;
	else
		return true;
}

bool Array2D::IsInvertible(Array2D arr, bool checkSingular)
{
	if (arr.Rows() != arr.Columns())
		return false;
	else if (checkSingular && abs(arr.Determinant()) <= MINDET) //Making use of the optimization where determinant computation wil be skipped if checkSingular is false
																//TODO check whether this is a standard and always the case, or is compiler/language version specific.
		return false;
	else
		return true;
}

bool Array2D::IsSymmetric(const Array2D & arr)
{
	if (!IsSquared(arr))
		return false;

	for (size_t i = 0; i < arr.Rows(); i++)
	{
		for (size_t j = 0; j < arr.Rows(); j++)
		{
			if (arr.GetValue(i, j) != arr.GetValue(j, i))
				return false;
		}
	}

	return true;
}

bool Array2D::IsSymmetric(const Array2D & arr, float tolerance)
{
	if (!IsSquared(arr))
		return false;
	
	tolerance = abs(tolerance); //in case it was sent as a negative value, in-which case all matrices would turn out to be symmetric.

	for (size_t i = 0; i < arr.Rows(); i++)
	{
		for (size_t j = 0; j < arr.Rows(); j++)
		{
			if (abs(arr.GetValue(i, j) - arr.GetValue(j, i)) >= tolerance)
				return false;
		}
	}

	return true;
}

bool Array2D::AreJoinable(const Array2D & arr1, const Array2D & arr2, bool testHorizontally)
{
	if (testHorizontally) //testing for horizontal merging, row count must be equall.
	{
		if (arr1.Rows() == arr2.Rows())
			return true;
		else
			return false;
		
	}
	else //testing for vertical merging, column count must be equall.
	{
		if (arr1.Columns() == arr2.Columns())
			return true;
		else
			return false;
	}
}

bool Array2D::AreNearlyEquall(const Array2D & arr1, const Array2D & arr2, float tolerance)
{
	tolerance = abs(tolerance); //in case it was sent as a negative value, in-which case all matrices would turn out to be equall.
	if (arr1.Rows() != arr2.Rows() || arr1.Columns() != arr2.Columns())
		return false;

	for (size_t i = 0; i < arr1.Rows(); i++)
	{
		for (size_t j = 0; j < arr1.Rows(); j++)
		{
			if (abs(arr1.GetValue(i, j) - arr2.GetValue(i, j)) >= tolerance)
				return false;
		}
	}

	return true;
}

Array2D Array2D::MergeArrays(const Array2D & arr1, const Array2D & arr2)
{
	if (!AreJoinable(arr1, arr2, true))
	{
		std::cout << "ERROR! Attempting to merge array of different heights" << std::endl;
		return Array2D();
	}

	Array2D result(arr1.Rows(), arr1.Columns() + arr2.Columns());

	for (size_t i = 0; i < result.Rows(); i++)
	{
		for (size_t j = 0; j < arr1.Columns(); j++) //loop over the western (left) half of the array.
			result.SetValue(i, j, arr1.GetValue(i, j));

		for (size_t j = arr1.Columns(); j < arr1.Columns() + arr2.Columns(); j++) //loop over the easter (right) half of the array.
			result.SetValue(i, j, arr2.GetValue(i, j - arr1.Columns()));
	}

	return result;
}

Array2D ** Array2D::DecomposeLU(const Array2D & arr)
{
	//Based on the algorithm detailed on Introduction to Algorithms (3rd ed), Cormen, T., Lieserson, C., Rivest, R., and Stein, C.

	if (!IsSquared(arr))
	{
		std::cout << "ERROR! Cannot decompose a non-squared matrix." << std::endl;
		return NULL;
	}

	Array2D ** decomposition = new Array2D * [2]; //first is lower, second is upper.

	Array2D * lower = decomposition[0] = new Array2D(arr.Rows(), arr.Rows());
	Array2D * upper = decomposition[1] = new Array2D(arr.Rows(), arr.Rows());
	
	//we need a temporary matrix initially holding the same content of original matrix for the computations bellow
	Array2D * tempMatrix = new Array2D(arr);

	for (int i = 0; i < arr.Rows(); i++)
	{
		//upper diagonal is same for original matrix
		upper->SetValue(i, i, tempMatrix->GetValue(i, i));

		//lower diagonal = 1
		lower->SetValue(i, i, 1.0f);

		for (int j = i + 1; j < arr.Rows(); j++)
		{
			//lower is divided by upper's pivot
			lower->SetValue(j, i, tempMatrix->GetValue(j, i) / upper->GetValue(i, i));

			//upper triangle (above diagonal) is the Schur compliment
			upper->SetValue(i, j, tempMatrix->GetValue(i, j));
		}

		//compute the Schur complement in-place in the temporary matrix
		for (int k = i + 1; k < arr.Rows(); k++)
		{
			for (int l = i + 1; l < arr.Rows(); l++)
			{
				tempMatrix->SetValue(k, l, tempMatrix->GetValue(k, l) - lower->GetValue(k, i) * upper->GetValue(i, l));
			}
		}
	}

	//cleanup and return
	delete tempMatrix;

	return decomposition;
}

Array2D ** Array2D::DecomposeLUP(const Array2D & arr) //TODO finish implementing this
{
	//Based on the algorithm detailed on Introduction to Algorithms (3rd ed), Cormen, T., Lieserson, C., Rivest, R., and Stein, C.

	if (!IsSquared(arr))
	{
		std::cout << "ERROR! Cannot decompose a non-squared matrix." << std::endl;
		return NULL;
	}

	Array2D ** decomposition = new Array2D * [3]; //first is lower, second is upper, third is permutation

	return decomposition;
}

void Array2D::DisplayArrayInCLI(int displayPrecision)
{
	if (content == NULL)
	{
		std::cout << "WARNING! Array content not initialized.\nCannnot display array content." << std::endl;
		return;
	}

	std::cout << "- - - - - - - - - - - - - -" << std::endl;
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			std::cout << std::fixed << std::setw(displayPrecision + 4) << std::setprecision(displayPrecision) << content[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << "- - - - - - - - - - - - - -" << std::endl;
}

Array2D Array2D::MultiplyArrays(const Array2D & arr1, const Array2D & arr2)
{
	if (!AreMultipliable(arr1, arr2))
	{
		std::cout << "ERROR! Attempting to multiply array of " << arr1.Columns() << "columns with an array of " << arr2.Rows() << " rows." << std::endl;
		return Array2D();  //really need to figure out how to make this thing more gracefull.
	}
	
	Array2D result(arr1.Rows(), arr2.Columns());

	for (size_t i = 0; i < arr1.Rows(); i++)
	{
		for (size_t j = 0; j < arr2.Columns(); j++)
		{
			double cellValue = 0.0f;

			for (size_t k = 0; k < arr1.Columns(); k++)
				cellValue += arr1.GetValue(i, k) * arr2.GetValue(k, j);

			result.SetValue(i, j, cellValue);
		}
	}

	return result;

}

Array2D Array2D::MultiplayArrayWithScalar(const Array2D & arr1, const double scalar)
{
	Array2D result = arr1;

	for (size_t i = 0; i < result.Rows(); i++)
	{
		for (size_t j = 0; j < result.Columns(); j++)
		{
			result.SetValue(i, j, result.GetValue(i, j) * scalar);
		}
	}

	return result;
}

Array2D Array2D::AddArrays(const Array2D & arr1, const Array2D & arr2, int opSign)
{
	//TODO consider dividing opSign by the square of opSign, so the opSign is always either +1 or -1. Caveats: extra line for something that shouldn't be exposed outside of object to begin with.

	if (!AreOfSameSize(arr1, arr2))
	{
		std::cout << "ERROR! Attempting to add arrays of different sizes." << std::endl;
		return Array2D();
	}
	
	Array2D result(arr1.Rows(), arr1.Columns());

	for (size_t i = 0; i < arr1.Rows(); i++)
	{
		for (size_t j = 0; j < arr1.Columns(); j++)
		{
			double value = arr1.GetValue(i, j) + opSign * arr2.GetValue(i, j);
			result.SetValue(i, j, value);
		}
	}
	
	return result;
}

Array2D Array2D::InvertArray(const Array2D & sourceArr, MatrixInversionMethod method)
{
	if (!IsInvertible(sourceArr))
	{
		std::cout << "ERROR! Attempting to invert a non-invertible array." << std::endl;
		return Array2D();
	}
	
	switch (method)
	{
	case MatrixInversionMethod::Gauss_Jordan:
		return GausJordanElimination(sourceArr);
	default:
		std::cout << "ERROR! Received unsupported MatrixInversionMethod in InverArray()." << std::endl; //shouldn't happen (but still...)
		return Array2D();
	}
}

Array2D Array2D::TransposeArray(const Array2D & sourceArr)
{
	if (sourceArr.content == NULL)
	{
		std::cout << "ERROR! Attempting to transpose a non-initialized Array2D" <<std::endl;
		return Array2D();
	}

	Array2D result (sourceArr.Columns(), sourceArr.Rows());

	for (size_t i = 0; i < result.Rows(); i++)
	{
		for (size_t j = 0; j < result.Columns(); j++)
		{
			result.SetValue(i, j, sourceArr.GetValue(j, i));
		}
	}

	return result;
}

double Array2D::CalculateDeterminant(const Array2D & sourceArr) 
{
	//This recurive method calculate the determinant for a matrix of arbitrary dimensions. If the recieved matrix is less than 2x2, directly return the determinant, else will make use of
	//the method GetMinorSubMatrix() and work recuresively untill reaching the 2x2 matrices. 
	double result = 0.0f;

	if (!IsSquared(sourceArr))
	{
		std::cout << "ERROR! Attempting to calculate the determinant of a non-squared matrix." << std::endl;
		return result;  //really need to figure out how to make this thing more gracefull.
	}
	
	if (sourceArr.Rows() > 2)
	{
		for (size_t i = 0; i < sourceArr.Rows(); i++)
		{
			result += pow(-1.0f, i) * sourceArr.GetValue(0, i) * CalculateDeterminant(GetMinorSubMatrix(sourceArr, 0, i));	//this is where the recurssion happens. The pow(-1.0f, i) term is used
		}																													//to flip the sign of the addition based on which column we're at.
	}
	else
	{
		result = (sourceArr.GetValue(0, 0) * sourceArr.GetValue(1, 1)) - (sourceArr.GetValue(1, 0) * sourceArr.GetValue(0, 1));
	}

	return result;
}

Array2D Array2D::GausJordanElimination(const Array2D & sourceArr)
{
	Array2D result(sourceArr.Rows(), sourceArr.Columns());
	Array2D augmentedArr = MergeArrays(sourceArr, Identity(sourceArr.Rows())); //augmentedArr is the augment matrix, which is the original matrix with a identity matrix attached to its right.

	//If first pivot value is zero, must swap the row with another that has a non-zero value. If none exist, can't use this method.
	if (augmentedArr.GetValue(0, 0) == 0.0f)
	{
		//look for a row where first element is not zero
		for (int i = 0; i < augmentedArr.Rows(); i++)
		{
			if (augmentedArr.GetValue(i, 0) != 0.0f)
			{
				augmentedArr.SwapRows(0, i);
				break;
			}
			if (i == augmentedArr.Rows())
			{
				std::cout << "ERROR! Could not find a suitable pivot value for first row. Can't invert useing Gauss-Jordan Elimmination" << std::endl;
				return Array2D();
			}
		}
	}

	//For an array of n*n size, n steps are needed to get the inverse.
	for (size_t step = 0; step < result.Rows(); step++) //the variable "step" here will be our pivot row, and by virtue of being a squared array, our pivot column as well.
	{
		double pivotValue = augmentedArr.GetValue(step, step);  //the value of th pivot cell is always on the diagonal.
		
		//mFactor is the value that, when multiplied with our pivot row and substracted from current row, should help reduce it towards zero (except diagonal value)
		//we extract mFactors before the loops because, their cell values will change inside the loops at first iterations, but we'll still be needing them for remaining iterations.
		double * mFactors = new double[result.Rows()]; 
		for (size_t i = 0; i < augmentedArr.Rows(); i++)
			mFactors[i] = augmentedArr.GetValue(i, step);

		for (size_t j = 0; j < augmentedArr.Columns(); j++)
		{
			double newColumnValueAtPivotRow = augmentedArr.GetValue(step, j) / pivotValue;
			augmentedArr.SetValue(step, j, newColumnValueAtPivotRow); //the pivot row value is adjusted before iterating over other rows.

			for (size_t i = 0; i < augmentedArr.Rows(); i++) //we then iterate over other rows, using the if-statement bellow to avoid modifying our pivot row.
			{
				if (i != step)
				{
					double newValueAtOtherRow = augmentedArr.GetValue(i, j) - (mFactors[i] * newColumnValueAtPivotRow);
					augmentedArr.SetValue(i, j, newValueAtOtherRow);
				}
				//augmentedArr.DisplayArrayInCLI();
			}
		}
		delete[] mFactors;
	}

	//extract result from augmentedArr
	result = augmentedArr.GetSubMatrix(0, result.Rows(), sourceArr.Columns(), result.Columns());

	return result;
}

void Array2D::AllocateMemory(size_t _rows, size_t _columns)
{
	try
	{
		content = new double*[_rows];
		double * helperPtr = new double[_rows * _columns];

		for (size_t i = 0; i < _rows; i++)
		{
			content[i] = helperPtr;
			helperPtr += _columns;
		}

	}
	catch (const std::bad_alloc& exception)
	{
		std::cout << "ERROR! Could not allocate array. " << exception.what() << std::endl;
		throw exception;
	}
}

Array2D Array2D::GetMinorSubMatrix(const Array2D & sourceArr, size_t _row, size_t _column)
{
	//The ij-minor sub-matrix is obtained by deleting the ith row and jth column of a matrix.

	Array2D result(sourceArr.Rows() - 1, sourceArr.Columns() - 1);

	for (size_t i = 0; i < result.Rows(); i++) //this loop iterates on the smaller, sub matrix. We maintain seperate indeces inside for use when reading from source matrix.
	{
		size_t rowInSourceArr = i;
		if (i >= _row) //This means we've reached the row we want to ommit, so now we read the next row.
			rowInSourceArr += 1;

		for (size_t j = 0; j < result.Columns(); j++)
		{
			size_t columnInSourceArr = j;
			if (j >= _column) //This means we've reached the column we want to ommit, so now we read the next column.
				columnInSourceArr += 1;

			result.SetValue(i, j, sourceArr.GetValue(rowInSourceArr, columnInSourceArr));
		}
	}

	return result;
}

void Array2D::DeleteContent()
{
	if (content != NULL)
	{
		/*for (size_t i = 0; i < rows; i++)
		{
			if (content[i] != NULL)
			{
				delete content[i];
			}
		}*/
		if (content[0] != NULL)
			delete[] content[0];

		delete[] content;
		content = NULL;
	}
	columns = 0;
	rows = 0;
}
