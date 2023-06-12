// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include "Tile.h"

#include <citygmls/Envelope.h>
#include <citygmls/ParserParams.h>
#include <citygmls/CityObject.h>
#include <citygmls/CityModel.h>
#include <citygmls/URI.h>

#include <parsers/ParserLibXml2.cpp>

Tile::Tile()
    : m_root(nullptr)
{
}

Tile::Tile(const std::string& filepath)
    : m_root(nullptr)
{
    load(filepath);
    //computeEnvelope();
}

Tile::~Tile()
{
    delete m_root;
}

/*Tile::Tile(const TVec3d& pMin, const TVec3d& pMax)
    : m_envelope(pMin, pMax)
{
}*/

Envelope& Tile::getEnvelope()
{
    //return m_envelope;
    return m_root->getEnvelope();
}

const Envelope& Tile::getEnvelope() const
{
    //return m_envelope;
    return m_root->getEnvelope();
}

void Tile::computeEnvelope()
{
    m_root->computeEnvelope();
    //m_envelope.merge(m_root->getEnvelope());
}

//void loadRec(CityObject* node, ReaderOsgCityGML& reader)
//{
//    //FIXME: remove this method since all its code is commented out
//
//    //node->computeEnvelope();
//    /*osg::ref_ptr<osg::Group> grp = reader.createCityObject(node);
//
//    if(node->getType() == COT_Building)
//    {
//        int yearOfConstruction = 0;
//        int yearOfDemolition = 0;
//
//        if(node->getAttribute("yearOfConstruction") != "")
//        {
//            std::istringstream(node->getAttribute("yearOfConstruction")) >> yearOfConstruction;
//            grp->setUserValue("yearOfConstruction", yearOfConstruction);
//        }
//        if(node->getAttribute("yearOfDemolition") != "")
//        {
//            std::istringstream(node->getAttribute("yearOfDemolition")) >> yearOfDemolition;
//            grp->setUserValue("yearOfDemolition", yearOfDemolition);
//        }*/
//
//
//
//
//        /*grp->getUserValue("yearOfConstruction", yearOfConstruction);
//        grp->getUserValue("yearOfDemolition", yearOfDemolition);*/
//
//        /*std::cout << "yearOfConstruction : " << yearOfConstruction << std::endl;
//        std::cout << "yearOfDemolition : " << yearOfDemolition << std::endl;
//
//        std::cout << "yearOfConstruction2 : " << node->getAttribute("yearOfConstruction") << std::endl;
//        std::cout << "yearOfDemolition2 : " << node->getAttribute("yearOfDemolition") << std::endl;*/
//        //}
//
//        //node->setOsgNode(grp);
//
//        /*CityObjects& cityObjects = node->getChildren();
//        CityObjects::iterator it = cityObjects.begin();
//        for( ; it != cityObjects.end(); ++it)
//        {
//            loadRec(*it, reader);
//        }*/
//}

void Tile::load(const std::string& filepath)
{
    ParserParams params;
    delete m_root;
    m_root = load(filepath, params);
    m_root->computeEnvelope();
    //std::cout << citygmlmodel->getEnvelope() << std::endl;

    m_citygmlFilePath = filepath;

    //FIXME: remove following commented out code
    // create osg geometry
    /*size_t pos = filepath.find_last_of("/\\");
    std::string path = filepath.substr(0, pos);
    ReaderOsgCityGML readerOsgGml(path);
    readerOsgGml.m_settings.m_useTextures = app().getSettings().m_loadTextures;*/

    /*CityObjects& cityObjects = m_root->getCityObjectsRoots();
    CityObjects::iterator it = cityObjects.begin();
    for( ; it != cityObjects.end(); ++it)
    {
        loadRec(*it, readerOsgGml);
    }*/

    ////////////////////////////////////////////


    /*CityObjectsMap& cityObjects = citygmlmodel->getCityObjectsMap();
    CityObjectsMap::iterator it = cityObjects.begin();
    for( ; it != cityObjects.end(); ++it)
    {
        CityObjects& cityObject = it->second;
        CityObjects::iterator itObj = cityObject.begin();
        for( ; itObj != cityObject.end(); ++itObj)
        {
            osg::ref_ptr<osg::Group> node = readerOsgGml.createCityObject(*itObj);
            //node->setUserDataContainer(new osg::DefaultUserDataContainer);

            int yearOfConstruction;
            int yearOfDemolition;

            std::istringstream((*itObj)->getAttribute("yearOfConstruction")) >> yearOfConstruction;
            std::istringstream((*itObj)->getAttribute("yearOfDemolition")) >> yearOfDemolition;

            node->setUserValue("yearOfConstruction", yearOfConstruction);
            node->setUserValue("yearOfDemolition", yearOfDemolition);



            node->getUserValue("yearOfConstruction", yearOfConstruction);
            node->getUserValue("yearOfDemolition", yearOfDemolition);

            std::cout << "yearOfConstruction : " << yearOfConstruction << std::endl;
            std::cout << "yearOfDemolition : " << yearOfDemolition << std::endl;

            std::cout << "yearOfConstruction2 : " << (*itObj)->getAttribute("yearOfConstruction") << std::endl;
            std::cout << "yearOfDemolition2 : " << (*itObj)->getAttribute("yearOfDemolition") << std::endl;

            //(*itObj)->getOsgNode() = node;
            (*itObj)->setOsgNode(node);
        }
    }*/

    // set tile name
    static int id = 0;
    std::stringstream ss;
    std::string filename = filepath.substr(filepath.find_last_of("/\\") + 1);
    ss << id++ << "_" << filename;
    m_name = ss.str();
}

CityModel* Tile::getCityModel()
{
    return m_root;
}

const CityModel* Tile::getCityModel() const
{
    return m_root;
}

const std::string& Tile::getName() const
{
    return m_name;
}

void Tile::setName(const std::string& name)
{
    m_name = name;
}

/*osg::ref_ptr<osg::Node> Tile::getOsgRoot()
{
    return m_rootOsg;
}*/

CityObject* getNodeRec(CityObject* node, const std::string& name)
{
    CityObject* res = nullptr;
    if (node->getId() == name)
    {
        //std::cout << "found node " << name << std::endl;
        return node;
    }

    CityObjects& cityObjects = node->getChildren();
    CityObjects::iterator it = cityObjects.begin();
    for (; it != cityObjects.end(); ++it)
    {
        res = getNodeRec(*it, name);
        if (res) break;
    }

    return res;
}

// FIXME: Remove this method
/*CityObject* Tile::findNode(const std::string& name)
{
    CityObject* res = NULL;

    CityObjects& cityObjects = m_root->getCityObjectsRoots();
    CityObjects::iterator it = cityObjects.begin();
    for( ; it != cityObjects.end(); ++it)
    {
        res = findNodeRec(*it, name);
        if(res) break;
    }

    return res;
}*/

void Tile::deleteNode(const std::string& /*name*/)
{

}

void Tile::insertNode(CityObject* /*node*/)
{

}

void Tile::replaceNode(const std::string& /*name*/, CityObject* /*node*/)
{

}

CityObject* Tile::getNode(const URI& uri, bool inPickingMode)
{
    return m_root->getNode(uri, inPickingMode);
    // FIXME: Remove the following commented out code
    /*std::string name = uri.getLastNode();

    CityObject* res = nullptr;

    CityObjects& cityObjects = m_root->getCityObjectsRoots();
    CityObjects::iterator it = cityObjects.begin();
    for( ; it != cityObjects.end(); ++it)
    {
        res = getNodeRec(*it, name);
        if(res) break;
    }

    return res;*/
}

const std::string& Tile::getCityGMLfilePath() const
{
    return m_citygmlFilePath;
}