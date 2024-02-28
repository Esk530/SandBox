#ifndef __PO_HDF5_STRUCT_WRITER_H__
#define __PO_HDF5_STRUCT_WRITER_H__
#include "poHdf5StructWriter.h"
#include "poHdfDataArray.h"

namespace po
{
    namespace hdf5
    {
        class writeRectGrid3D : public writeStruct
        {
        public:
            writeRectGrid3D(){
                topoType = "3DRectMesh";
                geomType = "XYZ";

            }

            void setDimensions(int nx, int ny, int nz){
                numElems.resize(3);
                numElems[0] = nx;
                numElems[1] = ny;
                numElems[2] = nz;
                return;
            }

            void setRootName(std::string _name){

                geomItem.resize(3);
                geomItem[0] = _name + "/Fixed/" + name + "/X";
                geomItem[1] = _name + "/Fixed/" + name + "/Y";
                geomItem[2] = _name + "/Fixed/" + name + "/Z";

                if(itime > 0){
                    po::hdf5::getTimeIndex timeIndex(5);
                    std::string id = timeIndex(itime);
                    rootName = _name + "/" + dataGroupName + "/" + id + "/"  + name;
                }else{
                    rootName = _name + "/" + dataGroupName + "/"  + name;
                }
            }

            template<typename real>
            void setXCoordinate(std::vector<real> x)
            {
                po::hdf5::dataArray<real> X(static_cast<int>(x.size()));
                X.setKey("X");
                X.setDescription("X-Coordinate");
                X.set(x.begin(),x.end());
                X.setRoot(rootName);
                X.write(group);
            }

            template<typename real>
            void setYCoordinate(std::vector<real> y)
            {
                po::hdf5::dataArray<real> Y(static_cast<int>(y.size()));
                Y.setKey("Y");
                Y.setDescription("Y-Coordinate");
                Y.set(y.begin(),y.end());
                Y.setRoot(rootName);
                Y.write(group);
            }

            template<typename real>
            void setZCoordinate(std::vector<real> z)
            {
                po::hdf5::dataArray<real> Z(static_cast<int>(z.size()));
                Z.setKey("Z");
                Z.setDescription("Z-Coordinate");
                Z.set(z.begin(),z.end());
                Z.setRoot(rootName);
                Z.write(group);
            }
        };
    }
} // namespace po



#endif// __PO_HDF5_STRUCT_WRITER_H__