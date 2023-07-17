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

#include "RayHit.h"
#include "Ray.h"

Triangle::Triangle(TVec3d a, TVec3d b, TVec3d c)
{
    this->a = a;
    this->b = b;
    this->c = c;
    objectType = CityObjectsType::COT_All;
    subObjectType = CityObjectsType::COT_All;
}

Triangle::Triangle(const TVec3d& _a, const TVec3d& _b, const TVec3d& _c, const std::string& triangleId, const std::string& tileName)
   : a(_a), b(_b), c(_c), m_id(triangleId), m_tileName(tileName)
{
}

TVec3d Triangle::getNormal() const
{
   auto x = b - a;
   auto y = c - a;
   auto normal = x.cross(y);

   return normal.normalize();
}

const std::string& Triangle::getId() const
{
   return m_id;
}

const std::string& Triangle::getTileName() const
{
   return m_tileName;
}

//Ray triangle intersection, from geometric tools engine
//License : http://www.boost.org/LICENSE_1_0.txt
std::optional<RayHit> Triangle::doesIntersect(const Ray& ray) const
{
   // Compute the offset origin, edges, and normal.
   TVec3d diff = ray.origin - a;
   TVec3d normal = getNormal();

   TVec3d edge1 = b - a;
   TVec3d edge2 = c - a;

   // Solve Q + t*D = b1*E1 + b2*E2 (Q = kDiff, D = ray direction,
   // E1 = edge1, E2 = edge2, N = Cross(E1,E2)) by
   //   |Dot(D,N)|*b1 = sign(Dot(D,N))*Dot(D,Cross(Q,E2))
   //   |Dot(D,N)|*b2 = sign(Dot(D,N))*Dot(D,Cross(E1,Q))
   //   |Dot(D,N)|*t = -sign(Dot(D,N))*Dot(Q,N)
   double DdN = ray.direction.dot(normal);
   double sign;
   if (0.0 < DdN)
   {
      sign = 1.0;
   }
   else if (DdN < 0.0)
   {
      sign = -1.0;
      DdN = -DdN;
   }
   else
   {
      // Ray and triangle are parallel, call it a "no intersection"
      // even if the ray does intersect.
      return {};
   }

   double DdQxE2 = sign * ray.direction.dot(diff.cross(edge2));
   if (0.0 <= DdQxE2)
   {
      double DdE1xQ = sign * ray.direction.dot(edge1.cross(diff));
      if (0.0 <= DdE1xQ)
      {
         if (DdQxE2 + DdE1xQ <= DdN)
         {
            // Line intersects triangle, check whether ray does.
            double QdN = -sign * diff.dot(normal);
            if (0.0 <= QdN)
            {
               // Ray intersects triangle.
               double inv = ((float)1) / DdN;
               TVec3d impactPoint(ray.origin + inv * QdN * ray.direction);

               float distance ((ray.direction * QdN * inv).length());

               return RayHit(ray, impactPoint, *this, distance);
            }
            // else: t < 0, no intersection
         }
         // else: b1+b2 > 1, no intersection
      }
      // else: b2 < 0, no intersection
   }
   // else: b1 < 0, no intersection

   return {};
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
                            t.m_tileName = tilefilename;

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
                        t.m_tileName = tilefilename;

                        triangles->push_back(std::make_shared<Triangle>(t));
                    }
                }
        }
    }

    delete tile;

    return triangles;
}
