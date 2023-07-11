# Sunlight

You can access to all Sunlight documentations [here](https://github.com/VCityTeam/Sunlight/wiki).

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