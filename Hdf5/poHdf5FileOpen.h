#ifndef __PO_HDF5_FILE_OPEN_H__
#define __PO_HDF5_FILE_OPEN_H__
#include <H5Cpp.h>
#include <string>
#include <sstream>
#include <filesystem>

namespace po
{
    namespace hdf5
    {
        class fileOpen{
        private:
            enum fileOpenMode{
                write = 0,
                readWrite,
                readOnly
            };
        public:

            /// @brief Default constractor
            fileOpen(){}

            /// @brief constractor of fileOpen
            /// @param _filePath hdf5 file Path
            fileOpen(std::string _filePath):filePath(_filePath){}

            /// @brief setting hdf5 file path
            /// @param path hdf5 file path
            void setFilePath(std::string path){filePath = path;}

            /// @brief setting file open new file mode
            void setToWrite(){mode = write;}

            /// @brief setting file open mode to read only
            void setToReadOnly(){mode = readOnly;}

            /// @brief setting file open mode to read write.
            void setToReadWrite(){mode = readWrite;}

            /// @brief setting root group name
            /// @param name name of root group
            void setRootGroupName(std::string name){
                rootGroupName = name;
            }

            /// @brief file Open
            H5::Group open(int& ierr){
                if(mode == write){
                    H5::H5File file = H5::H5File(filePath.c_str(),H5F_ACC_TRUNC);
                    ierr = 0;
                    return file.createGroup(rootGroupName.c_str());
                }else{
                    // file check. c++17 later
                    if(this->IsFileExist(filePath)){
                        H5::H5File file;
                        if(mode == readWrite)
                        {
                            file = H5::H5File(filePath.c_str(),H5F_ACC_RDWR);
                        }else{
                            file = H5::H5File(filePath.c_str(),H5F_ACC_RDONLY);
                        }
                        ierr = 0;
                        return file.openGroup(rootGroupName.c_str());
                    }else{
                        ierr = -1;
                    }
                }
                return H5::Group();
            }

            /// @brief getting error stirng from error code
            /// @param ierr error code
            /// @return error string.
            std::string getErrorString(int ierr)const{
                if(ierr == -1){
                    return "File Open Error : Cannot find hdf5 file when using read option.";
                }
                std::stringstream ss;
                ss << "Code : " << ierr << std::endl;
                return ss.str();
            }

        private:
            /// file open mode
            fileOpenMode mode = write;
            /// file path
            std::string filePath;
            /// root group name
            std::string rootGroupName = "Root";

        private:
            bool IsFileExist(const std::string& name) {
                return std::filesystem::is_regular_file(name);
            }
        };
    } // namespace hdf5
} // namespace po


#endif// __PO_HDF5_FILE_OPEN_H__
