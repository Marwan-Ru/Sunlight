# At least 3.12, because FetchContent was release at 3.12
cmake_minimum_required(VERSION 3.12.0...3.27.0)
project(Sunlight LANGUAGES CXX)

# Set C++ 20 for the entire project and not only for the current target,
# as this ensures consistency across all targets
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# ============================== Add All Sunlight souces ============================== 
include_directories("src/")

# Add all headers and sources files
# FIXME : make a list of all class available, 
# because we can compile some file that don't need to be linked
file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE SRCS "src/*.cpp")

add_executable(Sunlight ${SRCS} ${HEADERS})


# ============================== Add External libraries ============================== 
# Add OpenGL requirement
find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
  message (FATAL_ERROR "OpenGL not found.")
endif()

target_link_libraries(Sunlight ${OPENGL_LIBRARIES})

# Add libxml2
find_package(LibXml2 REQUIRED)
if(NOT LIBXML2_FOUND)
  message (FATAL_ERROR "LibXml2 not found.")
endif()

include_directories(${LIBXML2_INCLUDE_DIR})
ADD_DEFINITIONS("-DUSE_LIBXML2")
target_link_libraries(Sunlight ${LIBXML2_LIBRARIES})

# Add spdlog
include_directories("src/third-partys/spdlog-1.11.0/include/")

# Add glm requirement
include_directories("src/third-partys/glm.0.9.9.800/build/native/include/")
