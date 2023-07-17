// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <thread>

#include "RayTracing.h"
#include "Triangle.h"
#include "RayHit.h"

/**
*	@brief Data used by a ray tracing thread
*/
struct RayTracingData
{
    std::vector<std::shared_ptr<Triangle>>* triangles; ///< List of triangles of a 3D Model
    std::vector<std::shared_ptr<Ray>>* rowToDo; ///< List of ray to use for ray tracing
    std::vector<RayHit*>* Hits; ///< List of hits which store the intersections informations
    bool breakOnFirstInter; ///< If true, stop raytracing when an intersection is found. If false, compute all intersections between rays and triangles.
};

//Loop through all triangles and check if any rays intersect with triangles
void RayLoop(const RayTracingData& data)
{
   for (unsigned int k = 0; k < data.rowToDo->size(); k++)
   {
      auto ray (data.rowToDo->at(k));

      for (const auto& tri : (*data.triangles))
      {
         RayHit* hit = new RayHit();
         if (ray->Intersect(tri, hit))//Check if the ray hit the triangle and
         {
               data.Hits->push_back(hit);

            if (data.breakOnFirstInter) //No need to test other intersections for this ray if we want to stop after first intersection is found
               break;
         }
         else
         {
            delete hit;
         }
      }
   }
}

std::vector<RayHit*>* RayTracing(std::vector<std::shared_ptr<Triangle>>* triangles, const std::vector<std::shared_ptr<Ray>>& rays, bool breakOnFirstInter)
{
   unsigned int tCount = std::thread::hardware_concurrency() - 1;//Get how many thread we have
   unsigned int rayPerThread = static_cast<unsigned int>(rays.size()) / tCount;

   //List of rays and their frag coord
   std::vector<std::shared_ptr<Ray>>* toDo = new std::vector<std::shared_ptr<Ray>>[tCount];//List of rays for each threads

    for (unsigned int i = 0; i < tCount; i++)
    {
       toDo[i].insert(toDo[i].begin(), rays.begin() + i * rayPerThread, rays.begin() + (i + 1) * rayPerThread);
    }

    //If number of Ray is not a multiple of thread number, add the rest of the ray to the threads
    if (rays.size() % tCount != 0)
    {
        for (unsigned int i = 0; i < rays.size() % tCount; ++i)
        {
            toDo[i].push_back(rays.at(rayPerThread * tCount + i));
        }
    }

    //List of hits for each thread
    std::vector<RayHit*>* hitsArray = new std::vector<RayHit*>[tCount];

    //std::cout << "Thread : " << tCount << std::endl;
    //std::cout << "Ray count : " << rays.size() << std::endl;

    std::vector<std::thread*> threads;//Our thread list

    for (unsigned int i = 0; i < tCount; i++)
    {
        RayTracingData data;
        data.triangles = triangles;
        data.rowToDo = &toDo[i];
        data.Hits = &hitsArray[i];
        data.breakOnFirstInter = breakOnFirstInter;

        std::thread* t = new std::thread(RayLoop, data);
        threads.push_back(t);
    }

    //std::cout << "Thread Launched " << std::endl;

    for (unsigned int i = 0; i < tCount; i++)//Join all our thread and update global data min and max distance
    {
        (*threads[i]).join();
        delete threads[i];
    }


    //std::cout << "The joining is completed" << std::endl;

    //Join vector of hits
    std::vector<RayHit*>* hits = new std::vector<RayHit*>();

    // preallocate memory
    std::vector<RayHit*>::size_type mem_size = 0;

    for (unsigned int i = 0; i < tCount; i++)
    {
        mem_size += hitsArray[i].size();
    }

    hits->reserve(mem_size);

    //Join hits vectors of each thread into one vector
    for (unsigned int i = 0; i < tCount; i++)
    {
        hits->insert(hits->end(), hitsArray[i].begin(), hitsArray[i].end());
    }


    hitsArray->clear();
    delete[] hitsArray;

    toDo->clear();
    toDo = nullptr;

    return hits;
}
