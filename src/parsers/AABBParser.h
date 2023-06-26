#pragma once

#include <map>

#include <maths/AABB.h>
#include <citygmls/CityObject.h>
#include <parsers/TiledFilesLayout.h>


/**
*	@brief Load a collection of box from a file
*	@param path Path to the file
*	@return A collection of box
*/
std::vector<AABB> loadAABBFile(const std::string& path);

/**
*	@brief Load a bounding box for an entire layer (building, mount...)
*	@param layerDirectory Directory contain one folder for each data layer
*  (_BATI : building, _MNT : ground...) that contains your citygml
*	@return AABB Collection of the set of tiles
*/
AABBCollection loadLayersAABBs(const std::string& layerDirectory);

/**
*	@brief Save a collection of boxes on disk
*	@param filePath where to save the collection
*	@param AABBs The collection of box
*/
void saveAABB(const std::string& filePath, const std::map<std::string, std::pair<glm::highp_dvec3, glm::highp_dvec3>>& AABBs);

/**
*	@brief Build a collection of boxes from a citygml file in a set of directory
*	@param cityGmlDirectory Directory where citygml tiles are located
*	@param tile Groups of cityobject in a given type
*	@param type Type of citygml (building, ground...)
*	@return a collection of boxes, key = name of the box, value = <min of the box, max of the box>
*/
std::map<std::string, std::pair<glm::highp_dvec3, glm::highp_dvec3>> buildAABB(const std::string& cityGmlDirectory, const TiledLayer& tile, const CityObjectsType& type);

/**
*	@brief Build bounding box from a set of tile in a text file. One file will contain Building AABBs (tilename_Building_AABB.dat)
*  and the other one Building Parts AABBs (tilename_BuildingParts_AABB.dat)
*	@param cityGmlDirectory Directory where the citygml files are located, must be contain in a layer folder
*  (_BATI : building, _MNT : ground..)
*/
void buildLayersAABBs(const std::string& cityGmlDirectory);

///
/// \brief doBuildBuildingAABBs Build Building AABBs and Building Parts AABBs
/// \param filepath Path to file to build AABB for.
///
///

/**
 * @brief Create building and building parts bounding box for a citygml tile at a given path
 * @param filepath Full path of the citygml tile
*/
void buildBuildingAABBs(const std::string& filepath);

/**
 * @brief Loops recursively through folder containing gml files representing buildings, compute and save AABBs for CityGML Building and Building Parts notions.
 * @param buildingFilesFolder Folder containing a "_BATI" folder holding Building gml files
*/
void buildBuildingAABBsInDirectory(const std::string& buildingFilesFolder);
