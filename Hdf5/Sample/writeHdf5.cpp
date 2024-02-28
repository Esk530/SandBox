#include "poHdf5FileOpen.h"
#include "poHdf5RectGridWriter.h"
#include "poHdf5WriteTimeIndex.h"
#include "poHdfWriter.h"
#include "poHdf5WriteAttribute.h"
#include <iostream>

int main(){

    // Open And Create Root Group
    po::hdf5::fileOpen hdf5File("writeHdf5.hdf5");
    hdf5File.setToWrite();
    hdf5File.setRootGroupName("Root");
    int ierr;
    auto root = hdf5File.open(ierr);
    if(ierr != 0){
        std::cerr << "Error: file open problem" << std::endl;
        std::cerr << "Message : " << hdf5File.getErrorString(ierr);
        return -1;
    }
    auto group = root.createGroup("test");


    // write attribute to group
    po::hdf5::group::writeAttribute writeAtt(group);
    writeAtt("string","This is write attribute test");
    writeAtt("integer",1);
    writeAtt("float",1.0f);
    writeAtt("double",static_cast<double>(1.0f));
    std::vector<std::string> test(5);
    test[0] = "Watarai";
    test[1] = "Hayashi";
    test[2] = "Sano";
    test[3] = "Maki";
    test[4] = "Miyazaki";
    writeAtt("Baystart",test);

    std::vector<int> A(10);
    for(int i = 0; i < 10;i++)A[i] = i;
    writeAtt("Counter",A);

    // Write Data to Group
    po::hdf5::writeHdf5 write(group);
    std::string temp = "string test";
    write("string",temp);
    po::hdf5::dataset::writeAttribute wat(group.openDataSet("string"));
    std::string temp_str = "This is string value";
    wat("Desc",temp_str);
    write("integer",1);
    write("float",1.0f);
    write("double",static_cast<double>(2.0f));

    {
        po::hdf5::extendDataWriter ewrite(group);
        ewrite("extends",10);
    }
    {
        po::hdf5::extendDataWriter ewrite(group);
        ewrite("extends",20);
    }
}