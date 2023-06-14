#include <CppUnitTest.h>
#include <vector>
#include <string>
#include <cassert>

#include <cores/SunlightDetection.h>
#include <cores/FileInfo.h>
#include <utils/Utils.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SunlightTest
{
    TEST_CLASS(Output)
    {
    public:
        /*
        *
        * Testing if the computed output is identical to the expected output (old 3DUSE)
        *
        */
        TEST_METHOD(CompareWith3DUse)
        {
            const std::string rootDirectory("..\\..\\datas\\");
            const std::string sunlightCsv(rootDirectory + "AnnualSunPath_Lyon.csv");
            const std::vector<FileInfo*> testingFiles({ new FileInfo(rootDirectory + "_BATI\\26_HOTEL_POLICE_LYON.gml") });

            // Compute with the current version of sunlight computation
            SunlightDetection(rootDirectory, testingFiles, sunlightCsv, "2016-01-01", "2016-01-01", rootDirectory);

            // Compare computed and expected output
            const std::string computedSunlight(rootDirectory + "Sunlight\\_BATI\\26_HOTEL_POLICE_LYON\\2016-01-01.csv");
            const std::string expectedOutput(rootDirectory + "3DUSE_Output\\_BATI\\26_HOTEL_POLICE_LYON\\2016-01-01.csv");
            Assert::IsTrue(areFilesIdentical(computedSunlight, expectedOutput), L"Shading calculation differs from original.");
        }
    };
}