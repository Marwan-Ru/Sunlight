#include <vector>
#include <string>
#include <cassert>
#include <spdlog/spdlog.h>

#include <cores/SunlightDetection.h>
#include <cores/FileInfo.h>
#include <utils/Utils.h>

namespace fs = std::filesystem;

int main()
{
   ///// <summary>
   ///// Testing if the computed output is identical to the expected output (old 3DUSE)
   ///// </summary>
   const std::string rootDirectory(".\\datas\\LYON_3EME_2018\\");
   const std::string sunlightCsv(".\\datas\\AnnualSunPath_Lyon.csv");
   const std::vector<FileInfo*> testingFiles(getAllGmlTilesIn(rootDirectory));

   int BENCHMARK_RUN(30);
   for (size_t i = 1; i <= BENCHMARK_RUN; i++)
   {
      spdlog::info("Compute {} on {}", i, BENCHMARK_RUN);
      // Compute with the current version of sunlight computation
      computeSunlight(rootDirectory, testingFiles, sunlightCsv, "2016-01-01", "2016-01-01", rootDirectory);
   }

    ///// Compare computed and expected output
    //const std::string computedSunlight(rootDirectory + "Sunlight\\_BATI\\3689_10348_BATI\\2016-01-01.csv");
    //const std::string expectedOutput(rootDirectory + "SunlightBeforeRefactoring\\_BATI\\3689_10348_BATI\\2016-01-01.csv");
    //assert(areFilesIdentical(computedSunlight, expectedOutput) && "Shading calculation differs from original.");
}
