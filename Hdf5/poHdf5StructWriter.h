#ifndef __PO_HDF5_PARTICLE_WRITER_H__
#define __PO_HDF5_PARTICLE_WRITER_H__

#include <H5Cpp.h>
#include <vector>
#include <string>

#include "poHdfDataArray.h"
#include "poHdf5WriteTimeIndex.h"
#include "poHdf5WriteAttribute.h"

namespace po
{
    namespace hdf5
    {
        class writeStruct
        {
        public:

            writeStruct(){}

            void setName(std::string _name){name =_name;}

            void open(H5::Group _group){
                parent = _group;
                po::hdf5::getTimeIndex timeIndex(5);
                std::string id = timeIndex(itime);
                if(_group.exists(dataGroupName.c_str())){
                    auto temp = parent.openGroup(dataGroupName.c_str());
                    if(itime < 0){
                        group = temp.createGroup(name.c_str());
                        return;
                    }
                    if(temp.exists(id.c_str())){
                        group = temp.openGroup(id.c_str()).createGroup(name.c_str());
                    }else{
                        group = temp.createGroup(id.c_str()).createGroup(name.c_str());
                    }
                }
                else{

                    auto temp = parent.createGroup(dataGroupName.c_str());
                    if(itime < 0){
                        group = temp.createGroup(name.c_str());
                        return;
                    }
                    group = temp.createGroup(id.c_str()).createGroup(name.c_str());
                }
                opened = true;
            }

            void setToFixed(){
                dataGroupName = "Fixed";
            }

            void setToRestart(){
                dataGroupName = "Restart";
            }

            void setToSelected(){
                dataGroupName = "Selected";
            }

            void setTime(int _itime, float _time)
            {
                itime = _itime;
                time  = _time;
            }



            template<typename T>
            void write(po::hdf5::dataArray<T>& data,int dim = 1)
            {
                numData++;
                dataList.push_back(data.getKey());
                data.setRoot(rootName);
                std::vector<int> temp(numElems.begin(),numElems.end());
                temp.push_back(dim);
                data.setDimensions(temp);
                data.write(group);
            }

            void close(){
                if(!opened)return;
                po::hdf5::group::writeAttribute writeAtt(group);
                writeAtt("Name",name);
                writeAtt("TopologyType",topoType);
                writeAtt("NumberOfElement",numElems);
                writeAtt("NumData",numData);
                writeAtt("DataList",dataList);
                writeAtt("GeomType",geomType);
                writeAtt("NumGeom",static_cast<int>(geomItem.size()));
                writeAtt("GeomItem",geomItem);
                writeAtt("TopoItem",topoItem);
                group.close();
            }

            virtual void setRootName(std::string _name ){
                po::hdf5::getTimeIndex timeIndex(5);
                std::string id = timeIndex(itime);
                rootName = _name + "/" + dataGroupName + "/" + id + "/"  + name;
            }

            ~writeStruct(){this->close();}


        protected:
            H5::Group parent,group;
            std::string topoType;
            std::string name;
            std::string geomType;
            std::string rootName;
            std::string dataGroupName;
            std::vector<int> numElems;
            std::string topoItem = "None";
            std::vector<std::string> geomItem;
            int numData = 0;
            std::vector<std::string> dataList;
            bool opened = false;
            int itime = -1;
            float time;
        };
    } // namespace hdf5
    
} // namespace po


#endif// __PO_HDF5_PARTICLE_WRITER_H__