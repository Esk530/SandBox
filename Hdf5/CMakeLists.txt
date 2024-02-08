cmake_minimum_required(VERSION 3.0)
project(SandBox LANGUAGES CXX)

add_executable(Hdf5Test hdf5_test.cpp)
find_package(HDF5 REQUIRED COMPONENTS CXX)
target_include_directories(Hdf5Test PRIVATE ${HDF5_INCLUDE_DIR})
target_link_libraries(Hdf5Test PRIVATE HDF5::HDF5)
target_compile_definitions(Hdf5Test PRIVATE -D HAVE_HDF5)