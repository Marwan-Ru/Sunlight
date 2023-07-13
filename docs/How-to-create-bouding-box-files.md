# How to create bounding box files

## Introduction
Bounding box files (.dat) are custom files specially made for [3DUSE](https://github.com/VCityTeam/3DUSE), it's not a standard. Our program (Sunlight) uses these bounding box to optimize his render time.

## Prerequisites
You need to create at least one Tile for 3DUSE (and not 3DTiles), [here](https://github.com/VCityTeam/3DUSE/blob/master/doc/HowTo/HowTo_CreateTiles.md) is an explaining on how to make one.

## Bounding Box Types
They are two types of bounding box : Layer AABB and Building AABB. We use naming conventions to differentiate them.
Even if we support ground, we do not create or use bounding box, because it's already contained one large object.

### Layer AABB
Layer AABB is a bounding box of a layer or a type of surface (e.g Building, Ground...). The naming convention is "_BAT_AABB.dat" or "_MNT_AABB.dat".

A Layer AABB file contains the number of tiles, then a list of tiles with their names and bounding box coordinates.

Here a example of mount Layer AABB : 
```
16
_BATI/3682_10350/3682_10350_BATI.gml
340282346638528859811704183484516925440.000000
340282346638528859811704183484516925440.000000
340282346638528859811704183484516925440.000000
-340282346638528859811704183484516925440.000000
-340282346638528859811704183484516925440.000000
-340282346638528859811704183484516925440.000000
_BATI/3682_10351/3682_10351_BATI.gml
340282346638528859811704183484516925440.000000
340282346638528859811704183484516925440.000000
340282346638528859811704183484516925440.000000
-340282346638528859811704183484516925440.000000
-340282346638528859811704183484516925440.000000
-340282346638528859811704183484516925440.000000
```

### Building AABB
**Building AABB** is a list of bounding box of all buildings in a cityGML file. The naming convention is "Your_CityGML_Name_Building_AABB.dat".

A Building AABB file contains the number of building, then a list of building with their names and bounding box coordinates.

Here a small part of _"Lyon 1er arrondissement 2018"_ bounding box : 
```
47
OPERA_LYON_45
1842810.000000
5175815.500000
170.786972
1842869.750000
5175863.500000
209.217361
MUSEE_BEAUX_ARTS_43
1842555.500000
5175645.000000
168.679657
1842663.750000
5175774.000000
210.853302
```


## How to create a Bounding Box file

### Follow the supported folder hierarchy
Sunlight datas folder hierarchy : 
```
Specified Datas Directory
├── _BATI                                        # Contains cityGML Buildings
|    ├── 3686_10345                              # Tile folder
|        ├── 3686_10345.gml                      # Building example
|        ├── 3686_10345_Building_AABB.gml        # Building AABB
|
├── _MNT                                         # Contains cityGML Ground / Mount
|    ├── 3686_10345                              # Tile folder
|        ├── 3686_10345.gml                      # Ground example
|
├── _BATI_AABB.dat                               # Layer AABB for building layer
├── _MNT_AABB.dat                                # Layer AABB for mount layer
```
It's the only folder hierarchy supported by Sunlight, it's a relic of 3DUSE Plugin. You have to follow this standard, otherwise, your cityGML files will be ignored.

### Create a Layer AABB File

After setting the correct datas directory, execute the code below : 
```cp
#include <string>

#include <parsers/AABBParser.h>

int main()
{
   // Directory containing all datas for testing purpose (gml, bounding box, 3DUSE output..)
   const std::string datasDirectory(".\\datas\\");

   buildLayersAABBs(datasDirectory);
}
```
If the code was successful, for each layer type, you will found a Layer AABB file in your datas directory (e.g "_BATI_AABB.dat").

### Create a Building AABB File

After setting the correct datas directory, execute the code below : 
```cp
#include <string>

#include <parsers/AABBParser.h>

int main()
{
   // Directory containing all datas for testing purpose (gml, bounding box, 3DUSE output..)
   const std::string datasDirectory(".\\datas\\_BATI\\");

   buildBuildingAABBsInDirectory(datasDirectory);
}
```

If the code was successful, for each gml file encounter, it will create a Building AABB file in the same folder.

## Glossary
AABB : Axis Aligned Bounding Box  
Layer : Type of surface (Building, Mount...)