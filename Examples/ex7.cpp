// Example of iterative solvers

#include <iostream>
#include "..\NumericalAnalysis\Matrix.h"

int main()
{
    std::cout.flush();
    int iters = 30;
    std::vector<double> x(100);
    double omega = 1.03125;
    std::vector<double> b(100);
    NumericalAnalysis::Matrix<double> A(100, 100, 0.0);
    for (size_t i = 0; i < 100; i++)
    {
        for (size_t j = 0; j < 100; j++)
        {
            if (j == i)
            {
                A.setValue(i, j, 10.0);
            }
            else if ((j == i + 1) || (j == i - 1))
            {
                A.setValue(i, j, -1);
            }
        }
        if ((i == 0) || (i == 99))
        {
            b[i] = 9; // 0;
        }
        else
        {
            b[i] = 8; // 0;
        }
    }

    x = A.Jacobi_iterator(b, iters);
    x = A.GaussSeidel_iterator(b, iters);
    x = A.SOR_iterator(b, iters, omega);
    x = A.gradient_iterator(b, iters);
    x = A.conjugate_gradient_iter(b, iters);

    return 0;
}