cmake_minimum_required(VERSION 3.22)

project(EhlJuceDesign VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(EHL_JUCE_DESIGN_BUILD_TESTS "Build EHL JUCE design module tests" ${PROJECT_IS_TOP_LEVEL})
set(EHL_JUCE_SOURCE_DIR "" CACHE PATH "Existing JUCE source checkout")

if(NOT TARGET juce::juce_gui_basics)
    include(FetchContent)
    if(EHL_JUCE_SOURCE_DIR)
        if(NOT EXISTS "${EHL_JUCE_SOURCE_DIR}/CMakeLists.txt")
            message(FATAL_ERROR "EHL_JUCE_SOURCE_DIR does not contain JUCE: ${EHL_JUCE_SOURCE_DIR}")
        endif()
        add_subdirectory("${EHL_JUCE_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/juce")
    else()
        FetchContent_Declare(
            JUCE
            GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
            GIT_TAG 91ad83ae34a81e0833b1a2b0866f54846370ae53
            GIT_SHALLOW FALSE)
        FetchContent_MakeAvailable(JUCE)
    endif()
endif()

add_library(ehl_juce_design INTERFACE)
add_library(EHL::JuceDesign ALIAS ehl_juce_design)

target_sources(ehl_juce_design INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/EhlDesign.cpp>)
target_include_directories(ehl_juce_design
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
target_compile_features(ehl_juce_design INTERFACE cxx_std_17)
target_link_libraries(ehl_juce_design INTERFACE juce::juce_gui_basics)

if(EHL_JUCE_DESIGN_BUILD_TESTS)
    enable_testing()
    add_executable(ehl_juce_design_tests tests/EhlDesignTests.cpp)
    target_link_libraries(ehl_juce_design_tests PRIVATE EHL::JuceDesign juce::juce_gui_basics)
    target_compile_definitions(ehl_juce_design_tests PRIVATE
        EHL_JUCE_DESIGN_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
    add_test(NAME ehl_juce_design_tests COMMAND ehl_juce_design_tests)
endif()
