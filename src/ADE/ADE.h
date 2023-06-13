// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <parsers/Parser.h>

// Base ADE Handler class, containing methods to access the CityGMLHandler members
class ADEHandler
{
public:
	ADEHandler(void){gmlHandler=NULL;}
	ADEHandler(CityGMLHandler* gHandler){gmlHandler=gHandler;}
    virtual ~ADEHandler(void){};
	void setGMLHandler(CityGMLHandler* gHandler){gmlHandler=gHandler;}
	virtual void startElement(std::string,void*){};
	virtual void endElement(std::string){};
	virtual void endDocument(){};
protected:
	CityGMLHandler* gmlHandler;
	//access to gmlHandler members
	std::map< std::string, CityGMLNodeType >* getCityGMLNodeTypeMap(){return &(gmlHandler->s_cityGMLNodeTypeMap);}
	std::vector< std::string >* getKnownNamespace(){return &(gmlHandler->s_knownNamespace);}
	std::vector< std::string >* getNodePath(){return &(gmlHandler->_nodePath);}
	std::stringstream* getBuff(){return &(gmlHandler->_buff);}
	ParserParams* getParams(){return &(gmlHandler->_params);}
	CityModel** getModel(){return &(gmlHandler->_model);}
	TVec3d* getTranslate(){return &(gmlHandler->_translate);}
	CityObject** getCurrentCityObject(){return &(gmlHandler->_currentCityObject);}
	std::stack<CityObject*>* getCityObjectStack(){return &(gmlHandler->_cityObjectStack);}
	Object** getCurrentObject(){return &(gmlHandler->_currentObject);}
	std::stack<Object*>* getObjectStack(){return &(gmlHandler->_objectStack);}
	Geometry** getCurrentGeometry(){return &(gmlHandler->_currentGeometry);}
	std::set<Geometry*>* getGeometries(){return &(gmlHandler->_geometries);}
	Polygon** getCurrentPolygon(){return &(gmlHandler->_currentPolygon);}
	LinearRing** getCurrentRing(){return &(gmlHandler->_currentRing);}
	Appearance** getCurrentAppearance(){return &(gmlHandler->_currentAppearance);}
	CityObjectsTypeMask* getObjectsMask(){return &(gmlHandler->_objectsMask);}
	std::string* getAttributeName(){return &(gmlHandler->_attributeName);}
	int* getCurrentLOD(){return &(gmlHandler->_currentLOD);}
	bool* getFilterNodeType(){return &(gmlHandler->_filterNodeType);}
	unsigned int* getFilterDepth(){return &(gmlHandler->_filterDepth);} // MT (MAC OS X problem...)
	std::vector<TVec3d>* getPoints(){return &(gmlHandler->_points);}
	int* getSrsDimension(){return &(gmlHandler->_srsDimension);}
	char* getOrientation(){return &(gmlHandler->_orientation);}
	bool* getExterior(){return &(gmlHandler->_exterior);}
	bool* getAppearanceAssigned(){return &(gmlHandler->_appearanceAssigned);}
	GeometryType* getCurrentGeometryType(){return &(gmlHandler->_currentGeometryType);}
	void** getGeoTransform(){return &(gmlHandler->_geoTransform);}

	//some gmlHandler methods
	virtual std::string getAttribute( void*, const std::string&, const std::string&){return "";}
	std::string getGmlIdAttribute( void* attributes );
	unsigned int getPathDepth();
	void pushCityObject( CityObject* );
	void pushObject( Object* );
	void popCityObject();
	void popObject();
};
///////////////////
//
//  Derived ADE handlers management :
//
//  JE 17/02/2016: DISCARDED BECAUSE PROBLEMS WITH RECENT VERSIONS OF UBUNTU: 
//	inspired from http://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name
//
//
////template for creating a handler of the given type
//template<typename T> ADEHandler* createT() {return new T();};
//
////Contains the map for the derived ADE handlers
//struct ADEHandlerFactory
//{
//	typedef std::map<std::string,ADEHandler*(*)()> mapType;
//
//	static ADEHandler* createInstance(std::string const& s) 
//	{
//		mapType::iterator it = getMap()->find(s);
//		if(it == getMap()->end())
//			return 0;
//		return it->second();
//    }
//
//	static void getInstances(std::map<std::string,ADEHandler*>& map)
//	{
//		for(mapType::iterator it = getMap()->begin(); it != getMap()->end(); it++) map[it->first] = it->second();
//	}
//
//protected:
//	static mapType* getMap() {if (!ADEmap) {ADEmap = new mapType();} return ADEmap;}
//
//private:
//	static mapType* ADEmap;
//};
//
////template for registring each ADE handler in the ADEHandlerFactory map
//template<typename T> struct ADERegister:ADEHandlerFactory
//{
//	ADERegister(const std::string s)
//	{
//		std::pair<std::string,ADEHandler*(*)()> entry (s,&createT<T>);
//		getMap()->insert(entry);
//	}
//};

class ADEHandlerFactory
{
public:
	void getInstances(std::map<std::string,ADEHandler*>*); 
};