cmake_minimum_required(VERSION 3.0)
project(FAVOR_PROJECT LANGUAGES CXX)

#HDF5 INCLUDE
function(HDF_SETTING EXE_NAME)
FIND_PACKAGE(HDF5 COMPONENTS CXX HL)
if(HDF5_FOUND)
    target_include_directories(${EXE_NAME} PRIVATE ${HDF5_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../Hdf5)
    target_compile_definitions(${EXE_NAME} PRIVATE -D HAVE_HDF5)
    target_link_libraries(${EXE_NAME} PRIVATE HDF5::HDF5)
    set_property(TARGET ${EXE_NAME} PROPERTY CXX_STANDARD 20) 
endif()
endfunction()


add_executable(WriteExample Sample/writeHdf5.cpp)
HDF_SETTING(WriteExample)

add_executable(WriteRectGrid Sample/writeRectGrid.cpp)
HDF_SETTING(WriteRectGrid)
