cmake_minimum_required(VERSION 3.24)

project(Sunlight LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# include(FetchContent)
# FetchContent_Declare(
#     libcitygml
#     GIT_REPOSITORY https://github.com/jklimke/libcitygml.git
#     GIT_TAG v2.5.1
# )
# FetchContent_MakeAvailable(libcitygml)

# set(libcitygml_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/libcitygml-build)
# message("PATH libcitygml: "${libcitygml_DIR})
# list(APPEND CMAKE_MODULE_PATH ${libcitygml_DIR})
# find_package(libcitygml REQUIRED)
# include_directories(${libcitygml_INCLUDE_DIRS})
# link_directories(${libcitygml_LIBRARY_DIRS})

include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
  FetchContent_Populate(glm)
  add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
endif()


find_package(OpenGL REQUIRED)
if(NOT ${OPENGL_FOUND})
  message("OpenGL not found")
endif()

if(NOT ${OPENGL_GLU_FOUND})
  message("GLU not found")
endif()

find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})


file(GLOB_RECURSE source_files "src/*.cpp")
file(GLOB_RECURSE header_files "src/*.h")

message("Source files")
foreach(line IN LISTS source_files)
  message("${line}")
endforeach()

message("Header files")
foreach(line IN LISTS header_files)
  message("${line}")
endforeach()

find_package(Boost 1.40 COMPONENTS program_options REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

include_directories( "src/" )
#include_directories( "src/third-partys/libxml2-vc143/include/" )
include_directories( "src/third-partys/spdlog-1.11.0/include/" )


add_executable(Sunlight ${source_files} ${header_files})
target_include_directories(Sunlight PRIVATE ${glm_SOURCE_DIR})
target_link_libraries(Sunlight LINK_PUBLIC ${OPENGL_LIBRARIES} ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES})
