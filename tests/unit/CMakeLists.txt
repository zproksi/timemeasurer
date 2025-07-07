cmake_minimum_required(VERSION 3.19)
set(library_under_test timemeasurer)
set(pname test${library_under_test})
project(${pname})

# Set C++ standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

# Set the build type to Debug if not explicitly specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Initial COMPILER_FLAGS:" ${CMAKE_CXX_FLAGS})

set(CMAKE_CXX_FLAGS "")
message(STATUS "CMAKE_CXX_FLAGS:" ${CMAKE_CXX_FLAGS})
message(STATUS "CMAKE_CXX_COMPILER_ID:" ${CMAKE_CXX_COMPILER_ID})
set(CMAKE_EXE_LINKER_FLAGS "")


option(BITNESS64 "Build in 64-bit mode" ON)
if(BITNESS64)
  # 64-bit mode
  set(bitnesswin "")    # MSVC/Windows
  set(bitnesslinux "")        # Linux
else()
  # 32-bit mode
  set(bitnesswin "/arch:IA32")  # MSVC/Windows
  set(bitnesslinux "-m32")    # Linux (GCC/Clang need the -m32 flag for 32-bit builds)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # GCC (GNU Compiler)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -D_DEBUG -g -D_CONSOLE -Wno-unknown-pragmas -Wall -Werror -fno-rtti -pthread ${bitnesslinux}")
        set(CMAKE_EXE_LINKER_FLAGS "-g")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}         -DNDEBUG    -D_CONSOLE -Wno-unknown-pragmas -Wall -Werror -fno-rtti -pthread ${bitnesslinux}")
        set(CMAKE_EXE_LINKER_FLAGS "-O3 -flto")
    else()
        message(FATAL_ERROR "Unsupported compilation mode")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*")  #Clang or AppleClang or anything with Clang
    # clang (MAC Compiler)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -D_DEBUG -g -D_CONSOLE -Wno-unknown-pragmas -Wall -Werror -Wno-unqualified-std-cast-call -fno-rtti -pthread -stdlib=libc++ ${bitnesslinux}")
        set(CMAKE_EXE_LINKER_FLAGS "-g")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}         -DNDEBUG    -D_CONSOLE -Wno-unknown-pragmas -Wall -Werror -Wno-unqualified-std-cast-call -fno-rtti -pthread -stdlib=libc++ ${bitnesslinux}")
        set(CMAKE_EXE_LINKER_FLAGS "-O3 -flto")
    else()
        message(FATAL_ERROR "Unsupported compilation mode")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # MSVC (Microsoft Visual C++)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS /D_CONSOLE /DDEBUG /D_DEBUG      /Zi /EHsc /W4 /WX /GR- /MDd ${bitnesswin}")
        set(CMAKE_EXE_LINKER_FLAGS "/DEBUG:FULL /INCREMENTAL")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS /D_CONSOLE         /DNDEBUG /Ob1     /EHsc /W4 /WX /GR- /MD  ${bitnesswin}")
        set(CMAKE_EXE_LINKER_FLAGS "            /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG")
    else()
        message(FATAL_ERROR "Unsupported compilation mode")
    endif()
else()
    message(FATAL_ERROR "Unsupported compiler")
endif()

message(STATUS "CMAKE_CXX_FLAGS:" ${CMAKE_CXX_FLAGS})

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.12.1
)

# Make the googletest content available
FetchContent_MakeAvailable(googletest)


# folder with library
#target_include_directories(${pname} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../)
#include_directories(../../)

# Source files
set(SOURCE_FILES
    test_${library_under_test}.cpp
)
#set(HEADER_FILES
#)

# Define the executable target
add_executable(${pname} ${SOURCE_FILES})

# Set the name of the executable based on the platform
if(WIN32)
    set_target_properties(${pname} PROPERTIES OUTPUT_NAME "${pname}")
else()
    set_target_properties(${pname} PROPERTIES OUTPUT_NAME "${pname}")
endif()

# Configuration-specific settings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # For GCC and Clang, use the -m64 flag to target 64-bit
set_target_properties(${pname} PROPERTIES
    COMPILE_OPTIONS "$<$<CONFIG:Release>:-O3>"
    COMPILE_OPTIONS "$<$<CONFIG:Debug>:-O0>"
)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # For MSVC, set the generator platform to x64
set_target_properties(${pname} PROPERTIES
    COMPILE_OPTIONS "$<$<CONFIG:Release>:/Ox>"
    COMPILE_OPTIONS "$<$<CONFIG:Debug>:/Od>"
)
else()
    message(FATAL_ERROR "Unsupported compiler")
endif()


target_include_directories(${pname} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../ build${library_under_test})

target_link_libraries(${pname} PRIVATE ${library_under_test} gtest_main gmock_main)

# Console commands for building on Windows with Visual Studio
# !!!!!!!!!!!!!
#    Use these commands in the Visual Studio Command Prompt
# !!!!!!!!!!!!!
# Navigate to the directory containing CMakeLists.txt
#   create folder 'build' and enter it
#  For visual studio 2022 option for cmake is "Visual Studio 17 2022"
#       for 32 bits compilation use:
#  cmake -G "Visual Studio 17 2022" -A Win32 ..
#       for 64 bits compilation use:
#  cmake -G "Visual Studio 17 2022" -A x64 ..
#       and execute to build
#    Release:
# cmake -DCMAKE_BUILD_TYPE=Release ..
# cmake --build . --config Release
#    Debug:
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake --build . --config Debug
#

# Console commands for building on Linux with GCC
# Use these commands in the terminal
# Navigate to the directory containing CMakeLists.txt
# mkdir build
# cd build
#   to build Release
# cmake -DCMAKE_BUILD_TYPE=Release ..
# cmake --build . --config Release
#   to build Debug
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake --build . --config Debug
#
# as a result folder should contain version of the application
# to generate 32 bits version of linux application add -m32 flag
# to CMAKE_CXX_FLAGS above in "GNU|Clang" section
