// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <memory>
#include <map>
#include <string>

#include "citygmls/CityObject.h"
#include "citygmls/CityModel.h"

#include "citygml.hpp"
#include "URI.hpp"

#ifdef _MSC_VER
#pragma warning(disable: 4251) // VC++ DLL jejune complains on STL _Id member
#endif

/// \brief The Tile class
/// Holds citygml, shp data
class Tile
{
public:
  Tile();

  /// Build a tile and load a CityGML file
  /// \param filepath CityGML file path
  Tile(const std::string& filepath);
  //Tile(const TVec3d& pMin, const TVec3d& pMax);
  ~Tile();

  /// Get tile enveloppe
  Envelope& getEnvelope();
  const Envelope& getEnvelope() const;

  /// Compute envoloppe
  void computeEnvelope();

  void load(const std::string& filepath);
  //osg::ref_ptr<osg::Node> buildOsgData();

  CityModel* getCityModel();
  const CityModel* getCityModel() const;

  /// Get tile name
  const std::string& getName() const;

  /// Set tile name
  void setName(const std::string& name);
  //osg::ref_ptr<osg::Node> getOsgRoot();

  //citygml::CityObject* findNode(const std::string& name);

  void deleteNode(const std::string& name);
  void insertNode(CityObject* node);
  void replaceNode(const std::string& name, CityObject* node);

  /// \brief getNode Get a CityGML node
  /// \param uri URI pointing to the CityGML node
  /// \return Ptr to CityGML node or nullptr
  CityObject* getNode(const URI& uri, bool inPickingMode = false);

  /// Get the path of the CityGML file loaded
  const std::string& getCityGMLfilePath() const;

private:
  std::string m_name;             ///< tile name
  std::string m_citygmlFilePath;  ///< Path of the CityGML file loaded

  CityModel* m_root; // TODO : remove
};

typedef std::shared_ptr<Tile> TilePtr;