/**
 * @file poHdf5GetPredType.h
 * @brief hdf5 で型を引数にdata type を返す構造体を作成した。
 */

#ifndef __HDF5_GET_PRED_TYPE_H__
#define __HDF5_GET_PRED_TYPE_H__
#include <H5Cpp.h>
#include <string>

namespace po
{
    namespace hdf5
    {

        /**
         * @brief テンプレートメタプログラムを使用して、HDF5のデータ型を取得する構造体
         * @tparam T データ型
         */
        template<typename T>
        struct getPredType{};

        /**
         * @brief float型に対するHDF5のデータ型を取得する特殊化
         */
        template<>
        struct getPredType<float>{
            static  H5::PredType get(){return H5::PredType::NATIVE_FLOAT;}
            static  H5::FloatType getDataType(){
                H5::FloatType datatype(get());
                datatype.setOrder(H5T_ORDER_LE);
                return datatype;
            }
        };

        /**
         * @brief double型に対するHDF5のデータ型を取得する特殊化
         */
        template<>
        struct getPredType<double>{
            static  H5::PredType get(){return H5::PredType::NATIVE_DOUBLE;}
            static  H5::FloatType getDataType(){
                H5::FloatType datatype(get());
                datatype.setOrder(H5T_ORDER_LE);
                return datatype;
            }
        };

        /**
         * @brief int型に対するHDF5のデータ型を取得する特殊化
         */
        template<>
        struct getPredType<int>{
            static  H5::PredType get(){return H5::PredType::NATIVE_INT;}
            static  H5::IntType getDataType(){
                H5::IntType datatype(get());
                datatype.setOrder(H5T_ORDER_LE);
                return datatype;
            }
        };

        /**
         * @brief unsigned int 型に対するHDF5のデータ型を取得する特殊化
         */
        template<>
        struct getPredType<unsigned int>{
            static  H5::PredType get(){return H5::PredType::NATIVE_UINT32;}
            static  H5::IntType getDataType(){
                H5::IntType datatype(get());
                datatype.setOrder(H5T_ORDER_LE);
                return datatype;
            }
        };

        /**
         * @brief size_t型に対するHDF5のデータ型を取得する特殊化
        */
        template<>
        struct getPredType<size_t>{
            static  H5::PredType get(){return H5::PredType::NATIVE_UINT64;}
            static  H5::IntType getDataType(){
                H5::IntType datatype(get());
                datatype.setOrder(H5T_ORDER_LE);
                return datatype;
            }
        };

        /**
         * @brief string型に対するHDF5のデータ型を取得する特殊化
         */
        template<>
        struct getPredType<std::string>{
            static  H5::PredType get(){return H5::PredType::C_S1;}
            static  H5::StrType getDataType(){
                H5::StrType datatype(get(),128);
                datatype.setOrder(H5T_ORDER_LE);
                return datatype;
            }
        };
    } // namespace hdf5
} // namespace po


#endif// __HDF5_GET_PRED_TYPE_H__