// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <libxml/parser.h>
#include <libxml/SAX.h>
#include <libxml/xlink.h>
#include <libxml/xpath.h>
// String comparaison with strcmp
#include <cstring>

#include "DocumentHandler.h"
#include <citygmls/CityGml.h>
#include <utils/Utils.h>

DocumentHandler::DocumentHandler(void):ADEHandler()
{

}

DocumentHandler::DocumentHandler(CityGMLHandler* gHandler):ADEHandler(gHandler)
{

}

std::string DocumentHandler::getAttribute( void* attributes, const std::string& attname, const std::string& defvalue = "" )
{
    const xmlChar **attrs = (const xmlChar**)attributes;
    if ( !attrs ) return defvalue;
    for ( int i = 0; attrs[i] != 0; i += 2 ) {
        std::cout << "attr val: " << attrs[i] <<std::endl;
        if ( (const char*)( attrs[i] ) == attname ) return (const char*)( attrs[ i + 1 ] );
    }
    return defvalue;
}

std::string DocumentHandler::removeNamespace(std::string name)
{
    size_t pos = name.find_first_of( ":" );
    return name.substr( pos + 1 );
}

std::string DocumentHandler::getIDfromQuery(std::string query)
{
    size_t pos1 = query.find("//*[@id='");
    size_t pos2 = query.find("']",pos1);
    if (pos1!=std::string::npos && pos2!=std::string::npos)
    {
        return query.substr(pos1+9,pos2-(pos1+9));
    }
    if (query.find("#")==0)
    {
        return query.substr(1);
    }
    return "";
}

//////////////////////
//
// Parsing routines

void DocumentHandler::startElement(std::string name, void* attributes)
{
   // Transform name in uppercase to compare with other string without taking account case issue
    name = removeNamespace(name);
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::toupper(c); });

    if (strcmp(name.c_str(), "DOCUMENTOBJECT") == 0)
    {
       _currentDocument = new documentADE::DocumentObject(getGmlIdAttribute( attributes ));
        std::cout << "document object: " <<_currentDocument->getId()<< std::endl;
        _documents.push_back(_currentDocument);
        pushObject(_currentDocument);
    }
    else if (strcmp(name.c_str(), "REFERENCE") == 0)
    {
        _currentReference = new documentADE::Reference(getGmlIdAttribute( attributes ));
        std::cout << "reference: " <<_currentReference->getId()<< std::endl;
        _references.push_back(_currentReference);
    }
    else if (strcmp(name.c_str(), "TAG") == 0)
    {
       _currentTag = new documentADE::Tag(getGmlIdAttribute( attributes ));
        std::cout << "tag: " <<_currentTag->getId()<< std::endl;
    }
    else if (strcmp(name.c_str(), "REFERRINGTO") == 0)
    {
        GenericCityObject* cityObject = new  GenericCityObject(getGmlIdAttribute( attributes ));
       _currentReference->setReferencedCityObject(cityObject);

    }
    else if (strcmp(name.c_str(), "REFERREDBY") == 0)
    {
        documentADE::DocumentObject* document = new documentADE::DocumentObject(getGmlIdAttribute( attributes ));
       _currentReference->setReferenceDocument(document);
    }
}
/******************************************************/
void DocumentHandler::setAttributeValue(std::string name)
{
    Object** currentObject = getCurrentObject();
    std::stringstream buffer;
    buffer << trim(getBuff()->str());
    std::cout << name << ": " << buffer.str() << std::endl;
    if ( *currentObject ) (*currentObject)->setAttribute( name, buffer.str(), false );
}
/******************************************************/
void DocumentHandler::setDocumentAttributeValue(std::string name)
{
    std::stringstream buffer;
    buffer << trim(getBuff()->str());
    std::cout << name << ": " << buffer.str() << std::endl;
    _currentDocument->setAttribute( name, buffer.str(), false );
}
/******************************************************/

void DocumentHandler::endElement(std::string name)
{
   // Transform name in uppercase to compare with other string without taking account case issue
   name = removeNamespace(name);
   std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::toupper(c); });

    if (strcmp(name.c_str(), "DOCUMENTOBJECT") == 0)
    {
        CityModel** model = getModel();
        (*model)->addCityObjectAsRoot(_currentDocument);
    }
    else if (strcmp(name.c_str(), "REFERENCE") == 0)
    {
       _currentReference = nullptr;
    }
    else if (strcmp(name.c_str(), "TITLE") == 0 ||
            strcmp(name.c_str(), "IDENTIFIER") == 0 ||
             strcmp(name.c_str(), "CREATOR") == 0 ||
             strcmp(name.c_str(), "PUBLICATIONDATE") == 0)
    {
        setDocumentAttributeValue(name);
    }
    else if (strcmp(name.c_str(), "TEXT") == 0)
    {
        std::stringstream buffer;
        buffer << trim(getBuff()->str());
        std::cout << name << ": " << buffer.str() << std::endl;
        _currentTag->setText(buffer.str());
    }
    else if (strcmp(name.c_str(), "COUNT") == 0)
    {
        std::stringstream buffer;
        buffer << trim(getBuff()->str());
        std::cout << name << ": " << buffer.str() << std::endl;
        int count;
        std::stringstream s_str( buffer.str() );
        s_str>>count;
        _currentTag->setCount(count);
    }
}
/******************************************************/
void DocumentHandler::endDocument()
{
        CityModel** model = getModel();
        (*model)->setDocuments(_documents);
        (*model)->setReferences(_references);
}
