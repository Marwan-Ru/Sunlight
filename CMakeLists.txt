# For more information about the current CMake implementation,
# follow this link : https://github.com/VCityTeam/sunlight/issues/5

# At least 3.12, because FetchContent was release at 3.12
cmake_minimum_required(VERSION 3.13...3.27)
project(sunlight LANGUAGES CXX)

# Include fetch cmake module to fetch external content (e.g. repository...)
include(FetchContent)


# ============================== Add All sunlight sources ============================== 
# Add all headers and sources files
# FIXME : make a list of all class available, 
# because we can compile some file that don't need to be linked
file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE SRCS "src/*.cpp")

# Add static library for pySunlight
add_library(sunlight ${HEADERS} ${SRCS})
# add_executable(sunlight ${SRCS} ${HEADERS})

target_include_directories(sunlight PRIVATE "src/")


# ============================== Add Compiler Flags ============================== 
target_compile_features(sunlight PRIVATE cxx_std_20)

if(MSVC)
  # Enable Security Development Lifecycle flag in Visual Studio
  # The compiler will generate some code that does several checks at run time
  # For more information : https://learn.microsoft.com/en-us/cpp/build/reference/sdl-enable-additional-security-checks?view=msvc-170#runtime-checks
  target_compile_options(sunlight PRIVATE /sdl)
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
  target_compile_options(sunlight PRIVATE -fstack-protector -D_FORTIFY_SOURCE=2)

  # fPIC flag : https://iq.opengenus.org/fpic-in-gcc/
  target_compile_options(sunlight PRIVATE -fPIC)
endif()


# ============================== Add External libraries ============================== 
# Add OpenGL requirement
find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
  message (FATAL_ERROR "OpenGL not found.")
endif()

target_link_libraries(sunlight PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(sunlight PRIVATE ${OPENGL_glu_LIBRARY})


# Add libxml2
if (UNIX)
  find_package(LibXml2 REQUIRED)
  if (NOT LIBXML2_FOUND)
    message (FATAL_ERROR "LibXml2 not found.")
  endif()

  target_include_directories(sunlight PRIVATE ${LIBXML2_INCLUDE_DIR})
  target_link_libraries(sunlight PRIVATE ${LIBXML2_LIBRARIES})
endif()

# Add precompiled windows libxml
if (WIN32)
  set(LibXml2 "third-partys/libxml2-2.9.3")

  target_include_directories(sunlight PRIVATE ${LibXml2}/include/libxml2/)
  target_link_directories(sunlight PRIVATE ${LibXml2}/lib/)
  target_link_libraries(sunlight PRIVATE libxml2)
endif()

# Add multithreading support on unix
# For more information : https://stackoverflow.com/questions/1620918/cmake-and-libpthread/29871891#29871891
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(sunlight PRIVATE Threads::Threads)

# Add spdlog
# WARNING fetching an external content will always populate the build directory
# following the standard of a lower-case name. For instance, the package LibXml2 
# will have a macro libxml2_SOURCE_DIR and a directory libxml2-src directory.
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY    https://github.com/gabime/spdlog.git
    GIT_TAG           v1.11.0
)
FetchContent_MakeAvailable(spdlog)

target_include_directories(sunlight PRIVATE ${spdlog_SOURCE_DIR}/include)