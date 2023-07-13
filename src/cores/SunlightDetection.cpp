// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <algorithm>
#include <fstream>
#include <string>
#include <maths/Vector3.h>
#include <optional>
// Log in console
#include <spdlog/spdlog.h>
// Log in file
#include "spdlog/sinks/rotating_file_sink.h"

#include "SunlightDetection.h"
#include "SunlightCsvExporter.h"
#include "SunlightObjExporter.h"
#include "FileInfo.h"
#include <maths/Hit.h>
#include <utils/Timer.h>
#include <utils/DateTime.h>
#include <citygmls/CityObject.h>
#include <parsers/AABBParser.h>
#include <parsers/SunEarthParser.h>


std::queue<RayBoxHit> intersectAndSortByDistance(const std::vector<AABB>& boxes, const std::vector<std::shared_ptr<RayBox>>& raysBoxes)
{
   //The order in which the box has been crossed : if a ray cross multiple AABB, we record the order of crossing in each one
    std::map<std::string, int> boxToMaxOrder;

    //We keep record of the smallest rayboxhit of a box
    // MultiResolution
    std::map<std::string, RayBoxHit> boxToRayBoxHit;

    for (unsigned int i = 0; i < boxes.size(); i++)
    {
        boxToMaxOrder[boxes[i].name] = -1;
    }

    //For each ray
    for (unsigned int i = 0; i < raysBoxes.size(); i++)
    {
        auto rayBox = raysBoxes[i];
        rayBox->boxes.clear();

        //For each box we check if the ray intersect it
        for (unsigned int j = 0; j < boxes.size(); j++)
        {
            float hit0, hit1;
            if (rayBox->Intersect(boxes[j], &hit0, &hit1))
            {
                RayBoxHit hTemp;
                hTemp.box = boxes[j];
                hTemp.minDistance = hit0;
                rayBox->boxes.push_back(hTemp); //Add this box to list of intersected boxes by this ray
            }
        }

        std::sort(rayBox->boxes.begin(), rayBox->boxes.end());//Sort the boxes depending on their intersection distance

        for (std::size_t j = 0; j < rayBox->boxes.size(); j++)//We update the order of each box
        {
            int current = boxToMaxOrder[rayBox->boxes[j].box.name];
            boxToMaxOrder[rayBox->boxes[j].box.name] = std::max(static_cast<int>(j), current);

            if (boxToRayBoxHit.find(rayBox->boxes[j].box.name) != boxToRayBoxHit.end())
                // if rayBox->boxes[j].box.name already exists in boxToRayBoxHit
                // MultiResolution
            {
                boxToRayBoxHit[rayBox->boxes[j].box.name] = std::min(rayBox->boxes[j], boxToRayBoxHit[rayBox->boxes[j].box.name]);
            }
            else
            {
                boxToRayBoxHit.insert(std::make_pair(rayBox->boxes[j].box.name, rayBox->boxes[j]));
            }
        }
    }

    //Sort the boxes depending on their max order
    std::vector<BoxOrder> boxesOrder;

    for (auto it = boxToMaxOrder.begin(); it != boxToMaxOrder.end(); it++)
    {
        if (it->second >= 0)
        {
            BoxOrder boTemp;
            boTemp.box = it->first;
            boTemp.order = it->second;
            boxesOrder.push_back(boTemp);
        }
    }

    std::sort(boxesOrder.begin(), boxesOrder.end());

    //Setup the queue and return it
    std::queue<RayBoxHit> fileOrder;

    for (BoxOrder& bo : boxesOrder)
        fileOrder.push(boxToRayBoxHit[bo.box]);

    return fileOrder;
}



std::vector<std::shared_ptr<RayBox>> keepIntersectRaysBoxWith(const std::string& currentBBName, const std::vector<std::shared_ptr<RayBox>>& allRayBoxes, std::map<int, bool>& datetimeSunInfo)
{
   std::vector<std::shared_ptr<RayBox>> raysBoxes ({});//Not all rays intersect the box

    //We only get the rays that intersect with the box
    for (unsigned int i = 0; i < allRayBoxes.size(); i++)
    {
        auto raybox = allRayBoxes[i]; //Get the ray

        bool found = false;

        for (RayBoxHit& rbh : raybox->boxes)//Go through all the boxes that the ray intersect to see if the current box is one of them
            found = found || rbh.box.name == currentBBName;

        // If a triangle is already in the shadow at this datetime, no need throw ray again
        // (e.g the sun is low)
        if (found && datetimeSunInfo[raybox->id] == true)
        {
            raysBoxes.push_back(raybox);
        }
    }

    return raysBoxes;
}



std::vector<std::shared_ptr<Ray>> keepIntersectRaysWith(const std::string& currentBBName, const std::vector<std::shared_ptr<RayBox>>& allRayBoxes, std::map<int, bool>& datetimeSunInfo)
{
    std::vector<std::shared_ptr<Ray>> rays;//Not all rays intersect the box

    //We only get the rays that intersect with the box
    for (unsigned int i = 0; i < allRayBoxes.size(); i++)
    {
        auto raybox = allRayBoxes[i];//Get the ray

        bool found = false;

        for (RayBoxHit& rbh : raybox->boxes)//Go through all the boxes that the ray intersect to see if the current box is one of them
            found = found || rbh.box.name == currentBBName;

        // If triangle is already in the shadow at this datetime, no need throw ray again
        // (e.g the sun is low)
        if (found && datetimeSunInfo[raybox->id] == true)
            rays.push_back(raybox);
    }

    return rays;

}



void loadTriangleAndCheckIntersectionAndUpdateSunlightResult(const std::string& filepath, const CityObjectsType& fileType, const std::string& cityObjId,
                                                            const std::vector<std::shared_ptr<Ray>>& rayColl, std::map<int, bool>& datetimeSunInfo)
{
    //Get the triangle list of files matching intersected AABB
   std::vector<std::shared_ptr<Triangle>>* trianglesTemp = BuildTriangleList(filepath, fileType, cityObjId, rayColl.at(0)->origin.z);

    //Perform raytracing
    std::vector<Hit*>* tmpHits = RayTracing(trianglesTemp, rayColl, true);

    for (Hit* h : *tmpHits)
    {
        datetimeSunInfo[h->ray.id] = false;
    }

    //Delete triangles
    delete trianglesTemp;

    //Delete hits
    for (unsigned int i = 0; i < tmpHits->size(); ++i)
        delete tmpHits->at(i);

    delete tmpHits;
}



void computeSunlight(const std::string& fileDir, const std::vector<FileInfo>& filenames, const std::string& sunpathFile, const std::string& startDate, const std::string& endDate, const std::string& outputDir)
{
    Timer timer;
    timer.start();

    spdlog::info("Sunlight Calculation started.");

    //Create output folders
    createOutputFolders(outputDir);

    // Create a file rotating logger with 5 MB size max and 3 rotated files
    const auto LOG_FILE_PATH (outputDir + "Sunlight/logFile.txt");
    const auto LOGGER_NAME ("sunlight_detection");
    const auto MAX_LOG_SIZE = 1048576 * 5;
    const auto MAX_LOG_FILES = 3;

    // Verify if spdlog has already a logger define
    std::shared_ptr<spdlog::logger> fileLogger = spdlog::get(LOGGER_NAME);
    if (!fileLogger)
    {
        fileLogger = spdlog::rotating_logger_mt("sunlight_detection", LOG_FILE_PATH, MAX_LOG_SIZE, MAX_LOG_FILES);
    }


    //Convert dates to integer
    int iStartDate = encodeDateTime(startDate, 0);
    int iEndDate = encodeDateTime(endDate, 23);

    // *** Compute sun's beam direction from sunpathFile and associate them to an hour encoded as an int. *** //
    SunEarthToolsParser sunParser (sunpathFile, iStartDate, iEndDate);

    // *** Build datetime_sunnyMap : result map associating a datetime to sunny info *** //
    //This map is created once as the sun beams are always the same in one simulation and will be associated with each triangle
    std::map<int, bool> datetime_sunnyMap;

    for (auto const& beamdir : sunParser.getDirectionTowardsTheSun()) //For all sun beams
    {
        if (beamdir.second == TVec3d(0.0, 0.0, 0.0)) //If beam direction is nul, i.e. sun is down
            datetime_sunnyMap[beamdir.first] = false; //sunny = false
        else
            datetime_sunnyMap[beamdir.first] = true; // sunny = true
    }


    // *** Load AABB of all files *** //
    auto boxes = loadLayersAABBs(fileDir);
    if (!boxes.has_value())
    {
       spdlog::error("Missing Layer AABB files. Can't compute sunlight...");
       fileLogger->error("Missing Layer AABB files. Can't compute sunlight...");
       return;
    }

    //Concatenate buildingAABB and terrainAABB
    std::vector<AABB> layerBoundingBoxes;

    // Merge all layer bounding box
    layerBoundingBoxes.reserve(boxes.value().building.size() + boxes.value().ground.size());
    layerBoundingBoxes.insert(layerBoundingBoxes.end(), boxes.value().building.begin(), boxes.value().building.end());
    layerBoundingBoxes.insert(layerBoundingBoxes.end(), boxes.value().ground.begin(), boxes.value().ground.end());


    // *** Load files to analyse *** //

    unsigned int cpt_files = 1;
    double time_tot = 0.0;

    for (const auto& f : filenames) //Loop through files
    {
        spdlog::info("===================================================");
        spdlog::info("Computation of file {}...", f.withPrevFolderAndGMLExtension());
        spdlog::info("===================================================");

        //Log file
        const auto text ("File " + f.withPrevFolderAndGMLExtension());
        spdlog::info(text);
        fileLogger->info(text);

        //Load TriangleList of file to compute sunlight for
        std::vector<std::shared_ptr<Triangle>>* triangles {};

        if (f.getType() == CityGMLFileType::_BATI)
        {
           triangles = BuildTriangleList(f.getPath(), CityObjectsType::COT_Building);
        }
        else if (f.getType() == CityGMLFileType::_MNT)
        {
           triangles = BuildTriangleList(f.getPath(), CityObjectsType::COT_TINRelief);
        }
        else
        {
           spdlog::error("Unsupported file type at {}", f.getPath());
        }

        fileLogger->info("Triangles Number : {}", triangles->size());

        SunlightObjExporter resultExporter;
        //Create csv file where results will be written
        createFileFolder(f, outputDir);

        int cpt_tri = 1;//output print purpose

        for (auto it = triangles->begin(); it != triangles->end(); ++it) //Loop through each triangle
        {
           std::shared_ptr<Triangle> t = *it;

           spdlog::debug("Triangle {} of {}...", cpt_tri, triangles->size());

            //Initialize sunlight Info results
            std::map<int, bool> datetimeSunInfo = datetime_sunnyMap;

            //Compute Barycenter of triangle
            TVec3d barycenter ((t->a + t->b + t->c) / 3.0);

            //Create rayBoxCollection (All the rays for this triangle)
            std::vector<std::shared_ptr<RayBox>> raysBoxes ({});

            for (auto const& beamdir : sunParser.getDirectionTowardsTheSun())
            {
                //If direction is null (ie sun is too low) leave triangle in the shadow and go to next iteration
                if (beamdir.second == TVec3d(0.0, 0.0, 0.0))
                    continue;

                //if triangle is not oriented towards the sun, it is in the shadow
                if (t->GetNormal().dot(beamdir.second) < 0.0)
                {
                    datetimeSunInfo[beamdir.first] = false;
                    continue;
                }

                //Add an offset for raytracing. Without this offset, origin of the ray might be behind the barycenter,
                //which will result in a collision between the ray its origin triangle
                TVec3d tmpBarycenter ( barycenter + 0.01 * beamdir.second);

                //Add ray to list
                RayBox raybox = RayBox(tmpBarycenter, beamdir.second, beamdir.first);
                raysBoxes.push_back(std::make_shared<RayBox>(raybox));
            }

            //Setup and get the file's boxes in the right intersection order
            std::queue<RayBoxHit> fileOrder = intersectAndSortByDistance(layerBoundingBoxes, raysBoxes);

            //While we have boxes, files
            while (fileOrder.size() != 0)
            {
                //Get current AABB hit (starting from the closest)
                RayBoxHit myRayBoxHit = fileOrder.front();
                std::string fileName_boxhit = myRayBoxHit.box.name;
                fileOrder.pop();

                //Create file information variable
                std::string path_boxhit = fileDir + fileName_boxhit;
                FileInfo fBoxHit = FileInfo(path_boxhit);

                std::string cityObjId = "";

                if (fBoxHit.getType() == CityGMLFileType::_BATI) //If _BATI, Multi-Resolution
                {
                    //Create RayBoxes
                    auto rayboxBuilding = keepIntersectRaysBoxWith(fileName_boxhit, raysBoxes, datetimeSunInfo);

                    //Clear current boxes associated to rays because they correspond to AABB of tiles.
                    //We will now recompute the boxes associated to rays using one level deeper AABB (Building AABB)
                    for (auto& rb : rayboxBuilding)
                        rb->boxes.clear();

                    //Load Building AABB (B_AABB)
                    std::string path_B_AABB = fBoxHit.getPathForBoundingBox();
                    std::vector<AABB> B_AABB = loadAABBFile(path_B_AABB);

                    if (B_AABB.empty())
                    {
                       spdlog::error("Missing Building AABB file. Can't check intersection with {}...", path_B_AABB);
                       fileLogger->error("Missing Building AABB file. Can't check intersection with {}...", path_B_AABB);

                       rayboxBuilding.clear();
                       continue;
                    }

                    //Setup AABB (The sorting is not essential here, we could just go "inside" an AABB when an intersection is found).
                    std::queue<RayBoxHit> B_AABB_Order = intersectAndSortByDistance(B_AABB, rayboxBuilding);

                    while (B_AABB_Order.size() != 0) //For each intersected AABB
                    {
                        //Get current AABB hit (starting from the closest)
                        RayBoxHit currentBuildRayBoxHit = B_AABB_Order.front();
                        std::string fileName_BuildBoxhit = currentBuildRayBoxHit.box.name;
                        B_AABB_Order.pop();

                        std::string cityObjId = fileName_BuildBoxhit; //CityObj is the name of the Building intersected

                        auto raysTemp = keepIntersectRaysWith(fileName_BuildBoxhit, rayboxBuilding, datetimeSunInfo); //Create Rays

                        //If no rays, continue
                        if (raysTemp.size() == 0)
                        {
                            continue;
                        }

                        //Raytracing on current building (thanks to cityObjId)
                        loadTriangleAndCheckIntersectionAndUpdateSunlightResult(fBoxHit.getPath(), CityObjectsType::COT_Building, cityObjId, raysTemp, datetimeSunInfo);

                        raysTemp.clear();
                    }

                    rayboxBuilding.clear();
                }
                else if (fBoxHit.getType() == CityGMLFileType::_MNT) //If _MNT, no multiresolution
                {
                    auto raysTemp = keepIntersectRaysWith(fileName_boxhit, raysBoxes, datetimeSunInfo);

                    if (raysTemp.size() == 0)
                    {
                        continue;
                    }

                    loadTriangleAndCheckIntersectionAndUpdateSunlightResult(fBoxHit.getPath(), CityObjectsType::COT_TINRelief, cityObjId, raysTemp, datetimeSunInfo);

                    //Clear rays
                    raysTemp.clear();
                }
            }

            exportLightningToCSV(datetimeSunInfo, t, f, iStartDate, iEndDate, outputDir); //Export result for this triangle in csv files
            //resultExporter.exportResult(datetimeSunInfo, t, f, iStartDate, iEndDate, outputDir);

            raysBoxes.clear();

            ++cpt_tri;
        }

        //Delete TriangleList
        delete triangles;

        spdlog::info("===================================================");
        spdlog::info("file {} of {} done in : {}s", cpt_files, filenames.size(), timer.getElapsedInSeconds());
        spdlog::info("===================================================");

        //Log file
        fileLogger->info("Computation time : {}s \n", timer.getElapsedInSeconds());

        time_tot += timer.getElapsedInSeconds();
        timer.restart();
        ++cpt_files;
    }

    spdlog::info("Total time : {}s", time_tot);
}
