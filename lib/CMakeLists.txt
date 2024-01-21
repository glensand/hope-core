cmake_minimum_required(VERSION 3.11)

project(hope-core)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB HOPE_HEADERS
    hope_core/fsm*.h
    hope_core/switch_expression*.h
    hope_core/tuple*.h
    hope_core/type_traits*.h
)

add_library(hope-core INTERFACE ${HOPE_HEADERS})
target_include_directories(hope-core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_INSTALL_DIRECTORY ${CMAKE_BINARY_DIR})
INSTALL(DIRECTORY hope_core/ DESTINATION ${CMAKE_INSTALL_DIRECTORY}/include/hope_core
        FILES_MATCHING PATTERN "*.h")