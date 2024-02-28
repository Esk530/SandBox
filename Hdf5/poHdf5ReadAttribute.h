#ifndef __PO_HDF5_READ_ATTRIBUTE_H__
#define __PO_HDF5_READ_ATTRIBUTE_H__
#include <H5Cpp.h>
#include <vector>
#include <memory>
#include "poHdf5GetPredType.h"
namespace po
{
    namespace hdf5
    {
        namespace group
        {
            struct readAttribute{
                H5::Group group;
 
                readAttribute(){}
                readAttribute(H5::Group _group):group(_group){}

                template<typename T>
                void operator()(std::string name, T& value)const{
                    auto att = group.openAttribute(name.c_str());
                    att.read(po::hdf5::getPredType<T>::getDataType(),&value);
                    return;
                }

                template<>
                void operator()<std::string>(std::string name ,std::string& data)const{
                    H5::StrType datatype(po::hdf5::getPredType<std::string>::get(),1024);
                    datatype.setOrder(H5T_ORDER_LE);
                    auto att = group.openAttribute(name.c_str());
                    char temp[1024];
                    att.read(datatype,temp);
                    data = temp;
                    return;
                }

                template<typename T>
                void operator()(std::string name, std::vector<T>& ret)const
                {
                    auto att = group.openAttribute(name.c_str());
                    att.read(getPredType<T>::getDataType(),ret.data());
                }

                template<>
                void operator()(std::string name,std::vector<std::string>& data)const
                {
                    H5::StrType datatype(po::hdf5::getPredType<std::string>::get(),H5T_VARIABLE);
                    datatype.setOrder(H5T_ORDER_LE);
                    int N = data.size();
                    char** temp = new  char*[N];
                    auto att = group.openAttribute(name.c_str());
                    att.read(datatype,temp);

                    for(int i =0 ; i < N;i++){
                        data[i] = temp[i];
                    }
                    return ;
                }

            };
        } // namespace group
        
        namespace dataset
        {
            struct readAttribute{

                readAttribute(){}
                readAttribute(H5::DataSet _dataset):dataset(_dataset){}

                template<typename T>
                void operator()(std::string name, T& value)const{
                    auto att = dataset.openAttribute(name.c_str());
                    att.read(po::hdf5::getPredType<T>::getDataType(),&value);
                    return;
                }

                template<>
                void operator()<std::string>(std::string name ,std::string& data)const{
                    H5::StrType datatype(po::hdf5::getPredType<std::string>::get(),1024);
                    datatype.setOrder(H5T_ORDER_LE);
                    auto att = dataset.openAttribute(name.c_str());
                    char temp[1024];
                    att.read(datatype,temp);
                    data = temp;
                    return;
                }

                template<typename T>
                std::shared_ptr<std::vector<T>> getVector(std::string name)const
                {
                    auto att = dataset.openAttribute(name.c_str());
                    std::shared_ptr<std::vector<T>> ret = std::make_shared<std::vector<T>>(100,-100);
                    att.read(getPredType<T>::getDataType(),ret->data());
                    int size;
                    for(int i = 0; i < 100;i++){
                        if(ret->at(i) < 0){
                            size = i;
                            break;
                        }
                    }
                    ret->resize(size);
                    return ret;
                }


                H5::DataSet dataset;
            };
        } // namespace dataset
        
    } // namespace hdf5
    
} // namespace po


#endif// __PO_HDF5_READ_ATTRIBUTE_H__