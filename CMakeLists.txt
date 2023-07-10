# At least 3.12, because FetchContent was release at 3.12
cmake_minimum_required(VERSION 3.12.0...3.27.0)
project(Sunlight LANGUAGES CXX)

# Set C++ 20 for the entire project and not only for the current target,
# as this ensures consistency across all targets
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# ============================== Add Security Check ============================== 

if(MSVC)
  # Enable Security Development Lifecycle flag in Visual Studio
  # The compiler will generate some code that does several checks at run time
  # For more information : https://learn.microsoft.com/en-us/cpp/build/reference/sdl-enable-additional-security-checks?view=msvc-170#runtime-checks
  add_compile_options(/sdl)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Add several security checks
  #
  # -fstack-protector : the compiler adds a small amount of code to the prologue and epilogue 
  # of functions that have local arrays or buffers. This added code is responsible for setting up 
  # and checking a "canary" value. If the canary value has been altered, it indicates that a buffer 
  # overflow has occured and the program will terminate immediately.
  # More information https://wiki.debian.org/Hardening#DEB_BUILD_HARDENING_STACKPROTECTOR_.28gcc.2Fg.2B-.2B-_-fstack-protector-strong.29
  #
  # -D_FORTIFY_SOURCE=2 : the compiler replaces calls to unsafe functions (such as strcpy, sprintf, and gets) 
  # with safer, fortified versions of those functions (like __strcpy_chk, __sprintf_chk, and __gets_chk)
  # preventing buffer overflow vulnerabilities.
  # More information : https://wiki.debian.org/Hardening#DEB_BUILD_HARDENING_FORTIFY_.28gcc.2Fg.2B-.2B-_-D_FORTIFY_SOURCE.3D2.29
  add_compile_options(-fstack-protector -D_FORTIFY_SOURCE=2)
endif()


# ============================== Add All Sunlight sources ============================== 
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
