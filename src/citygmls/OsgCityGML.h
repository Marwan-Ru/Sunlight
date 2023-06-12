//// Copyright University of Lyon, 2012 - 2017
//// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
//// (Refer to accompanying file LICENSE.md or copy at
////  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )
//
//#pragma once
//
//#include <osgDB/Registry>
//#include <osgDB/ReadFile>
//#include <osgDB/WriteFile>
//#include <osg/Texture2D>
//#include "citygml.hpp"
//
//#include "ParserParams.h"
//
//class CityObject;
//
//class ReaderOsgCityGML
//{
//public:
//    class Settings
//    {
//    public:
//        Settings();
//
//    public:
//        ParserParams _params;
//        bool _printNames;
//        bool _first;
//        bool _useMaxLODOnly;
//        bool _recursive;
//        bool m_useTextures;
//        osg::Vec3 _origin;
//        std::map< std::string, osg::Texture2D* > _textureMap;
//        std::string m_filepath;
//    };
//
//    ReaderOsgCityGML(const std::string& filepath);
//    osg::ref_ptr<osg::Node> readNode(const citygml::CityObject* citygml);
//
//    static unsigned int getHighestLodForObject(const CityObject * object);
//
//    //private:
//    osg::ref_ptr<osg::Node> readCity(const citygml::CityModel*);
//    osg::ref_ptr<osg::Group> createCityObject(const citygml::CityObject*, unsigned int minimumLODToConsider = 0);
//
//public:
//    Settings m_settings;
//};
