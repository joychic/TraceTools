# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
project(Trace)
set(TARGET Trace)


# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

#execute_process(COMMAND rm ${CMAKE_CURRENT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/main/libs/${ANDROID_ABI}/)
find_package(shadowhook REQUIRED CONFIG)
find_package(xdl REQUIRED CONFIG)

aux_source_directory(cpp TRACE)
include_directories(
        ${TRACE}
)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
add_library( # Sets the name of the library.
        ${TARGET}


        # Sets the library as a shared library.
        SHARED
        ${TRACE}
        cpp/log.h
        cpp/Trace.h
        )


# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        ${TARGET}
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        shadowhook::shadowhook
        xdl::xdl)

