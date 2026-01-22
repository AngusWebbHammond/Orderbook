cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Orderbook)

include_directories(${PROJECT_SOURCE_DIR}/assets)

add_executable(main 
    main.cpp
    src/HelperFunctions.cpp
    src/OrderbookContainer.cpp
    src/Parser.cpp
    src/MatchingEngine.cpp
    src/Order.cpp
)

target_include_directories(main PRIVATE ${PROJECT_SOURCE_DIR} )

# 
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    ${CMAKE_CURRENT_LIST_DIR}/assets 
    $<$<CONFIG:Debug>:${CMAKE_BINARY_DIR}/Debug/assets> 
    $<$<CONFIG:Release>:${CMAKE_BINARY_DIR}/Release/assets>
    $<$<CONFIG:MinSizeRel>:${CMAKE_BINARY_DIR}/Debug/assets> 
    $<$<CONFIG:RelWithDebInfo>:${CMAKE_BINARY_DIR}/Release/assets>
)
add_dependencies(main copy_assets)

if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'Orderbook' as StartUp Project in Visual Studio.\n" )
    else()
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main )
    endif()
endif()