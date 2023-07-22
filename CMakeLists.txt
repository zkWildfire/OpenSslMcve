cmake_minimum_required(VERSION 3.23)
project(MCVE LANGUAGES CXX)
find_package(OpenSSL REQUIRED)

add_executable(MCVE source.cpp)
target_link_libraries(MCVE PRIVATE OpenSSL::SSL OpenSSL::Crypto)
install(TARGETS MCVE)
