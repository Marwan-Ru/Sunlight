#pragma once

#include <maths/AABB.h>

/**
*	@brief Build bounding box from a set of tile, bounding boxes are saved on disk in a text file, not returned
*	@param dir Directory where the citygml files are located
*/
void BuildLayersAABBs(std::string dir);

/**
*	@brief Load a collection of box from a file
*	@param path Path to the file
*	@return A collection of box
*/
std::vector<AABB> LoadAABBFile(std::string path);

/**
*	@brief Load an aabb set previously build
*	@param dir Directory where citygml files are located, must contain several subdirectories, one for each data layer (terrain, building, water, ..)
*	@return AABB Collection of the set of tiles
*/
AABBCollection LoadLayersAABBs(std::string dir);

///
/// \brief BuildBuildingAABBs Loops recursively through folder containing gml files representing buildings, compute and save AABBs for CityGML Building and Building Parts notions.
///                           One file will contain Building AABBs (tilename_Building_AABB.dat) and the other one Building Parts AABBs (tilename_BuildingParts_AABB.dat).
///
/// \param buildingFilesFolder Path to folder holding Building gml files
///
void BuildBuildingAABBs(std::string buildingFilesFolder);
