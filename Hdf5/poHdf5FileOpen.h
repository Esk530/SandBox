#ifndef __PO_HDF5_FILE_OPEN_H__
#define __PO_HDF5_FILE_OPEN_H__
#include <H5Cpp.h>
#include <string>
namespace po
{
    namespace hdf5
    {
        enum openFlag
        {
            write = 0,
            readWrite,
            readOnly
        };

        template<openFlag flag>
        struct fileOpen{
            fileOpen(std::string root):rootGroup(root){}
            H5::Group operator()(std::string filePath)
            {
                file = H5::H5File(filePath.c_str(),H5F_ACC_TRUNC);
                return file.createGroup(rootGroup.c_str());
            }
            ~fileOpen(){file.close();}
            H5::H5File file;
            std::string rootGroup;
        };
        template<>
        struct fileOpen<readWrite>{
            fileOpen(std::string root):rootGroup(root){}
            H5::Group operator()(std::string filePath)
            {
                file = H5::H5File(filePath.c_str(),H5F_ACC_RDWR);
                return file.openGroup(rootGroup.c_str());
            }
            ~fileOpen(){file.close();}
            H5::H5File file;
            std::string rootGroup;
        };

        template<>
        struct fileOpen<readOnly>{
            fileOpen(std::string root):rootGroup(root){}
            H5::Group operator()(std::string filePath)
            {
                file = H5::H5File(filePath.c_str(),H5F_ACC_RDONLY);
                return file.openGroup(rootGroup.c_str());
            }
            ~fileOpen(){file.close();}
            H5::H5File file;
            std::string rootGroup;
        };
    }
} // namespace po
#endif// __PO_HDF5_FILE_OPEN_H__