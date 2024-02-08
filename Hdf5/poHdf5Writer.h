/**
 * @file poHdf5Writer.h
 * @brief Function for write to hdf5 file format.
 */

#ifndef __HDF5_WRITE_DATA_H__
#define __HDF5_WRITE_DATA_H__

#include <H5Cpp.h>
#include <vector>
#include "poHdf5GetPredType.h"


namespace po
{
    namespace hdf5
    {
        /**
         * @brief This is function for writing hdf5.
         */
        struct writer{
            H5::Group group; ///Hdf5 group

            /**
             * @brief Constractor
             * @param _group group for writing
             */
            writer(H5::Group _group):group(_group){}

            ~writer(){group.close();}

            /**
             * @brief writing function for std::vector
             * @tparam T pa
             * @param name data group name
             * @param data data vector
             */
            template<typename T>
            void operator()(std::string name, const std::vector<T>& data)
            {
                hsize_t rank = 1;
                hsize_t* dims = new hsize_t[1];
                dims[0] = data.size();
                H5::DataSpace space(rank,dims);
                H5::DataSet dataset = group.createDataSet(name.c_str(),getPredType<T>::getDataType(),space);
                dataset.write(data.data(),getPredType<T>::getDataType());
                return;
            }

            template<typename T>
            void operator()(std::string name, const T* data, int size)
            {
                hsize_t rank = 1;
                hsize_t* dims = new hsize_t[1];
                dims[0] = size;
                H5::DataSpace space(rank,dims);
                H5::DataSet dataset = group.createDataSet(name.c_str(),getPredType<T>::getDataType(),space);
                dataset.write(data,getPredType<T>::getDataType());
                return;
            }

            /**
             * @brief writing function for string
             * @param name name of data
             * @param data data array
             */
            template<>
            void operator()<std::string>(std::string name, const std::vector<std::string>& data)
            {
                int rank = 1;
                std::vector<size_t> dims(1);
                dims[0] = data.size();
                H5::DataSpace space(rank,dims.data());
                H5::StrType datatype(po::hdf5::getPredType<std::string>::get(),H5T_VARIABLE);
                datatype.setOrder(H5T_ORDER_LE);
                H5::DataSet dataset = group.createDataSet(name.c_str(),datatype,space);
                char** temp = new  char*[data.size()];
                for(int i =0 ; i < data.size();i++){
                    temp[i] = new char[data[i].size()];
                    std::strcpy(temp[i],data[i].c_str());
                }
                dataset.write(temp,datatype);
            }

            /**
             * @brief writing vector with dimensions
             * @tparam T type of data vector
             * @param name  name of array
             * @param vector data array vector
             * @param dims dimensions of this data.
             */
            template<typename T>
            void operator()(std::string name, const std::vector<T>& vector, std::vector<size_t> dims)
            {
                hsize_t rank = dims.size();
                H5::DataSpace space(rank,dims.data());
                H5::DataSet dataset = group.createDataSet(name.c_str(),getPredType<T>::getDataType(),space);
                dataset.write(vector.data(),getPredType<T>::getDataType());
                return;
            }

            /**
             * @brief writing scalar function
             * @tparam T type of scalar
             * @param name name of scalar
             * @param data scalar data.
             */
            template<typename T>
            void  operator()(std::string name ,const T& data){
                H5::DataSet dataset;
                if(!group.exists(name.c_str()))
                    dataset = group.createDataSet(name.c_str(), po::hdf5::getPredType<T>::getDataType() , H5S_SCALAR);
                else 
                    dataset = group.openDataSet(name.c_str());
                dataset.write(&data, getPredType<T>::getDataType());
            }

            /**
             * @brief writing string function
             * @param name name of string
             * @param value string
             */
            template<>
            void operator()<std::string>(std::string name, const std::string& value)
            {
                H5::StrType datatype(po::hdf5::getPredType<std::string>::get(),value.size());
                datatype.setOrder(H5T_ORDER_LE);
                H5::DataSet dataset;
                if(!group.exists(name.c_str()))
                    dataset = group.createDataSet(name.c_str(), datatype, H5S_SCALAR);
                else
                    dataset = group.openDataSet(name.c_str());
                dataset.write(value.c_str(), datatype);
            }

            template<typename T>
            void operator()(std::string name, typename std::vector<T>::iterator begin, typename std::vector<T>::iterator end)
            {
                int N  = std::distance(begin,end);
                std::vector<T> temp(N);
                boost::compute::copy(begin,end,temp.begin());
                this->operator()(name,temp);
                return;
            }

        };
    } // namespace hdf5
} // namespace po

#endif// __HDF5_WRITE_DATA_H__
