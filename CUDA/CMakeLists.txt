cmake_minimum_required(VERSION 3.0)
enable_language(CUDA)
project(SandBox LANGUAGES CUDA)
add_executable(cudaAddArray cudaAddArray.cu)
target_compile_options(cudaAddArray PRIVATE -expt-extended-lambda)

