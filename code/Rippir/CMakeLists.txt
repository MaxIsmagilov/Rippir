SET(Rippir_Pub_Headers
    pool.hpp
    )

SET(Rippir_Pri_Headers
    pool.hpp
    )

SET(Rippir_Implementation
    pool.cpp
    pool_impl.cpp
    )

ADD_LIBRARY(Rippir_Objects OBJECT 
    ${Rippir_Pub_Headers}  
    ${Rippir_Pri_Headers}  
    ${Rippir_Implementation}
    )


TARGET_COMPILE_FEATURES(Rippir_Objects
    PRIVATE cxx_std_20
    )


TARGET_INCLUDE_DIRECTORIES(Rippir_Objects
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
    )

ADD_LIBRARY(Rippir $<TARGET_OBJECTS:Rippir_Objects>)

ADD_LIBRARY(Rippir::Rippir ALIAS Rippir)

if (WIN32)
    SET_TARGET_PROPERTIES( Rippir
        PROPERTIES
        CXX_EXTENSIONS NO
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        COMPILE_OPTIONS "-Wall -Wextra -Werror"
        PUBLIC_HEADER "${Rippir_Pub_Headers}")
else ()
    SET_TARGET_PROPERTIES( Rippir
        PROPERTIES
        CXX_EXTENSIONS -lpthread 
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        COMPILE_OPTIONS "-Wall -Wextra -Werror"
        PUBLIC_HEADER "${Rippir_Pub_Headers}")
endif()

INCLUDE(GNUInstallDirs)

INSTALL(TARGETS Rippir Rippir_Objects
    EXPORT RippirTargets
    RUNTIME DESTINATION ${CMAKE_INSTAL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTAL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTAL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTAL_INCLUDEDIR}/Rippir
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTAL_INCLUDEDIR}/Rippir
    )

INSTALL( EXPORT RippirTargets
    FILE RippirTargets.cmake
    NAMESPACE Rippir::
    DESTINATION ${CMAKE_INSTAL_LIBDIR}/cmake/Rippir)
    
