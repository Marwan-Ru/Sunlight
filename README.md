# Sunlight

<!-- TOC -->

- [Sunlight](#sunlight)
    - [About The Project](#about-the-project)
    - [Getting Started](#getting-started)
        - [External Dependencies](#external-dependencies)
        - [External Sources Policies](#external-sources-policies)
        - [Prerequisites](#prerequisites)
            - [For Linux](#for-linux)
            - [For Windows](#for-windows)
        - [Installation](#installation)
            - [For Linux](#for-linux-1)
            - [For Windows](#for-windows-1)
    - [Usage](#usage)
        - [Directory Hierarchy](#directory-hierarchy)
    - [Contact](#contact)
    - [Acknowledgments](#acknowledgments)

<!-- /TOC -->

## About The Project
Library fo light pre-calculation based on real data (urban data and sun position). It's a project extract from [3DUSE](https://github.com/VCityTeam/3DUSE) separate from parsing operations and as such, with less dependency.


## Getting Started
### External Dependencies
We are using C++ 20 because we are using latest functionalities ([optional](https://en.cppreference.com/w/cpp/utility/optional), [filesystem](https://en.cppreference.com/w/cpp/filesystem)...).

- [spdlog - v1.11.0](https://github.com/gabime/spdlog)


### External Sources Policies
In CMake, we will [fetch](https://cmake.org/cmake/help/latest/module/FetchContent.html) all externals libraries to avoid external sources in VCity repository. For more information on how include the fetch content, click [here](https://stackoverflow.com/questions/63311116/how-to-find-the-source-directory-of-package-downloaded-via-fetchcontent).  
⚠️ Fetching an external content will always populate the build directory following the standard of a lower-case name. For instance, the package LibXml2 will have a macro libxml2_SOURCE_DIR and a directory libxml2-src directory.

More information [here](https://github.com/VCityTeam/Sunlight/issues/5).


### Prerequisites
- CMake 3.27.

#### For Linux
1. Install git LFS package
   ``` bash
   sudo apt-get install git-lfs
   ```

#### For Windows
1. [Follow this tutorial to install and configure Visual Studio](https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170).

2. [Download CMake 3.27](https://cmake.org/download/).

### Installation
#### For Linux
1. Clone the repository and move to sunlight folder.
   ``` bash
   git clone https://github.com/VCityTeam/Sunlight.git
   cd Sunlight
   ```

2. Fetch all cityGML files.
   ``` bash
   git lfs fetch --all
   ```

3. Create a build folder in Sunlight that will contains the build of sunlight.
   ``` bash
   mkdir build/
   ```

4. Compile Sunlight.
   ``` bash
   cd build/
   cmake .. && make
   ```

#### For Windows
1. Clone the repository and move to sunlight folder.
   ``` bash
   git clone https://github.com/VCityTeam/Sunlight.git
   cd Sunlight
   ```

2. Fetch all cityGML files.
   ``` bash
   git lfs fetch --all
   ```

3. Create a build folder in Sunlight that will contains the build of sunlight.

4. Open CMake and specify the source code path (Sunlight folder) and the build binaries path (Sunlight/build).

5. Click on Configure, Generate then Open Project and you are good to go.

## Usage
### Directory Hierarchy
```
Sunlight (repo)
├── datas                     # Datas use for testing
├── docs                      # Documentations (original charts...)
├── src                       # Source code
|    ├── cores                # Core program / Main functionality
|    ├── maths                # Mathematical and geometrical class
|    ├── parsers              # Annual sun position parser
|    ├── utils                # Small utils / helpers (timer, operators override...)
|
├── .gitattributes            # Define Large Files (.gml, .dat...)
├── .gitignore                # Files/folders ignored by Git
├── CMakeLists.txt            # CMake file to create a crossplatform software
├── README.md
```

## Contact
- Wesley Petit - [Website](https://wesleypetit.fr/) - wesley.petit.lemoine@gmail.com

## Acknowledgments
- [3DUSE Sunlight Plugin](https://github.com/VCityTeam/3DUSE)
