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

#ifdef _MSC_VER                  // Inhibit dll-interface warnings concerning
  #pragma warning(disable: 4251) // export problem on STL members
  #	include <windows.h>
#else
  #	define CALLBACK
  #	define APIENTRY
#endif

#ifdef __APPLE__
  #include <OpenGL/glu.h>
#else

#endif

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
// Win32 caveat emptor: must always be _preceded_ (as opposed to
// succeeded) by the inclusion of windows.h
#include <gl/GLU.h>

// GLU based polygon tesselator
class Tesselator 
{		
public:
	Tesselator(); 
	~Tesselator();

	void init( size_t verticesCount, const glm::highp_dvec3& normal, GLenum winding_rule = GLU_TESS_WINDING_ODD );

	// Add a new contour - add the exterior ring first, then interiors 
	void addContour( const std::vector<glm::highp_dvec3>&, const std::vector<glm::vec2>& );

	// Let's tesselate!
	void compute();

	// Tesselation result access
	inline const std::vector<glm::highp_dvec3>& getVertices() const { return _vertices; }
	inline const std::vector<glm::vec2>& getTexCoords() const { return _texCoords; }
	inline const std::vector<unsigned int>& getIndices() const { return _indices; }

private:
	typedef void (APIENTRY *GLU_TESS_CALLBACK)();
	static void CALLBACK beginCallback( GLenum, void* );
	static void CALLBACK vertexCallback( GLvoid*, void* );
	static void CALLBACK combineCallback( GLdouble[3], void* [4], GLfloat [4], void** , void* );
	static void CALLBACK endCallback( void* );
	static void CALLBACK errorCallback( GLenum, void* );	

private:
	GLUtesselator *_tobj;
	GLenum  _curMode;

	std::vector<glm::highp_dvec3> _vertices;		
	std::vector<glm::vec2> _texCoords;
	std::vector<unsigned int> _indices;

	std::vector<unsigned int> _curIndices;
};