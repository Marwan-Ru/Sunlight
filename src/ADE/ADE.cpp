// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include "ADE.h"
#include "temporal/TemporalHandler.h"
#include "document/DocumentHandler.h"

//  JE 17/02/2016: DISCARDED BECAUSE PROBLEMS WITH RECENT VERSIONS OF UBUNTU: 
//instanciate the ADE Handler constructors map
//ADEHandlerFactory::mapType * ADEHandlerFactory::ADEmap = new mapType();

// JE 17/02/2016: new ADE handler construction method
// ADD YOUR NEW ADE PARSERS HERE !
void ADEHandlerFactory::getInstances(std::map<std::string,ADEHandler*>* map)
{
    (*map)["temp"]=new TempHandler();
    (*map)["doc"]=new DocumentHandler();
}

//some gmlHandler methods
std::string ADEHandler::getGmlIdAttribute( void* attributes ) {
    return getAttribute( attributes, "gml:id", "" ); }
unsigned int ADEHandler::getPathDepth() { return static_cast<unsigned int>(getNodePath()->size()); }
void ADEHandler::pushCityObject( CityObject* object )
{
    CityObject** currentCityObject = getCurrentCityObject();
    // add parent relation
    if(*currentCityObject)
    {
        object->_parent = *currentCityObject;
    }

    if ( *currentCityObject && object ) (*currentCityObject)->getChildren().push_back( object );
    std::stack<CityObject*>* cityObjectStack = getCityObjectStack();
    cityObjectStack->push( *currentCityObject );
    *currentCityObject = object;
}
void ADEHandler::pushObject( Object* object )
{
    std::stack<Object*>* objectStack = getObjectStack();
    objectStack->push( object );
    Object** currentObject = getCurrentObject();
    *currentObject = object;
}
void ADEHandler::popCityObject()
{
    CityObject** currentCityObject = getCurrentCityObject();
    std::stack<CityObject*>* cityObjectStack = getCityObjectStack();
    *currentCityObject = 0; 
    if ( cityObjectStack->empty() ) return; 
    *currentCityObject = cityObjectStack->top(); 
    cityObjectStack->pop();
}
void ADEHandler::popObject()
{
    Object** currentObject = getCurrentObject();
    std::stack<Object*>* objectStack = getObjectStack();
    *currentObject = 0; 
    if ( objectStack->empty() ) return; 
    objectStack->pop();
    *currentObject = objectStack->empty() ? 0 : objectStack->top();			
}
