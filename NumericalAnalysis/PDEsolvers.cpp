#include <iostream>
#include <vector>
#include <fstream>
#include <string>

namespace NumericalAnalysis
{

    Matrix<double> PDEsolvers::heat_explicit(double x_init, double x_last, double t_init, double Nt, double Nx)
    {
        double t0 = 0.0, tf = 1.0;
        double a = 0.0, b = 1.0;
        double k = tf / Nt;
        double h = b / (Nx + 1);
        double r = k / (h * h);
        std::cout << "r = " << r << std::endl;
        Matrix<double> U(Nt, Nx, 0.0);

        for (int i = 0; i < Nx; i++)
        {
            U.setValue(0, i, t_init);
        }

        for (int i = 0; i < Nt; i++)
        {
            U.setValue(i, 0, x_init);
            U.setValue(i, Nx - 1, x_last);
        }

        for (int n = 0; n < Nt - 1; n++)
        {
            for (int i = 1; i < Nx - 1; i++)
            {
                U.setValue(n + 1, i, (1 - 2.0 * r) * U(n, i) + r * U(n, i - 1) + r * U(n, i + 1));
            }
        }

        for (int n = 0; n < Nt; n += 25)
        {
            std::string filename(std::to_string(n) + "sol.dat");
            std::ofstream my_file(filename);
            if (my_file.is_open())
            {
                my_file << "variables= "
                        << "\"x\""
                        << ","
                        << "\"y\""
                        << "\n";
                my_file << "zone t= "
                        << "\"1\""
                        << ",i=" << Nx << ",j=" << Nx << "\n";
                for (int pt = 0; pt < Nx; pt++)
                {
                    double X = pt * h;
                    my_file << X << " " << U(n, pt) << "\n";
                }
                my_file.close();
            }
        }

        return U;
    }

    Matrix<double> PDEsolvers::heat_implicit(double x_init, double x_last, double t_init, double Nt, double Nx)
    {
        double t0 = 0.0, tf = 1.0;
        double a = 0.0, b = 1.0;
        double k = tf / Nt;
        double h = b / (Nx + 1);
        double r = k / (h * h);
        Matrix<double> U(Nt, Nx, 0.0);

        for (int i = 0; i < Nx; i++)
        {
            U.setValue(0, i, t_init);
        }

        for (int i = 0; i < Nt; i++)
        {
            U.setValue(i, 0, x_init);
            U.setValue(i, Nx - 1, x_last);
        }

        Matrix<double> D(Nx - 2, Nx - 2, 0.0);
        std::vector<double> B(Nx - 2);
        for (int j = 0; j < Nx - 3; j++)
        {
            D.setValue(j + 1, j, -r);
            D.setValue(j, j + 1, -r);
        }
        for (int j = 0; j < Nx - 2; j++)
        {
            D.setValue(j, j, 1 + 2.0 * r);
        }

        Matrix<double> C(Nt, Nx - 2, 0.0);
        for (int n = 0; n < Nt; n++)
        {
            for (int i = 0; i < Nx - 2; i++)
            {
                C.setValue(n, i, U(n, i + 1));
            }
        }
        Matrix<double> TC(Nx - 2, Nt, 0.0);
        for (int i = 0; i < Nx - 2; i++)
        {
            for (int j = 0; j < Nt; j++)
            {
                TC.setValue(i, j, C(j, i));
            }
        }

        for (int n = 0; n < Nt - 1; n++)
        {
            for (int i = 0; i < Nx - 2; i++)
            {
                B[i] = TC(i, n);
            }
            std::vector<double> res = D.GaussSeidel_iterator(B, 10);
            for (int i = 0; i < Nx - 2; i++)
            {
                TC.setValue(i, n + 1, res[i]);
            }
        }

        for (int n = 0; n < Nt; n++)
        {
            for (int i = 0; i < Nx - 2; i++)
            {
                U.setValue(n, i + 1, TC(i, n));
            }
        }

        for (int n = 0; n < Nt; n += 25)
        {
            std::string filename(std::to_string(n) + "sol.dat");
            std::ofstream my_file(filename);
            if (my_file.is_open())
            {
                my_file << "variables= "
                        << "\"x\""
                        << ","
                        << "\"y\""
                        << "\n";
                my_file << "zone t= "
                        << "\"1\""
                        << ",i=" << Nx << ",j=" << Nx << "\n";
                for (int pt = 0; pt < Nx; pt++)
                {
                    double X = pt * h;
                    my_file << X << " " << U(n, pt) << "\n";
                }
                my_file.close();
            }
        }

        return U;
    }

    Matrix<double> PDEsolvers::heat_CrankNicolson(double x_init, double x_last, double t_init, double Nt, double Nx)
    {
        double t0 = 0.0, tf = 1.0;
        double a = 0.0, b = 1.0;
        double k = tf / Nt;
        double h = b / (Nx + 1);
        double r = k / (h * h);
        Matrix<double> U(Nt, Nx, 0.0);

        for (int i = 0; i < Nx; i++)
        {
            U.setValue(0, i, t_init);
        }

        for (int i = 0; i < Nt; i++)
        {
            U.setValue(i, 0, x_init);
            U.setValue(i, Nx - 1, x_last);
        }

        Matrix<double> D(Nx - 2, Nx - 2, 0.0);
        std::vector<double> B(Nx - 2);
        for (int j = 0; j < Nx - 3; j++)
        {
            D.setValue(j + 1, j, -r);
            D.setValue(j, j + 1, -r);
        }
        for (int j = 0; j < Nx - 2; j++)
        {
            D.setValue(j, j, 2.0 + 2.0 * r);
        }

        Matrix<double> C(Nt, Nx - 2, 0.0);
        for (int n = 0; n < Nt; n++)
        {
            for (int i = 0; i < Nx - 2; i++)
            {
                C.setValue(n, i, U(n, i + 1));
            }
        }
        Matrix<double> TC(Nx - 2, Nt, 0.0);
        for (int i = 0; i < Nx - 2; i++)
        {
            for (int j = 0; j < Nt; j++)
            {
                TC.setValue(i, j, C(j, i));
            }
        }

        for (int n = 0; n < Nt - 1; n++)
        {
            for (int i = 1; i < Nx - 3; i++)
            {
                B[i] = (2.0 - 2.0 * r) * TC(i, n) + r * TC(i - 1, n) + r * TC(i + 1, n);
            }
            B[0] = (2.0 - 2.0 * r) * TC(0, n) + r * TC(1, n);
            B[Nx - 3] = (2.0 - 2.0 * r) * TC(Nx - 3, n) + r * TC(Nx - 4, n);
            std::vector<double> res = D.Jacobi_iterator(B, 10);
            for (int i = 0; i < Nx - 2; i++)
            {
                TC.setValue(i, n + 1, res[i]);
            }
        }

        for (int n = 0; n < Nt; n++)
        {
            for (int i = 0; i < Nx - 2; i++)
            {
                U.setValue(n, i + 1, TC(i, n));
            }
        }

        for (int n = 0; n < Nt; n += 25)
        {
            std::string filename(std::to_string(n) + "sol.dat");
            std::ofstream my_file(filename);
            if (my_file.is_open())
            {
                my_file << "variables= "
                        << "\"x\""
                        << ","
                        << "\"y\""
                        << "\n";
                my_file << "zone t= "
                        << "\"1\""
                        << ",i=" << Nx << ",j=" << Nx << "\n";
                for (int pt = 0; pt < Nx; pt++)
                {
                    double X = pt * h;
                    my_file << X << " " << U(n, pt) << "\n";
                }
                my_file.close();
            }
        }

        return U;
    }

    Matrix<double> PDEsolvers::advection_upwind(double x_init, double x_last, double t_init, double Nt, double Nx)
    {
        double t0 = 0.0, tf = 1.0;
        double a = 0.0, b = 1.0;
        double k = tf / Nt;
        double h = b / (Nx + 1);
        double alpha = 2.0;
        double ni = k / h;
        std::cout << "courant = " << ni << std::endl;
        Matrix<double> U(Nt, Nx, 0.0);

        for (int i = 0; i < Nx; i++)
        {
            double X = i * h;
            if (X > 0.1 && X < 0.2)
            {
                U.setValue(0, i, std::pow(10.0, 4.0) * std::pow((0.1 - X), 2.0) * std::pow((0.2 - X), 2.0));
            }
        }

        for (int i = 0; i < Nt; i++)
        {
            U.setValue(i, 0, x_init);
            U.setValue(i, Nx - 1, x_last);
        }

        for (int n = 0; n < Nt - 1; n++)
        {
            for (int i = 1; i < Nx; i++)
            {
                U.setValue(n + 1, i, (1 - alpha * ni) * U(n, i) + alpha * ni * U(n, i - 1));
            }
        }

        for (int n = 0; n < Nt; n += 5)
        {
            std::string filename(std::to_string(n) + "sol.dat");
            std::ofstream my_file(filename);
            if (my_file.is_open())
            {
                my_file << "variables= "
                        << "\"x\""
                        << ","
                        << "\"y\""
                        << "\n";
                my_file << "zone t= "
                        << "\"1\""
                        << ",i=" << Nx << ",j=" << Nx << "\n";
                for (int pt = 0; pt < Nx; pt++)
                {
                    double X = pt * h;
                    my_file << X << " " << U(n, pt) << "\n";
                }
                my_file.close();
            }
        }

        return U;
    }

}