#include "poHdf5ToXdmf.h"
#include "poHdf5FileOpen.h"
#include "poHdf5RectGridWriter.h"
#include <iostream>

int main(){
    po::hdf5::fileOpen hdf5File("test.hdf5");
    hdf5File.setToReadOnly();
    hdf5File.setRootGroupName("Root");
    int ierr;
    auto root = hdf5File.open(ierr);
    if(ierr != 0){
        std::cerr << "Error: file open problem" << std::endl;
        std::cerr << "Message : " << hdf5File.getErrorString(ierr);
        return -1;
    }
    auto Restart = root.openGroup("Spatial/Restart/RectGrid");
    po::hdf5::xdmfStrcutWriter xdmf;

    xdmf.setGroup(Restart);
    std::cout << xdmf << std::endl;
    
}