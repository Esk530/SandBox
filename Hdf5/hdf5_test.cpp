#include "poHdf5FileOpen.h"
#include "poHdf5Writer.h"
#include <vector>
#include <string>

int main(int argc , char** argv)
{
    po::hdf5::fileOpen<po::hdf5::write> Open("Root");
    auto Root = Open("../../test.hdf5");
    std::vector<float> float_value(10,10);
    std::vector<double> double_value(10,10);
    std::vector<int> int_value(10,10);
    std::vector<unsigned int> uint_value(10,10);
    std::vector<std::string> string_value(10,"test");
    po::hdf5::writer write(Root);
    write("float_value",float_value);
    write("double_value",double_value);
    write("int_value",int_value);
    write("uint_value",uint_value);
    write("string_value",string_value);
}