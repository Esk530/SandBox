#ifndef __PO_HDF5_WRITE_TIME_INDEX_H__
#define __PO_HDF5_WRITE_TIME_INDEX_H__
#include <string>
#include <sstream>
namespace po
{
    namespace hdf5
    {
        struct getTimeIndex
        {
            getTimeIndex(){}
            getTimeIndex(int _keta){keta = _keta;}
            std::string operator()(int itime)const{
                int temp = itime;
                std::stringstream ss;

                ss << "Time";
                for(int i = 0; i < keta; i++){
                    int ttt = static_cast<int>(std::pow(10,keta-i-1));
                    ss << temp/ttt;
                    temp = temp%ttt;
                }
                return ss.str();
            }
            int keta = 5;
        };
        
    } // namespace hdf5
} // namespace po

#endif// __PO_HDF5_WRITE_TIME_INDEX_H__