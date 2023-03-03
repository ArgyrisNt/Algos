#include <iostream>
#include <vector>
#include "..\NumericalAnalysis\Matrix.h"

namespace NumericalAnalysis
{

    std::vector<std::pair<double, double>> Interpolation::linear(std::vector<std::pair<double, double>> &dataset)
    {
        interpolated_data = {};
        for (int i = 0; i < dataset.size() - 1; i++)
        {
            std::pair<double, double> p0 = dataset[i];
            std::pair<double, double> p1 = dataset[i + 1];
            for (int j = 0; j < 5; j++)
            {
                double x = p0.first + ((double) j) * ((p1.first - p0.first) / 4.0);
                double y = (p0.second * (p1.first - x) + p1.second * (x - p0.first)) / (p1.first - p0.first);
                std::pair<double, double> temp = std::make_pair(x, y);
                interpolated_data.push_back(temp);
            }
        }

        return interpolated_data;
    }

    std::vector<std::pair<double, double>> Interpolation::lagrange(std::vector<std::pair<double, double>> &dataset)
    {
        interpolated_data = {};
        for (int k = 0; k < 20; k++)
        {
            double x = dataset[0].first + ((double) k) * ((dataset[dataset.size()-1].first - dataset[0].first) / 19.0);
            double L = 0.0;
            for (int i = 0; i < dataset.size(); i++)
            {
                double nominator = 1.0;
                double denominator = 1.0;
                for (int j = 0; j < dataset.size(); j++)
                {
                    if (i == j) continue;
                    nominator *= (x - dataset[j].first);
                    denominator *= (dataset[i].first - dataset[j].first);
                }
                double li = nominator / denominator;

                L += dataset[i].second * li;
            }
            interpolated_data.push_back(std::make_pair(x, L));
        }

        return interpolated_data;
    }

    void Interpolation::plot(std::string filename)
    {
        std::ofstream my_file(filename);
        if (my_file.is_open())
        {
            my_file << "variables= " << "\"x\"" << "," << "\"y\"" << "\n";
            my_file << "zone t= " << "\"1\"" << ",i=" << interpolated_data.size() << ",j=" << interpolated_data.size() << "\n";
            for (int i = 0; i < interpolated_data.size(); ++i)
            {
                my_file << interpolated_data[i].first << " " << interpolated_data[i].second << "\n";
            }
        }
        my_file.close();
    }

    std::vector<std::pair<double, double>> Interpolation::newton(std::vector<std::pair<double, double>> &dataset)
    {
        interpolated_data = {};
        Matrix<double> coef(dataset.size(), dataset.size(), 0.0);
        for (int i = 0; i < dataset.size(); ++i)
        {
            coef.setValue(i, 0, dataset[i].second);
        }
        for (int j = 1; j < dataset.size(); j++)
        {
            for (int i = 0; i < dataset.size() - j; i++)
            {
                double temp = (coef(i+1,j-1) - coef(i,j-1)) / (dataset[i+j].first - dataset[i].first);
                coef.setValue(i, j, temp);
            }
        }

        for (int i = 0; i < 20; i++)
        {
            double x = dataset[0].first + ((double) i) * ((dataset[dataset.size() - 1].first - dataset[0].first) / 19.0);
            double p = coef(0, dataset.size() - 1);
            for (int k = 1; k < dataset.size(); k++)
            {
                p = coef(0, dataset.size() - 1 - k) + (x - dataset[dataset.size() - 1 - k].first) * p;
            }
            interpolated_data.push_back(std::make_pair(x, p));
        }

        return interpolated_data;
    }

    std::vector<std::pair<double, double>> Interpolation::cubicSpline(std::vector<std::pair<double, double>> &dataset)
    {
        interpolated_data = {};
        int n = dataset.size() - 1;
        std::vector<double> a;
        for (int i = 0; i < n + 1; i++) a.push_back(dataset[i].second);

        std::vector<double> b(n);
        std::vector<double> d(n);
        std::vector<double> h;
        
        for (int i = 0; i < n; i++)
            h.push_back(dataset[i+1].first - dataset[i].first);

        std::vector<double> alpha;
        alpha.push_back(0);
        for (int i = 1; i < n; ++i)
            alpha.push_back(3*(a[i+1]-a[i])/h[i] - 3*(a[i]-a[i-1])/h[i-1]);

        std::vector<double> c(n + 1);
        std::vector<double> l(n+1);
        std::vector<double> mu(n+1);
        std::vector<double> z(n + 1);
        l[0] = 1;
        mu[0] = 0;
        z[0] = 0;
        for (int i = 1; i < n; ++i)
        {
            l[i] = 2 * (dataset[i + 1].first - dataset[i - 1].first) - h[i - 1] * mu[i - 1];
            mu[i] = h[i] / l[i];
            z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
        }

        l[n] = 1;
        z[n] = 0;
        c[n] = 0;
        for (int j = n - 1; j >= 0; --j)
        {
            c[j] = z[j] - mu[j] * c[j + 1];
            b[j] = (a[j + 1] - a[j]) / h[j] - h[j] * (c[j + 1] + 2 * c[j]) / 3;
            d[j] = (c[j + 1] - c[j]) / (3 * h[j]);
        }

        std::vector<cspline> splines(n);
        for (int i = 0; i < n; ++i)
        {
            splines[i].a = a[i];
            splines[i].b = b[i];
            splines[i].c = c[i];
            splines[i].d = d[i];
            splines[i].x = dataset[i].first;
        }

        for (int k = 0; k < 20; k++)
        {
            double x = dataset[0].first + ((double)k) * ((dataset[dataset.size() - 1].first - dataset[0].first) / 19.0);
            double value = 0.0;
            for (int i = 0; i < n; i++)
            {
                if (x >= dataset[i].first && x <= dataset[i + 1].first)
                {
                    value += splines[i].a + splines[i].b * (x - splines[i].x) + splines[i].c * pow(x - splines[i].x, 2) + splines[i].d * pow(x - splines[i].x, 3);
                }
            }
            interpolated_data.push_back(std::make_pair(x, value));
        }
        return interpolated_data;
    }

    std::vector<std::pair<double, double>> Interpolation::bezier(std::vector<std::pair<double, double>> &dataset, std::vector<double> &times)
    {
        // Aitken
        interpolated_data = {};
        int n = dataset.size();
        std::vector<std::pair<double, double>> old_data = dataset;
        for (int k = 0; k < 20; k++)
        {
            double t = times[0] + k * ((times[times.size()-1] - times[0]) / 19.0);
            for (int i = 1; i < n; i++)
            {
                for (int j = 0; j < n - i; j++)
                {
                    dataset[j].first = ((times[j+i] - t) / (times[i+j] - times[j])) * dataset[j].first +
                        ((t - times[j]) / (times[i+j] - times[j])) * dataset[j+1].first;
                    dataset[j].second = ((times[j + i] - t) / (times[i + j] - times[j])) * dataset[j].second +
                        ((t - times[j]) / (times[i + j] - times[j])) * dataset[j + 1].second;
                }
            }
            interpolated_data.push_back(std::make_pair(dataset[0].first, dataset[0].second));
            dataset = old_data;
        }

        return interpolated_data;
    }

}