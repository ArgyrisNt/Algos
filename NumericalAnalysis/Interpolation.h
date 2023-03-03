#include <iostream>
#include <vector>

namespace NumericalAnalysis
{

    struct cspline
    {
        double a, b, c, d, x;
    };

    class Interpolation
    {
        public:
            std::vector<std::pair<double, double>> linear(std::vector<std::pair<double, double>> &dataset);
            std::vector<std::pair<double, double>> lagrange(std::vector<std::pair<double, double>> &dataset);
            std::vector<std::pair<double, double>> newton(std::vector<std::pair<double, double>> &dataset);
            std::vector<std::pair<double, double>> cubicSpline(std::vector<std::pair<double, double>> &dataset);
            std::vector<std::pair<double, double>> bezier(std::vector<std::pair<double, double>> &dataset, std::vector<double>& times);

            void plot(std::string filename);

        private:
            std::vector<std::pair<double, double>> interpolated_data;
    };

} // namespace NumericalAnalysis

#include "Interpolation.cpp"