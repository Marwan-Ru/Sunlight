// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <thread>
#include <mutex>

#include "RayTracing.h"
#include "RayHit.h"
#include "Triangle.h"

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
         auto rayHit = tri->doesIntersect(*ray);
         if (!rayHit.has_value())
            continue;

         data.Hits->push_back(&rayHit.value());

         if (data.breakOnFirstInter) //No need to test other intersections for this ray if we want to stop after first intersection is found
            break;
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


std::vector<RayHit> RayTracing(const Ray& ray, const std::vector<Triangle>& triangles) {
    std::vector<RayHit> result;

    // Define the number of threads to use
    const int numThreads = std::thread::hardware_concurrency();

    // Divide the triangles into equal parts for each thread
    const int numTriangles = triangles.size();
    const int trianglesPerThread = numTriangles / numThreads;

    // Define a mutex to protect the result vector
    std::mutex mutex;

    // Define a lambda function to be executed by each thread
    auto traceTriangles = [&](int start, int end) {
        std::vector<RayHit> threadResult;

        for (int i = start; i < end; i++) {
            auto triangle = triangles[i];
            
            auto rayHit = triangle.doesIntersect(ray);
            if (!rayHit.has_value())
                continue;

            threadResult.push_back(rayHit.value());
        }

        // Lock the mutex and append the threadResult vector to the result vector
        std::lock_guard<std::mutex> lock(mutex);
        result.insert(result.end(), threadResult.begin(), threadResult.end());
    };

    // Create and start the threads
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; i++) {
        int start = i * trianglesPerThread;

        // Split num triangles by thread and the last thread does intersection on remaining triangles
        int end = (i == numThreads - 1) ? numTriangles : (i + 1) * trianglesPerThread;
        threads.emplace_back(traceTriangles, start, end);
    }

    // Join the threads
    for (auto& thread : threads) {
        thread.join();
    }

    return result;
}