/* -*-c++-*- libcitygml - Copyright (c) 2010 Joachim Pouderoux, BRGM
*
* This file is part of libcitygml library
* http://code.google.com/p/libcitygml
*
* libcitygml is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 2.1 of the License, or
* (at your option) any later version.
*
* libcitygml is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*/

#pragma once

#include <vector>
#include <map>
#include <ostream>

#include <maths/Vectors.h>
#include <citygmls/Object.h>
#include <citygmls/CityObject.h>
#include "AppearanceManager.h"

#include <ADE/temporal/Version.h>
#include <ADE/temporal/VersionTransition.h>
#include <ADE/temporal/Workspace.h>

#include <ADE/document/DocumentObject.h>
#include <ADE/document/Reference.h>

#ifdef _MSC_VER                // Inhibit dll-interface warnings concerning
#pragma warning(disable: 4251) // export problem on STL members
#endif

class Envelope;
class URI;
class ParserParams;

typedef std::vector< CityObject* > CityObjects;
typedef std::map< CityObjectsType, CityObjects > CityObjectsMap;

class CityModel : public Object
{
    friend class CityGMLHandler;
public:
    CityModel( const std::string& id = "CityModel" );

    ~CityModel() override;

    // Return the envelope (ie. the bounding box) of the model
    const Envelope& getEnvelope() const;
    Envelope& getEnvelope();

    // Return the translation parameters of the model
    const TVec3d& getTranslationParameters() const;

    // Get the number of city objects
    size_t size() const;

    const CityObjectsMap& getCityObjectsMap() const;
    CityObjectsMap& getCityObjectsMap();

    const CityObjects* getCityObjectsByType( CityObjectsType type ) const;

    // Return the roots elements of the model. You can then navigate the hierarchy using object->getChildren().
    const CityObjects& getCityObjectsRoots() const;
    CityObjects& getCityObjectsRoots();

    const std::string& getSRSName() const;

    void computeEnvelope();

    AppearanceManager* getAppearanceManager();

    /// Add a direct child
    void addCityObjectAsRoot( CityObject* o );

    /// Add a CityObject to the model (used for finish method for example)
    ///
    /// A CityObject should always be added on the CityObject and on the master CityModel
    ///
    /// Example usage : model is the CityModel, obj is a CityObject to add to a Wall
    /// \code{.cpp}
    /// model->addCityObject(obj);
    /// wall->insertNode(obj);
    /// \endcode
    void addCityObject( CityObject* o );

    /// Get node by uri
	CityObject* getNode(const URI& uri, bool inPickingMode=false);

    /// Get node by name
    CityObject* getNodeById(const std::string& id);

    void finish( const ParserParams& );

    std::string m_basePath;

	void setVersions(std::vector<temporal::Version*>,std::vector<temporal::VersionTransition*>);
	const std::vector<temporal::Version*> getVersions() const;
	std::vector<temporal::Version*> getVersions();
	std::vector<temporal::VersionTransition*> getTransitions();
	
	void setWorkspaces(std::map<std::string,temporal::Workspace>);
    void setDocuments( std::vector<documentADE::DocumentObject*> );
    void setReferences( std::vector<documentADE::Reference*> );
	const std::map<std::string,temporal::Workspace> getWorkspaces() const;
    const std::vector<documentADE::DocumentObject *> getDocuments() const;
    const std::vector<documentADE::Reference *> getReferences() const;
    std::map<std::string,temporal::Workspace> getWorkspaces();

protected:
    Envelope _envelope;

    CityObjects _roots;

    CityObjectsMap _cityObjectsMap;

    AppearanceManager _appearanceManager;

    std::string _srsName;

    TVec3d _translation;

	std::vector<temporal::Version*> _versions;
	std::vector<temporal::VersionTransition*> _versionTransitions;
	std::map<std::string,temporal::Workspace> _workspaces;
    std::vector<documentADE::DocumentObject*> _documents;
    std::vector<documentADE::Reference*> _references;
};

std::ostream& operator<<( std::ostream&, const CityModel & );
