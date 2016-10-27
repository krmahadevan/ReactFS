cmake_minimum_required(VERSION 3.1)


project(rfs-core VERSION 0.1.0)

if (${APPLE})
    set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk)
    INCLUDE_DIRECTORIES(/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers/)
endif ()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE(../cmake/CMakeInclude.txt)

LINK_DIRECTORIES(../common/cmake ../watergate/cmake)

set(CORE_LIB_NAME ${PROJECT_NAME})

file(GLOB_RECURSE RFS_C_SOURCES "src/*.cpp")
file(GLOB_RECURSE RFS_C_INCLUDES "includes/*.h")

list(REMOVE_DUPLICATES RFS_C_SOURCES)

MESSAGE("SOURCE FILES : [" ${RFS_C_SOURCES} "]")

add_library(${CORE_LIB_NAME} SHARED ${RFS_C_INCLUDES} ${RFS_C_SOURCES})
target_link_libraries(${CORE_LIB_NAME} rfs-common)

if (${APPLE})
    MESSAGE("Host platform: MAC OS X")
    set(RFS_C_LIBRARIES "rfs-common")
elseif (${UNIX})
    MESSAGE("Host platform:  Other UNIX")
    set(RFS_C_LIBRARIES "pthread uuid rt rfs-common")
endif ()


MESSAGE("Using libraries : #" ${RFS_C_LIBRARIES} "#")

add_executable(main_fsb adhoc/main_fsb.cpp)
target_link_libraries(main_fsb rfs-core ${WG_LIBRARIES})
