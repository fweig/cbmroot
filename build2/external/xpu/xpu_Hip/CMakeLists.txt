project(xpu_Hip)
cmake_minimum_required(VERSION 3.18)

if (Hip STREQUAL "Cuda")
    enable_language(CUDA)
    set(CMAKE_CUDA_STANDARD ${CMAKE_CXX_STANDARD})
    if (OFF)
        set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --debug --device-debug")
    else()
        add_definitions(-DNDEBUG)
    endif()
    set(CMAKE_CUDA_ARCHITECTURES 75)
    set_source_files_properties(/home/heinemann/gpuunpacker/cbmroot/external/xpu/src/xpu/detail/platform/hip_cuda/cuhip_driver.cpp PROPERTIES LANGUAGE CUDA)
endif()

if ("Hip" STREQUAL "Hip")
    if (OFF)
        add_compile_options(-Wall -Wextra -Werror)
    endif()
    if (OFF)
        add_compile_options(-Og -g3)
    else()
        add_compile_options(-O3)
        add_definitions(-DNDEBUG)
    endif()

    # Hip sets some clang flags that results in warnings
    add_compile_options(-Qunused-arguments)
    set(GPU_TARGETS "gfx906;gfx908")
    list(APPEND CMAKE_PREFIX_PATH "/opt/rocm-5.4.3/hip" "/opt/rocm-5.4.3")
    find_package(hip REQUIRED)

    # On ROCm hipCUB requires rocPRIM
    find_package(rocprim REQUIRED CONFIG PATHS "/opt/rocm-5.4.3/rocprim")

    # "/opt/rocm" - default install prefix
    find_package(hipcub REQUIRED CONFIG PATHS "/opt/rocm-5.4.3/hipcub")
endif()

if ("Hip" STREQUAL "Sycl")
    message(STATUS "SYCL HOST CXX: /usr/bin/c++")
    set(SYCL_FLAGS -fsycl -fsycl-targets=spir64)
    message(STATUS "SYCL FLAGS: ${SYCL_FLAGS}")
    add_compile_options(${SYCL_FLAGS})
    if (OFF)
        add_compile_options(-Wall -Wextra -Werror)
    endif()
    if (OFF)
        add_compile_options(-Og -g3)
    else()
        add_compile_options(-O3)
        add_definitions(-DNDEBUG)
    endif()
    add_link_options(${SYCL_FLAGS})
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Try to get complete list of include directories with this: https://stackoverflow.com/questions/43554085/target-link-libraries-and-include-directories-property
# include_directories()
include(xpu_Hip-include-dirs.cmake)
add_library(xpu_Hip MODULE /home/heinemann/gpuunpacker/cbmroot/external/xpu/src/xpu/detail/platform/hip_cuda/cuhip_driver.cpp)

if ("Hip" STREQUAL "Hip")
    target_link_libraries(xpu_Hip hip::device hip::hipcub)
endif()
