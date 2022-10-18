#include <fstream>
#include <iostream>
#include <map>
#include "../common.h"

constexpr int MANTISSA_BITS = 9;
constexpr int EXP_BIAS = 15;
constexpr int MAX_VALID_BIASED_EXP = 31;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: expPow <lut1_file> <lut2_file>" << std::endl;
        return -1;
    }

    double expPow2[MAX_VALID_BIASED_EXP + 1];
    for (int exp = 0; exp <= MAX_VALID_BIASED_EXP; ++exp)
    {
        double val = pow(2.0, (double)exp - EXP_BIAS - MANTISSA_BITS);
        expPow2[exp] = val;
    }

    std::ofstream lutExpPow2(argv[1]);
    if (!lutExpPow2.is_open())
    {
        std::cout << "Couldn't write to file " << argv[1] << "." << std::endl;
        return -1;
    }

    lutExpPow2.precision(std::numeric_limits<float>::max_digits10);
    writeGeneratedByUtilityToolWarning(lutExpPow2);
    lutExpPow2 << "// 2^(exp - EXP_BIAS - MANTISSA_BITS)" << std::endl;
    lutExpPow2 << "constexpr float expPow2[MAX_VALID_BIASED_EXP + 1] = {" << std::endl;
    lutExpPow2 << "    ";
    for (int i = 0; i <= MAX_VALID_BIASED_EXP; ++i)
    {
        lutExpPow2 << (float)expPow2[i] << (expPow2[i] < 1.0 ? "f" : ".f");
        if (i && i % 4 == 0)
            lutExpPow2 << "," << std::endl << "    ";
        else if (i < MAX_VALID_BIASED_EXP)
            lutExpPow2 << ", ";
    }
    lutExpPow2 << std::endl << "};" << std::endl;

    std::ofstream lutRcpExpPow2(argv[2]);
    if (!lutRcpExpPow2.is_open())
    {
        std::cout << "Couldn't write to file " << argv[2] << "." << std::endl;
        return -1;
    }

    lutRcpExpPow2.precision(std::numeric_limits<float>::max_digits10);
    writeGeneratedByUtilityToolWarning(lutRcpExpPow2);
    lutRcpExpPow2 << "// 1/(2^(exp - EXP_BIAS - MANTISSA_BITS))" << std::endl;
    lutRcpExpPow2 << "constexpr float rcpExpPow2[MAX_VALID_BIASED_EXP + 1] = {" << std::endl;
    lutRcpExpPow2 << "    ";
    for (int i = 0; i <= MAX_VALID_BIASED_EXP; ++i)
    {
        double val = 1./expPow2[i];
        lutRcpExpPow2 << (float)val << (val < 1.0 ? "f" : ".f");
        if (i && i % 4 == 0)
            lutRcpExpPow2 << "," << std::endl << "    ";
        else if (i < MAX_VALID_BIASED_EXP)
            lutRcpExpPow2 << ", ";
    }
    lutRcpExpPow2 << std::endl << "};" << std::endl;

    std::cout << "End of source generation" << std::endl;
    return 0;
}
