cmake_minimum_required(VERSION 3.25)
project(128_bit_AES_encryptor_and_decryptor_in_C__)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
set (CMAKE_EXE_LINKER_FLAGS  "-static")
add_executable(128_bit_AES_encryptor_and_decryptor_in_C__
        decoding.h
        encoding.h
        key_expand.h
        lookup_table_decoding.h
        lookup_table_encoding.h
        main.cpp)
