cmake_minimum_required(VERSION 3.22)

find_package(Python3 3.4 REQUIRED)

project(
    hidusbgd
    VERSION 0.0.1
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include(FetchContent)
FetchContent_Declare(
    godotcpp
    GIT_REPOSITORY https://github.com/godotengine/godot-cpp.git
    GIT_TAG godot-4.5-stable
)
FetchContent_Declare(
    hidapi
    GIT_REPOSITORY https://github.com/libusb/hidapi.git
    GIT_TAG hidapi-0.15.0
)
FetchContent_MakeAvailable(godotcpp hidapi)

get_target_property(GODOTCPP_SUFFIX godot-cpp GODOTCPP_SUFFIX)

set(SAMPLE_TARGET_ADDON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sample/addons/hidusbgd)
set(CODE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/code)
set(DOC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc_classes)
file(GLOB TRANSLATION_UNIT_FILES ${CODE_DIR}/*.cpp)
file(GLOB HEADER_FILES ${CODE_DIR}/*.hpp)

set(TARGET_NAME ${PROJECT_NAME})

add_library(
    ${TARGET_NAME}
    SHARED
)

target_sources(
    ${TARGET_NAME}
    PRIVATE
    ${TRANSLATION_UNIT_FILES}
    ${HEADER_FILES}
)

target_link_libraries(
    ${TARGET_NAME}
    PRIVATE
    godot-cpp
    hidapi::hidapi
)

set_target_properties(
    ${TARGET_NAME}
    PROPERTIES
        CXX_EXTENSIONS OFF
        CXX_VISIBILITY_PRESET ${GODOTCPP_SYMBOL_VISIBILITY}

        POSITION_INDEPENDENT_CODE ON
        BUILD_RPATH_USE_ORIGIN ON

        # Try to ensure only static libraries are selected to be linked to.
        LINK_SEARCH_START_STATIC ON
        LINK_SEARCH_END_STATIC ON

        # NOTE: Wrapping the output variables inside a generator expression
        # prevents msvc generator from adding addition Config Directories
        LIBRARY_OUTPUT_DIRECTORY "$<1:${SAMPLE_TARGET_ADDON_DIR}>"
        RUNTIME_OUTPUT_DIRECTORY "$<1:${SAMPLE_TARGET_ADDON_DIR}>"
        PDB_OUTPUT_DIRECTORY     "$<1:${SAMPLE_TARGET_ADDON_DIR}>" #MSVC Only, ignored on other platforms

        PREFIX "lib"
        SUFFIX ${GODOTCPP_SUFFIX}
        OUTPUT_NAME ${TARGET_NAME}
)

if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    set(FRAMEWORK_DIR ${SAMPLE_TARGET_ADDON_DIR}/libhidusbgd.macos.${GODOTCPP_TARGET}.framework)
    set_target_properties(
        ${TARGET_NAME}
        PROPERTIES 
            SUFFIX ".${GODOTCPP_TARGET}" 
            LIBRARY_OUTPUT_DIRECTORY ${FRAMEWORK_DIR}
            RUNTIME_OUTPUT_DIRECTORY ${FRAMEWORK_DIR}
    )
endif()