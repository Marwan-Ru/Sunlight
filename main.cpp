// main.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <vector>
#include <string>
#include <cassert>

#include <cores/SunlightDetection.h>
#include <cores/FileInfo.h>
#include <utils/Utils.h>

namespace fs = std::filesystem;

int main()
{
    /// <summary>
    /// Testing if the computed output is identical to the expected output (old 3DUSE)
    /// </summary>
    const std::string rootDirectory(".\\datas\\");
    const std::string sunlightCsv(rootDirectory + "AnnualSunPath_Lyon.csv");
    const std::vector<FileInfo*> testingFiles({ new FileInfo(rootDirectory + "_BATI\\26_HOTEL_POLICE_LYON.gml") });

    // Compute with the current version of sunlight computation
    SunlightDetection(rootDirectory, testingFiles, sunlightCsv, "2016-01-01", "2016-01-01", rootDirectory);

    // Compare computed and expected output
    const std::string computedSunlight(rootDirectory + "Sunlight\\_BATI\\26_HOTEL_POLICE_LYON\\2016-01-01.csv");
    const std::string expectedOutput(rootDirectory + "3DUSE_Output\\_BATI\\26_HOTEL_POLICE_LYON\\2016-01-01.csv");
    assert(areFilesIdentical(computedSunlight, expectedOutput) && L"Shading calculation differs from original.");
}
