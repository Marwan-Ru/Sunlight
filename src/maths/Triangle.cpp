// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <vector>

#include "Triangle.h"
#include <citygmls/CityObject.h>
#include <citygmls/CityModel.h>
#include <citygmls/Polygon.h>
#include <citygmls/Geometry.h>
#include <citygmls/Tile.h>

Triangle::Triangle(TVec3d a, TVec3d b, TVec3d c)
{
    this->a = a;
    this->b = b;
    this->c = c;
    objectType = CityObjectsType::COT_All;
    subObjectType = CityObjectsType::COT_All;
}

TVec3d Triangle::GetNormal()
{
   auto x = b - a;
   auto y = c - a;
   auto normal = x.cross(y);

   return normal.normalize();
}

std::vector<std::shared_ptr<Triangle>>* BuildTriangleList(const std::string& tilefilename, const CityObjectsType& objectType, const std::string& cityObjId, const double& zMin)
{
    double epsilon = 0.0001;

    std::vector<std::shared_ptr<Triangle>>* triangles = new std::vector<std::shared_ptr<Triangle>>();

    Tile* tile = new Tile(tilefilename);

    CityModel* model = tile->getCityModel();

    for (CityObject* obj : model->getCityObjectsRoots()) //For each city object
    {
        if (cityObjId.compare("") != 0 && cityObjId.compare(obj->getId()) != 0) //If cityObj not default "" and current city object equals to cityObjId
            continue;

        if (obj->getType() == COT_Building && objectType == COT_Building) //We only take building or terrain
        {
            for (CityObject* object : obj->getChildren())//On parcourt les objets (Wall, Roof, ...) du batiment
                for (Geometry* Geometry : object->getGeometries()) //pour chaque geometrie
                    for (Polygon* PolygonCityGML : Geometry->getPolygons()) //Pour chaque polygone
                    {
                        //Get triangle list
                        const std::vector<TVec3d>& vert = PolygonCityGML->getVertices();
                        const std::vector<unsigned int>& ind = PolygonCityGML->getIndices();

                        for (unsigned int i = 0; i < ind.size() / 3; i++)//Push all triangle of the polygon in our list
                        {
                            TVec3d a = vert[ind[i * 3 + 0]];
                            TVec3d b = vert[ind[i * 3 + 1]];
                            TVec3d c = vert[ind[i * 3 + 2]];

                            // If all vertices of current triangle are below given zMin
                            if (a.z - zMin < epsilon && b.z - zMin < epsilon && c.z - zMin < epsilon)
                                continue;

                            Triangle t = Triangle(a, b, c);
                            t.subObjectType = object->getType();
                            t.objectType = obj->getType();
                            t.objectId = obj->getId();
                            t.polygonId = PolygonCityGML->getId();
                            t.tileFile = tilefilename;

                            triangles->push_back(std::make_shared<Triangle>(t));
                        }
                    }
        }
        // #CityObjectType
        // We check if the current cityobject is the same type of the wanted type of cityobject given in parameter
        // Exemple : (obj->getType() == COT_<MyType> && objectType == COT_<MyType>
        else if ((obj->getType() == COT_SolitaryVegetationObject && objectType == COT_SolitaryVegetationObject) ||
            (obj->getType() == COT_TINRelief && objectType == COT_TINRelief) ||
            (obj->getType() == COT_WaterBody && objectType == COT_WaterBody))
        {

            for (Geometry* Geometry : obj->getGeometries()) //pour chaque geometrie
                for (Polygon* PolygonCityGML : Geometry->getPolygons()) //Pour chaque polygone
                {
                    //Get triangle list
                    const std::vector<TVec3d>& vert = PolygonCityGML->getVertices();
                    const std::vector<unsigned int>& ind = PolygonCityGML->getIndices();

                    for (unsigned int i = 0; i < ind.size() / 3; i++)//Push all triangle of the polygon in our list
                    {
                        TVec3d a = vert[ind[i * 3 + 0]];
                        TVec3d b = vert[ind[i * 3 + 1]];
                        TVec3d c = vert[ind[i * 3 + 2]];

                        // If all vertices of current triangle are below given zMin
                        if (a.z - zMin < epsilon && b.z - zMin < epsilon && c.z - zMin < epsilon)
                            continue;

                        Triangle t = Triangle(a, b, c);
                        t.objectType = obj->getType();
                        t.objectId = obj->getId();
                        t.polygonId = PolygonCityGML->getId();
                        t.tileFile = tilefilename;

                        triangles->push_back(std::make_shared<Triangle>(t));
                    }
                }
        }
    }

    delete tile;

    return triangles;
}
