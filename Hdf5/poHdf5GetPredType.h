/**
 * @file poHdf5GetPredType.h
 * @brief hdf5 �Ō^��������data type ��Ԃ��\���̂��쐬�����B
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
         * @brief �e���v���[�g���^�v���O�������g�p���āAHDF5�̃f�[�^�^���擾����\����
         * @tparam T �f�[�^�^
         */
        template<typename T>
        struct getPredType{};

        /**
         * @brief float�^�ɑ΂���HDF5�̃f�[�^�^���擾������ꉻ
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
         * @brief double�^�ɑ΂���HDF5�̃f�[�^�^���擾������ꉻ
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
         * @brief int�^�ɑ΂���HDF5�̃f�[�^�^���擾������ꉻ
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
         * @brief unsigned int �^�ɑ΂���HDF5�̃f�[�^�^���擾������ꉻ
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
         * @brief size_t�^�ɑ΂���HDF5�̃f�[�^�^���擾������ꉻ
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
         * @brief string�^�ɑ΂���HDF5�̃f�[�^�^���擾������ꉻ
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