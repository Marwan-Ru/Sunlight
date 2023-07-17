# Sunlight
## Getting Started
### Prerequisites
We are using C++ 20 because we are using latest functionalities ([optional](https://en.cppreference.com/w/cpp/utility/optional), [filesystem](https://en.cppreference.com/w/cpp/filesystem)...).

- [glm - 0.9.9.8](https://github.com/g-truc/glm)
- [spdlog - v1.11.0](https://github.com/gabime/spdlog)
- [libxml2 - 2.9.3](https://github.com/GNOME/libxml2)  
⚠️ We use a modified version of libxml2 available in the [3DUSE binary kit](https://download.gforge.liris.cnrs.fr/meppbin/windows/vs2015/VCITY/kits/VCITY_local_vs2015_64.7z).
- OpenGL
- Glu

### External Sources Policies
In CMake, we will [fetch](https://cmake.org/cmake/help/latest/module/FetchContent.html) all externals libraries to avoid external sources in VCity repository. For more information on how include the fetch content, click [here](https://stackoverflow.com/questions/63311116/how-to-find-the-source-directory-of-package-downloaded-via-fetchcontent).  
⚠️ Fetching an external content will always populate the build directory following the standard of a lower-case name. For instance, the package LibXml2 will have a macro libxml2_SOURCE_DIR and a directory libxml2-src directory.

More information [here](https://github.com/VCityTeam/Sunlight/issues/5).

### Install
#### For Linux
1. [Install OpenGL](https://doc.ubuntu-fr.org/opengl), libXml2 and git LFS package
``` bash
sudo apt-get install freeglut3-dev && sudo apt-get install libxml2-dev && sudo apt-get install git-lfs
```

2. Clone the repository and move to sunlight folder.
``` bash
git clone https://github.com/VCityTeam/Sunlight.git
cd Sunlight
```

3. Fetch all cityGML files.
``` bash
git lfs fetch --all
```

4. Create a build folder in Sunlight that will contains the build of sunlight.
``` bash
mkdir build/
```

5. Compile Sunlight.
``` bash
cd build/
cmake .. && make
```

#### For Windows
1. [Follow this tutorial to install and configure Visual Studio](https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170).

2. Clone the repository and move to sunlight folder.
``` bash
git clone https://github.com/VCityTeam/Sunlight.git
cd Sunlight
```

3. Fetch all cityGML files.
``` bash
git lfs fetch --all
```

4. Create a build folder in Sunlight that will contains the build of sunlight.

5. [Download CMake 3.27](https://cmake.org/download/).

6. Open CMake and specify the source code path (Sunlight folder) and the build binaries path (Sunlight/build).

7. Click on configure, Generate then Open Project and you are good to go.

## Usage
### Pipeline - Activity Chart
Here is the pipeline we follow for Sunlight :
![Pipeline Activity Chart](./docs/Pipeline_Activity_Chart.png)

### Directory Hierarchy
```
Sunlight (repo)
├── datas                     # Datas use for testing
├── docs                      # Documentations (original charts...)
├── src                       # Source code
|    ├── ADE
|    ├── citygml                
|    ├── cores                # Core program / Main functionality
|    ├── maths                # Mathematical and geometrical class
|    ├── parsers
|    ├── utils                # Small utils / helpers (timer, operators override...)
|
├── third-partys              # Externals libraries
|        ├── libxml2-2.9.3    # Precompiled windows parser for cityGML
|
├── .editorconfig             # Formatting rules and configuration
├── .gitattributes            # Define Large Files (.gml, .dat...)
├── .gitignore                # Files/folders ignored by Git
├── CMakeLists.txt            # CMake file to create a crossplatform software
├── Readme.md
```
