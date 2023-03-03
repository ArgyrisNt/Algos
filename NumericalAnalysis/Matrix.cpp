#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

namespace NumericalAnalysis
{

template <typename T>
double norm(std::vector<T> &v)
{
	double result = 0;
	for (size_t i = 0; i < v.size(); i++)
	{
		result += v[i] * v[i];
	}
	result = sqrt(result);

	return result;
}

template <class T>
Matrix<T>::Matrix()
{
	rows = 0;
	cols = 0;
	detVal = 0;
	mat = new T* [rows];
	for (int i = 0; i < rows; i++)
	{
		mat[i] = new T[cols];
	}
}

template <class T>
Matrix<T>::Matrix(int my_rows, int my_cols, T val)
{
	rows = my_rows;
	cols = my_cols;
	detVal = 0;
	mat = new T* [rows];
	for (int i = 0; i < rows; i++)
	{
		mat[i] = new T[cols];
	}
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			mat[i][j] = val;
		}
	}
}

template <class T>
Matrix<T>::Matrix(const Matrix &m)
{
	rows = m.rows;
	cols = m.cols;
	detVal = m.detVal;
	mat = m.mat;
}

template <class T>
Matrix<T>::~Matrix() {}

template <class T>
Matrix<T> &Matrix<T>::operator=(Matrix m)
{
	rows = m.rows;
	cols = m.cols;
	detVal = m.detVal;
	mat = m.mat;

	return *this;
}

template <class T>
Matrix<T> Matrix<T>::operator+(Matrix m)
{
	assert(m.rows == rows);
	assert(m.cols == cols);
	Matrix res(rows, cols);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			res.setValue(i, j, mat[i][j] + m.mat[i][j]);
		}
	}

	return res;
}

template <class T>
Matrix<T> Matrix<T>::operator-(Matrix &m)
{
	assert(m.rows == rows);
	assert(m.cols == cols);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			mat[i][j] -= m.mat[i][j];
		}
	}
	return *this;
}

template <class T>
Matrix<T> Matrix<T>::operator*(Matrix &m)
{
	assert(m.rows == cols);
	Matrix n(rows, m.cols);
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < m.cols; ++j)
		{
			n.setValue(i, j, 0.0);
		}
	}
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < m.cols; ++j)
		{
			for (int k = 0; k < cols; ++k)
			{
				n.mat[i][j] += mat[i][k] * m.mat[k][j];
			}
		}
	}
	return n;
}

template <class T>
Matrix<T> Matrix<T>::operator*(const T &c)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			mat[i][j] = mat[i][j] * c;
		}
	}

	return *this;
}

template <class T>
std::vector<T> Matrix<T>::operator*(std::vector<T> &vec)
{
	assert(vec.size() == cols);
	std::vector<T> res(rows, 0.0);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			res[i] += mat[i][j] * vec[j];
		}
	}

	return res;
}

template <class T>
T Matrix<T>::operator()(int row, int col)
{
	assert(row < rows);
	assert(col < cols);
	return mat[row][col];
}

template <class T>
void Matrix<T>::setValue(int row, int col, T val)
{
	assert(row < rows);
	assert(col < cols);
	mat[row][col] = val;
}

template <class T>
void Matrix<T>::calcDet()
{
	switch (rows)
	{
	case 1:
		detVal = mat[0][0];
		break;
	case 2:
		detVal = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
		break;
	default:
		throw std::invalid_argument("Invalid dimensions");
		break;
	}
}

template <class T>
void Matrix<T>::print()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			std::cout << mat[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

template <class T>
Matrix<T> Matrix<T>::transpose()
{
	Matrix result(rows, cols);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			result.mat[i][j] = mat[j][i];
		}
	}
	result.detVal = detVal;

	return result;
}

template <class T>
Matrix<T> Matrix<T>::inverse()
{
	Matrix result(rows, cols);
	switch (rows)
	{
	case 1:
		result.mat[0][0] = 1.0 / mat[0][0];
		break;
	case 2:
		calcDet();
		if (!detVal)
		{
			throw std::invalid_argument("Non invertible matrix");
			break;
		}
		result.detVal = 1 / detVal;
		result.mat[0][0] = result.detVal * mat[1][1];
		result.mat[1][1] = result.detVal * mat[0][0];
		result.mat[0][1] = -result.detVal * mat[0][1];
		result.mat[1][0] = -result.detVal * mat[1][0];
		break;
	default:
		throw std::invalid_argument("Invalid dimensions");
		break;
	}

	return result;
}

template <class T>
std::vector<T> Matrix<T>::Jacobi_iterator(std::vector<T> b, int iters)
{
	assert(rows == b.size());
	assert(cols == b.size());
	std::cout << "\nSolving with Jacobi iterative method. . ." << "\n";
	size_t n = rows;
	std::vector<T> x(n), r(n), sec(n);
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = 0.0;
	}
	for (int iter = 0; iter <= iters; iter++)
	{
		std::vector<T> c(n);
		for (size_t i = 0; i < n; i++)
		{
			c[i] = b[i];
			for (size_t j = 0; j < n; j++)
			{
				if (j != i)	c[i] -= mat[i][j] * x[j];
			}
		}
		for (size_t i = 0; i < n; i++)
		{
			x[i] = c[i] / mat[i][i];
		}
		sec = (*this) * x;
		for (size_t k = 0; k < x.size(); k++)
		{
			r[k] = b[k] - sec[k];
		}
		if (norm(r) < 1e-7)
		{
			std::cout << iter << " iterations\n";
			break;
		}
	}
	return x;
}

template <class T>
std::vector<T> Matrix<T>::GaussSeidel_iterator(std::vector<T> b, int iters)
{
	assert(rows == b.size());
	assert(cols == b.size());
	std::cout << "\nSolving with Gauss Seidel iterative method. . ." << "\n";
	size_t n = rows;
	std::vector<T> x(n), y(n), r(n), sec(n);
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = 0.0;
	}
	for (int iter = 0; iter <= iters; iter++)
	{
		for (size_t i = 0; i < n; i++)
		{
			y[i] = b[i] / mat[i][i];
			for (size_t j = 0; j < n; j++)
			{
				if (j == i)
				{
					continue;
				}
				y[i] -= (mat[i][j] / mat[i][i]) * x[j];
				x[i] = y[i];
			}
		}
		sec = (*this) * x;
		for (size_t k = 0; k < x.size(); k++)
		{
			r[k] = b[k] - sec[k];
		}
		if (norm(r) < 1e-7)
		{
			std::cout << iter << " iterations\n";
			break;
		}
	}
	return x;
}

template <class T>
std::vector<T> Matrix<T>::SOR_iterator(std::vector<T> b, int iters, double omega)
{
	assert(rows == b.size());
	assert(cols == b.size());
	std::cout << "\nSolving with SOR iterative method. . ." << "\n";
	size_t n = rows;
	std::vector<T> x(n), y(n), r(n), sec(n);
	for (size_t i = 0; i < x.size(); i++)
	{
		if ((i == 0) || (i == x.size() - 1))
		{
			x[i] = 0.5;
		}
		else
		{
			x[i] = 0.25;
		}
	}
	for (int iter = 0; iter <= iters; iter++)
	{
		for (size_t i = 0; i < n; i++)
		{
			double sigma = 0;
			for (size_t j = 0; j < n; j++)
			{
				if (j != i)
				{
					sigma += mat[i][j] * x[j];
				}
			}
			x[i] = (1 - omega) * x[i] + (omega / mat[i][i]) * (b[i] - sigma);
		}
		sec = (*this) * x;
		for (size_t k = 0; k < x.size(); k++)
		{
			r[k] = b[k] - sec[k];
		}
		if (norm(r) < 1e-7)
		{
			std::cout << iter << " iterations\n";
			break;
		}
	}

	return x;
}

template <class T>
std::vector<T> Matrix<T>::gradient_iterator(std::vector<T> b, int iters)
{
	std::cout << "\nSolving with Gradient iterative method. . ." << "\n";
	size_t n = rows;
	std::vector<T> x(n), mult(n), r(n), denominator1(n);
	double nominator = 0.0;
	double denominator = 0.0;
	double a = 0.0;
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = 0;
	}
	for (int iter = 0; iter < iters; iter++)
	{
		mult = (*this) * x;
		for (size_t i = 0; i < x.size(); i++)
		{
			r[i] = b[i] - mult[i];
		}
		denominator1 = (*this) * r;
		for (size_t i = 0; i < x.size(); i++)
		{
			nominator += r[i] * r[i];
			denominator += r[i] * denominator1[i];
		}		
		a = nominator / denominator;
		for (size_t i = 0; i < x.size(); i++)
		{
			x[i] = x[i] + a * r[i];
		}
		if (norm(r) < 1e-7)
		{
			std::cout << iter << " iterations\n";
			break;
		}
	}

	return x;
}

template <typename T>
std::vector<T> Matrix<T>::conjugate_gradient_iter(std::vector<T> &b, int iters)
{
	std::cout << "\nSolving with Conjugate Gradient iterative method. . ." << "\n";
	size_t n = rows;
	std::vector<T> x(n), mult(n), temp(n), r(n), t(n);
	std::vector<T> denominator1(n), nom1(n);
	double nominator = 0;
	double denominator = 0;
	double a = 0, beta = 0, nom = 0, denom = 0;
	for (size_t i = 0; i < x.size(); i++)
	{
		x[i] = 0;
	}
	for (int iter = 0; iter < iters; iter++)
	{
		mult = (*this) * x;
		for (size_t i = 0; i < x.size(); i++)
		{
			r[i] = b[i] - mult[i];
		}
		if (iter == 0)
		{
			t = r;
		}
		denominator1 = (*this) * t;
		for (size_t i = 0; i < x.size(); i++)
		{
			nominator += r[i] * r[i];
			denominator += t[i] * denominator1[i];
		}
		a = nominator / denominator;
		temp = (*this) * t;
		for (size_t i = 0; i < x.size(); i++)
		{
			x[i] = x[i] + a * t[i];
			r[i] = r[i] - a * temp[i];
		}
		nom1 = (*this) * t;
		for (size_t i = 0; i < x.size(); i++)
		{
			nom += r[i] * nom1[i];
			denom += t[i] * nom1[i];
		}
		beta = nom / denom;
		for (size_t i = 0; i < x.size(); i++)
		{
			t[i] = r[i] - beta * t[i];
		}
		if (norm(r) < 1e-7)
		{
			std::cout << iter << " iterations\n";
			break;
		}
	}

	return x;
}

template <class T>
std::vector<Matrix<T>> Matrix<T>::LU_factor()
{
	assert(rows == cols);
	std::cout << "\nSolving with LU decomposition method. . ." << "\n";
	size_t n = rows;
	Matrix L(n, n);
	Matrix U(n, n);

	// Doolittle algorithm
	for (size_t i = 0; i < n; i++)
	{
		// matrix U
		for (size_t k = i; k < n; k++)
		{
			T sum = 0.0;
			for (size_t j = 0; j < i; j++)
			{
				sum += L(i, j) * U(j, k);
			}
			U.setValue(i, k, mat[i][k] - sum);
		}

		// matrix L
		for (size_t k = i; k < n; k++)
		{
			if (i == k) L.setValue(i, i, 1);
			else
			{
				T sum = 0.0;
				for (size_t j = 0; j < i; j++)
				{
					sum += L(k, j) * U(j, i);
				}
				L.setValue(k, i, (mat[k][i] - sum) / U(i, i));
			}
		}
	}
	std::vector<Matrix> result(2, Matrix(n, n));
	result[0] = L;
	result[1] = U;

	return result;
}

template <class T>
std::vector<T> Matrix<T>::forward_Euler(std::vector<T> b)
{
	size_t n = rows;
	std::vector<T> y(n);
	for (size_t j = 0; j < n; j++)
	{
		y[0] = b[0] / mat[0][0];
		for (size_t i = 1; i < n; i++)
		{
			T sum = 0;
			for (size_t k = 0; k <= i - 1; k++)
			{
				sum += mat[i][k] * y[k];
			}
			y[i] = (b[i] - sum) / mat[i][i];
		}
	}
	return y;
}

template <class T>
std::vector<T> Matrix<T>::backward_Euler(std::vector<T> b)
{
	int n = rows;
	std::vector<T> x(n);

	for (int j = 0; j < n; j++)
	{
		x[n - 1] = b[n - 1] / mat[n - 1][n - 1]; // OK
		for (int i = n - 2; i >= 0; i--)
		{
			T sum = 0.0;
			for (int k = i + 1; k < n; k++)
			{
				sum += mat[i][k] * x[k];
			}
			x[i] = (b[i] - sum) / mat[i][i];
		}
	}
	return x;
}

template <class T>
std::vector<Matrix<T>> Matrix<T>::QR_factor()
{
	assert(rows == cols);
	std::cout << "\nSolving with QR decomposition method. . ." << "\n";
	size_t n = rows;
	Matrix Q(n, n);
	Matrix R(n, n);

	int cnt = 0;
	Matrix AT = transpose();
	Matrix QT = Q.transpose();
	for (int j = 0; j < n; j++)
	{
		std::vector<T> e;
		std::vector<T> a;
		for (int col = 0; col < n; col++)
		{
			a.push_back(AT.mat[j][col]);
		}
		std::vector<T> u(a);

		for (int i = 0; i < cnt; i++)
		{
			T temp = 0.0;
			for (int k = 0; k < n; k++)
			{
				temp += Q(k,i) * a[k];
			}

			std::vector<T> proj;
			for (int k = 0; k < n; k++)
			{
				proj.push_back(Q(k,i) * temp);
			}

			for (int k = 0; k < n; k++)
			{
				u[k] = u[k] - proj[k];
			}
		}

		T result = 0;
		for (size_t i = 0; i < u.size(); i++)
		{
			result += u[i] * u[i];
		}
		result = sqrt(result);

		for (int k = 0; k < n; k++)
		{
			e.push_back(u[k]/result);
		}

		for (int k = 0; k < n; k++)
		{
			Q.setValue(k,cnt,e[k]);
		}

		cnt++;
	}

	QT = Q.transpose();
	R = QT * (*this);

	std::vector<Matrix> result(2, Matrix(n, n));
	result[0] = Q;
	result[1] = R;

	return result;
}

} // namespace NumericalAnalysis