#ifndef __PO_HDF5_TO_XDMF_H__
#define __PO_HDF5_TO_XDMF_H__
#include <string>
#include <sstream>
#include <H5Cpp.h>
#include "poHdf5FileOpen.h"
#include "poHdf5ReadAttribute.h"

namespace po
{
    namespace hdf5
    {


        class xdmfStrcutWriter{

        public:

            xdmfStrcutWriter(){}


/*             int open(std::string hdf5File)
            {
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
            } */

            void setGroup(H5::Group _group){
                group = _group;
                read = po::hdf5::group::readAttribute(_group);
            }



            int getNumData()const{
                int ret;
                read("NumData",ret);
                return ret;
            }

            std::shared_ptr<std::vector<std::string>> getDataList()const{
                int numData = getNumData();
                auto dataList = std::make_shared<std::vector<std::string>>(numData);
                read("DataList",*dataList);
                return dataList;
            }

            std::shared_ptr<std::vector<int>> getNumberOfElement()const{
                auto ret = std::make_shared<std::vector<int>>(100,-100); 
                read("NumberOfElement",*ret);
                int count = 0;
                for(auto v : *ret){
                    if(v < 0){
                        break;
                    }
                    count++;
                }
                ret->resize(count);
                return ret;
            }

            std::shared_ptr<std::vector<std::string>> getGeomItem()const{
                int numGeom;
                read("NumGeom",numGeom);
                auto ret = std::make_shared<std::vector<std::string>>(numGeom);
                read("GeomItem",*ret);
                return ret;
            }

            std::string getGeomType()const{
                std::string ret;
                read("GeomType",ret);
                return ret;
            }

            std::string getName()const{
                std::string ret;
                read("Name",ret);
                return ret;
            }

            std::string getTopoType()const{
                std::string ret;
                read("TopologyType",ret);
                return ret;
            }

            std::string getTopoItem()const{
                std::string ret;
                read("TopoItem",ret);
                return ret;
            }


            std::string getDataItemXdmf(std::string name)const{
                std::stringstream ss;
                auto dataset = group.openDataSet(name.c_str());
                po::hdf5::dataset::readAttribute dAtt(dataset);
                std::string numberOfType,PATH;
                int Precision;

                dAtt("NumberOfType",numberOfType);
                dAtt("Precision",Precision);
                auto dim = dAtt.getVector<int>("Dimensions");
                ss << "    <DataItem Format=\"HDF\" ";
                ss << "ItemType=\"Uniform\" ";
                ss << "NumberOfType=\"" << numberOfType << "\" ";
                ss << "Precision=\"" << Precision << "\" ";
                ss << "Dimensions=\"" << 1498 ;
                for(auto iter = dim->begin();iter != dim->end();iter++)ss << *iter << " ";
                ss << "\">" << std::endl;
                ss << "      ";
                dAtt("PATH",PATH);
                ss << hdfFilePath << "/" << PATH << std::endl;
                ss << "    </DataItem>";
                return ss.str();
            }

        private:
            std::string hdfFilePath;
            H5::Group group;
            po::hdf5::group::readAttribute read;

        };

        std::ostream& operator<<(std::ostream& oss, const xdmfStrcutWriter& xdmf)
        {
            auto numElem = xdmf.getNumberOfElement();
            
            oss << "<Grid Name=\"" << xdmf.getName() << "\" GridType=\"Uniform\">" << std::endl;
            
            // Write topology
            oss << "  <Topology TopologyType=\"" << xdmf.getTopoType() << "\" NumberOfElements=\"";
            for(auto v: *numElem){oss << v << " ";}
            // write topology
            std::string topoItem = xdmf.getTopoItem();
            if(topoItem != "None"){
                oss << "\">" << std::endl;
                oss << xdmf.getDataItemXdmf(topoItem) << std::endl;
                oss << "</Topology>" << std::endl;
            }else{
                oss << "\"/>" << std::endl;
            }

            // write geometry
            auto geomItem = xdmf.getGeomItem();

            oss << "   <Geometry GeometryType=\"" << xdmf.getGeomType() << "\">" << std::endl;
            for(auto v : *geomItem)oss << v << std::endl;
            oss << "   </Geometry>" << std::endl;

            oss << "</Grid>" << std::endl;
            return oss;

        }


    } // namespace hdf5
    
} // namespace po


#endif// __PO_HDF5_TO_XDMF_H__