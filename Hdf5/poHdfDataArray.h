#ifndef __PO_HDF5_DATA_ARRAY_H__
#define __PO_HDF5_DATA_ARRAY_H__
#include <vector>
#include <type_traits>
#include "poHdf5GetPredType.h"
#include "poHdf5WriteAttribute.h"

namespace po
{
    namespace hdf5
    {


        template<typename T>
        class dataArray{
        public:
            /// @brief default constractor
            dataArray(){}

            dataArray(int N){
                m_size = N;
                m_dimensions.resize(1,N);
                m_data = std::make_shared<std::vector<T>>(N);
                return;
            }

            void setSize(int N){
                m_size = N;
                m_dimensions.resize(1,N);
                m_data = std::make_shared<std::vector<T>>(N);
                return;
            }

            /// @brief 
            /// @return 
            decltype(auto) begin(){return m_data->begin();}

            /// @brief 
            /// @return 
            decltype(auto) end(){return m_data->end();}

            /// @brief 
            /// @return 
            decltype(auto) cbegin()const{return m_data->cbegin();}

            /// @brief 
            /// @return 
            decltype(auto) cend()const{return m_data->cend();}

            /// @brief setting dimensions
            /// @param dim dimensions of data
            void setDimensions(std::vector<int> dim){
                m_dimensions.resize(dim.size());
                std::copy(dim.begin(),dim.end(),m_dimensions.begin());
            }

            /// @brief setting description of data
            /// @param desc descriptin of data
            void setDescription(std::string desc){m_description = desc;}

            /// @brief getting descriptiopn of data
            /// @return description
            std::string getDescription()const{return m_description;}

            /// @brief setting key of data
            /// @param key key of data
            void setKey(std::string key){m_key = key;}

            std::string getKey()const{return m_key;}


            void setToVector(){
                m_attributeType = "Vector";
            }



#ifdef HAVE_CUDA
            template<typename Iter>
            void set(Iter begin , Iter end){
                int N = thrust::distance(begin,end);
                m_data = std::make_shared<std::vector<T>>(N);
                thrust::copy(begin,end,m_data->begin());
                return;
            }
#endif//

            /// @brief Setting data and copy
            /// @param begin 
            /// @param end 
            template<typename Iter>
            void set(Iter begin, Iter end){
                m_data = std::make_shared<std::vector<T>>(begin,end);
            }

            /// @brief setting data pointer
            /// @param _data pointer of data.
            void set(std::shared_ptr<std::vector<T>> _data){m_data =_data;}

            void write(H5::Group group){

                /// setting data space
                hsize_t* dims = new hsize_t[1];
                dims[0] = m_size;
                H5::DataSpace space(1,dims);

                /// 
                H5::DataSet dataSet = group.createDataSet(m_key.c_str(),hdf5::getPredType<T>::getDataType(),space);

                // writing attribute.
                po::hdf5::dataset::writeAttribute writeAttiribute(dataSet);
                writeAttiribute("Description",m_description);
                writeAttiribute("AttributeType",m_attributeType);
                writeAttiribute("Center",m_center);
                writeAttiribute("NumberOfType",getTypeString());
                writeAttiribute("Precision",sizeof(T));
                writeAttiribute("Dimensions",m_dimensions);
                writeAttiribute("PATH",m_rootName);

                // write data
                dataSet.write(m_data->data(),hdf5::getPredType<T>::getDataType());
                return;
            }

            void setCenterToNode(){m_center = "Node";}
            void setCenterToCell(){m_center = "Cell";}

            void setRoot(std::string name){
                m_rootName = name + "/" + m_key;
            }

        private:
            int m_size; 
            std::shared_ptr<std::vector<T>> m_data;
            std::vector<int> m_dimensions;
            std::string m_description;
            std::string m_key;
            std::string m_attributeType = "Scalar";
            std::string m_center= "Cell";
            std::string m_rootName;
        private:

            std::string getTypeString(){
                if constexpr(std::is_integral<T>::value)return "Int";
                else if constexpr(std::is_floating_point<T>::value)return "Float";
            }

        };
    } // namespace hdf5
    
} // namespace po


#endif// __PO_HDF5_DATA_ARRAY_H__