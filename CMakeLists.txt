cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Orderbook)

add_executable(main 
    main.cpp
    src/HelperFunctions.cpp
    src/Orderbook.cpp
    src/OrderbookContainer.cpp
)

target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR}/src)

if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'Orderbook' as StartUp Project in Visual Studio.\n" )
    else()
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main )
    endif()
endif()