#ifndef __PO_HDF5_WRITE_ATTRIBUTE_H__
#define __PO_HDF5_WRITE_ATTRIBUTE_H__

#include <H5Cpp.h>
#include <string>
#include "poHdf5GetPredType.h"

#define _CRT_SECURE_NO_WARNINGS

namespace po
{
    namespace hdf5
    {
        namespace group
        {
            struct writeAttribute
            {
                /// @brief constractor of this class
                /// @param _group hdf5 group
                writeAttribute(H5::Group _group):group(_group){}

                /// @brief write attribute to group.
                /// @tparam T type of data
                /// @param name name of attribute
                /// @param data scalar data to write.
                template<typename T>
                void operator()(std::string name ,const T& data){
                    auto att = group.createAttribute(name.c_str(),po::hdf5::getPredType<int>::getDataType() , H5S_SCALAR);
                    att.write(po::hdf5::getPredType<int>::getDataType(),&data);
                    return;
                }

                /// @brief write string to group.
                /// @param name name of attribute
                /// @param data data to write
                template<>
                void operator()<std::string>(std::string name ,const std::string& data){
                    H5::StrType datatype(po::hdf5::getPredType<std::string>::get(),data.size());
                    datatype.setOrder(H5T_ORDER_LE);
                    auto att = group.createAttribute(name.c_str(),datatype , H5S_SCALAR);
                    att.write(datatype,data.c_str());
                    return;
                }

                /// @brief write vector to group.
                /// @tparam T type of vector
                /// @param name name of attribute
                /// @param data vector data to write.
                template<typename T>
                void operator()(std::string name, const std::vector<T>& data)
                {
                    int rank = 1;
                    hsize_t* dims = new hsize_t[1];
                    dims[0] = data.size();
                    H5::DataSpace space(rank,dims);
                    auto att = group.createAttribute(name.c_str(),getPredType<T>::getDataType(),space);
                    att.write(getPredType<T>::getDataType(),data.data());
                    return;
                }


                /// @brief write string vector to gropu attribute
                /// @param name name of attribute
                /// @param data string vector.
                template<>
                void operator()<std::string>(std::string name, const std::vector<std::string>& data)
                {
                    int rank = 1;
                    std::vector<size_t> dims(1);
                    dims[0] = data.size();
                    H5::DataSpace space(rank,dims.data());
                    H5::StrType datatype(po::hdf5::getPredType<std::string>::get(),H5T_VARIABLE);
                    datatype.setOrder(H5T_ORDER_LE);
                    auto att = group.createAttribute(name.c_str(),datatype,space);
                    char** temp = new  char*[data.size()];
                    for(int i =0 ; i < data.size();i++){
                        temp[i] = new char[data[i].size()];
                        std::strcpy(temp[i],data[i].c_str());
                    }
                    att.write(datatype,temp);
                }

                ~writeAttribute(){group.close();}

                H5::Group group;
            };
        } // namespace group
        namespace dataset
        {
            struct writeAttribute
            {
                H5::DataSet dataset;

                /// @brief constractor of this struct
                /// @param _dataset hdf5 data set
                writeAttribute(H5::DataSet _dataset):dataset(_dataset){}

                /// @brief destractor of this struct. dataset struct close.
                ~writeAttribute(){dataset.close();}


                /// @brief writing attribute to data set.
                /// @tparam T scalar value
                /// @param name name of attribute
                /// @param data data to write
                template<typename T>
                void operator()(std::string name, const T& data)
                {
                    auto att = dataset.createAttribute(name.c_str(),po::hdf5::getPredType<T>::getDataType(),H5S_SCALAR);
                    att.write(po::hdf5::getPredType<T>::getDataType(),&data);
                }


                /// @brief writing attribute to dataset(writing data is string case).
                /// @param name name of attribute
                /// @param data data to write(string)
                template<>
                void operator()<std::string>(std::string name, const std::string& data){
                    H5::StrType datatype(po::hdf5::getPredType<std::string>::get(),data.size());
                    auto att = dataset.createAttribute(name.c_str(),datatype,H5S_SCALAR);
                    att.write(datatype,data.c_str());
                }

                /// @brief writing attribute to dataset(vector case)
                /// @tparam T type of vector
                /// @param name name of attribute
                /// @param data vector data.
                template<typename T>
                void operator()(std::string name, const std::vector<T>& data){
                    int rank = 1;
                    hsize_t* dims = new hsize_t[1];
                    dims[0] = data.size();
                    H5::DataSpace space(rank,dims);
                    auto att = dataset.createAttribute(name.c_str(),hdf5::getPredType<T>::getDataType(),space);
                    att.write(hdf5::getPredType<T>::getDataType(),data.data());
                }

            };


        } // namespace dataset
    } // namespace hdf5struct writeAttribute{
} // namespace po

#endif// __PO_HDF5_WRITE_ATTRIBUTE_H__