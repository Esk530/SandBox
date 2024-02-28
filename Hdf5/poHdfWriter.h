#ifndef __PO_HDF5_WRITER_H__
#define __PO_HDF5_WRITER_H__
#include <H5Cpp.h>
#include "poHdf5GetPredType.h"

namespace po
{
    namespace hdf5
    {
        /// @brief writing hdf5 function base
        struct writeHdf5{
            H5::Group group; ///Hdf5 group

            /// @brief constracto of struct
            /// @param _group hdf5 group
            writeHdf5(H5::Group _group):group(_group){}

            /// @brief destractor -- close parent group.
            ~writeHdf5(){group.close();}


            /// @brief write sclar data.
            /// @tparam T scalar type
            /// @param name name of data
            /// @param data scalar data
            template<typename T>
            void  operator()(std::string name ,const T& data);

            /// @brief write sclar string.
            /// @tparam T scalar type
            /// @param name name of data
            /// @param data scalar data
            template<>
            void operator()<std::string>(std::string name, const std::string& value);


            /// @brief wrining function
            /// @tparam T type of vector
            /// @param name name of data struct
            /// @param data data written in std::vector
            template<typename T>
            void operator()(std::string name, const std::vector<T>& data);

            /// @brief Specialization wrinig vector function. This is used to write string vector
            /// @param name name of data struct
            /// @param data sting vector
            template<>
            void operator()<std::string>(std::string name, const std::vector<std::string>& data);

            /// @brief Writing function with dimensions ; set to {N,3} if 3D vector array write  
            /// @tparam T type fo vector
            /// @tparam Interger integer -- size_t, int, unsigned int,...
            /// @param name name of struct
            /// @param vector data vector
            /// @param dims dimensions to write.
            template<typename T,typename Interger>
            void operator()(std::string name, const std::vector<T>& vector, std::vector<Interger> dims);

        };

        template<typename T>
        void  writeHdf5::operator()(std::string name ,const T& data){
            H5::DataSet dataset;
            if(!group.exists(name.c_str()))
                dataset = group.createDataSet(name.c_str(), po::hdf5::getPredType<T>::getDataType() , H5S_SCALAR);
            else 
                dataset = group.openDataSet(name.c_str());
            dataset.write(&data, getPredType<T>::getDataType());
        }

        template<>
        void writeHdf5::operator()<std::string>(std::string name, const std::string& value)
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

        template<typename T,typename Interger>
        void writeHdf5::operator()(std::string name, const std::vector<T>& vector, std::vector<Interger> dims)
        {
            hsize_t rank = dims.size();
            std::vector<hsize_t> dimensions(3);
            dimensions[0] = dims[2];
            dimensions[1] = dims[1];
            dimensions[2] = dims[0];
            H5::DataSpace space(rank,dimensions.data());
            H5::DataSet dataset = group.createDataSet(name.c_str(),getPredType<T>::getDataType(),space);
            dataset.write(vector.data(),getPredType<T>::getDataType());
            return;
        }

        template<typename T>
        void writeHdf5::operator()(std::string name, const std::vector<T>& data)
        {
            int rank = 1;
            hsize_t* dims = new hsize_t[1];
            dims[0] = data.size();
            H5::DataSpace space(rank,dims);
            H5::DataSet dataset = group.createDataSet(name.c_str(),getPredType<T>::getDataType(),space);
            dataset.write(data.data(),getPredType<T>::getDataType());
            return;
        }

        template<>
        void writeHdf5::operator()<std::string>(std::string name, const std::vector<std::string>& data)
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




        

        struct extendDataWriter
        {
            H5::Group group;
            extendDataWriter(H5::Group _group):group(_group){}
            template<typename T>
            void operator()(std::string dataName , T value, size_t maxSize = 1000)
            {
                if(!group.exists(dataName.c_str())){
                    auto dataSet = this->createExtendDataSet1D<T>(group, dataName, maxSize);
                    addExtendDataSet1D(dataSet,value);
                }else{
                    auto dataset = group.openDataSet(dataName.c_str());
                    addExtendDataSet1D(dataset,value);
                }
            }
            
            
            /**
             * @brief 拡張可能な1次元のHDF5データセットに新しい要素を追加します。
             *
             * この関数は、指定されたHDF5データセットに新しい要素を追加し、拡張後のデータセットのサイズを返します。
             * 新しい要素は、指定された値 `value` として提供され、データセットが拡張されます。
             *
             * @tparam T データセットの要素の型。
             * @param dataset 要素が追加されるHDF5データセット。
             * @param value 新しい要素の値。
             * @return 拡張後のデータセットのサイズ。
             *
             * 使用例:
             * @code
             * H5::DataSet myDataset; // あなたのHDF5データセット
             * float newValue = 42.0;
             * size_t newSize = po::hdf5::addExtendDataSet1D<float>(myDataset, newValue);
             * @endcode
             */
            template<typename T>
            size_t addExtendDataSet1D(H5::DataSet dataset, T value)
            {
                // データセットのデータスペース
                auto space = dataset.getSpace();

                // データセットの次元数
                size_t N = space.getSimpleExtentNdims();

                // 現在のデータセットのサイズと最大サイズの取得
                size_t *dims = new size_t[N];
                size_t *max_dims = new size_t[N];
                space.getSimpleExtentDims(dims, max_dims);

                // データセットの拡張前のサイズを取得
                hsize_t offset[] = {dims[0]};

                // データセットの拡張後のサイズ
                size_t dims_add[] = {1};
                hsize_t dims2[1];
                dims2[0] = {dims[0] + 1};
                dataset.extend(dims2);

                // データセットのデータスペースを更新
                space = dataset.getSpace();
                space.getSimpleExtentDims(dims, max_dims);

                // データセットの拡張後のデータスペース
                H5::DataSpace maxSpace = H5::DataSpace(1, dims, max_dims);

                // 新しい要素を書き込むためのデータスペース
                H5::DataSpace memspace(N, dims_add);
                
                // データ型によって処理を分岐
                if constexpr (!std::is_same<T, std::string>::value)
                {
                    // 文字列でない場合、T型の新しい要素を書き込み
                    T inp[1] = {value};
                    dataset.write(inp, po::hdf5::getPredType<T>::getDataType(), memspace, space);
                }
                else
                {
                    // 文字列の場合、可変長文字列型の新しい要素を書き込み
                    const char *inp[1] = {value.c_str()};
                    H5::StrType datatype(po::hdf5::getPredType<std::string>::get(), H5T_VARIABLE);
                    datatype.setOrder(H5T_ORDER_LE);
                    dataset.write(inp, datatype, memspace, space);
                }

                // 拡張後のデータセットのサイズを返す
                return dims[0];
            }

            /**
             * @brief 拡張可能な1次元のHDF5データセットを作成します。
             *
             * この関数は、指定されたHDF5グループに拡張可能な1次元のデータセットを作成し、そのデータセットを返します。
             * データセットは、指定されたデータ型 `T` および最大サイズ `maxSize1` で構成されます。
             * データセットはチャンクとして設定され、拡張可能な機能が有効になります。
             *
             * @tparam T データセットの要素の型。
             * @param group データセットを作成するHDF5グループ。
             * @param name データセットの名前。
             * @param maxSize1 データセットの最大サイズ（初期サイズは0）。
             * @return 作成されたHDF5データセット。
             *
             * 使用例:
             * @code
             * H5::Group myGroup; // あなたのHDF5グループ
             * H5::DataSet myDataset = po::hdf5::createExtendDataSet1D<float>(myGroup, "myFloatDataset", 1000);
             * @endcode
             */
            template<typename T>
            H5::DataSet createExtendDataSet1D(H5::Group group, std::string name, size_t maxSize1)
            {
                // データセットの次元（初期サイズは0）
                hsize_t dims[1] = {0};

                // データセットの最大サイズ
                hsize_t maxdims[1] = {maxSize1};

                // HDF5データスペースの作成（初期サイズおよび最大サイズを指定）
                H5::DataSpace maxSpace = H5::DataSpace(1, dims, maxdims);

                // データセット作成時のプロパティリストの作成
                H5::DSetCreatPropList prop;

                // データセットのチャンクサイズの指定（1000要素）
                hsize_t chunk_dims[1] = {1000};
                prop.setChunk(1, chunk_dims);

                // データ型によって処理を分岐
                if constexpr(!std::is_same<T, std::string>::value)
                {
                    // 文字列でない場合、T型のデータセットを作成
                    return group.createDataSet(name.c_str(), po::hdf5::getPredType<T>::getDataType(), maxSpace, prop);
                }
                else
                {
                    // 文字列の場合、可変長文字列型のデータセットを作成
                    H5::StrType datatype(po::hdf5::getPredType<std::string>::get(), H5T_VARIABLE);
                    datatype.setOrder(H5T_ORDER_LE);
                    return group.createDataSet(name.c_str(), datatype, maxSpace, prop);
                }
            }


        };


    } // namespace hdf5
} // namespace po


#endif// __PO_HDF5_WRITER_H__
