
#include "poHdf5FileOpen.h"
#include "poHdf5RectGridWriter.h"
#include "poHdf5WriteTimeIndex.h"
#include <iostream>
int main(){
    
    po::hdf5::fileOpen hdf5File("test.hdf5");
    hdf5File.setToWrite();
    hdf5File.setRootGroupName("Root");
    int ierr;
    auto root = hdf5File.open(ierr);
    if(ierr != 0){
        std::cerr << "Error: file open problem" << std::endl;
        std::cerr << "Message : " << hdf5File.getErrorString(ierr);
        return -1;
    }

    auto spatial = root.createGroup("Spatial");
    {
        po::hdf5::writeRectGrid3D rectGridWriter;
        rectGridWriter.setToFixed();
        rectGridWriter.setName("RectGrid");
        rectGridWriter.setDimensions(100,100,100);
        rectGridWriter.open(spatial);
        std::vector<float> A(100);
        for(int i = 0; i < 100;i++)A[i] = i;
        rectGridWriter.setXCoordinate(A);
        rectGridWriter.setYCoordinate(A);
        rectGridWriter.setZCoordinate(A);
    }

    {
        po::hdf5::writeRectGrid3D rectGridWriter;
        rectGridWriter.setToRestart();
        rectGridWriter.setTime(0,1.0f);
        rectGridWriter.setDimensions(100,100,100);
        rectGridWriter.setName("RectGrid");
        rectGridWriter.setRootName("Root/Spatial");
        rectGridWriter.open(spatial);
        std::vector<float> B(100*100*100,1);
        po::hdf5::dataArray<float> _B(100*100*100);
        _B.setKey("B");
        _B.setDescription("B value");
        _B.setCenterToCell();
        _B.set(B.begin(),B.end());
        rectGridWriter.write(_B);

    }

    return 0;
}