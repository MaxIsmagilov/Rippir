cmake_minimum_required(VERSION 3.25)

PROJECT( Rippir
    VERSION 0.0.0
    LANGUAGES C CXX
    )

ADD_SUBDIRECTORY(Rippir)

if(NOT WIN32)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-lpthread" COMPILER_SUPPORTS_THREADS)

    if (COMPILER_SUPPORTS_THREADS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
    else ()
        message(FATAL_ERROR "Your compiler does not support C++20 concepts.")
    endif ()
endif ()
