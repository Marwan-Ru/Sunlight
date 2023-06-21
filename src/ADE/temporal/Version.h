// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <citygmls/CityObject.h>

#ifdef _MSC_VER                // Inhibit dll-interface warnings concerning
#pragma warning(disable: 4251) // export problem on STL members
#endif

namespace temporal
{
class TempHandler;

class Version : public Object
{
   friend class TempHandler;
public :

   Version(const std::string& id);

   const std::vector<std::string>& getTags();
   void addTag( std::string );

   std::vector<CityObject*>* getVersionMembers();

   void addMember(CityObject*);

protected :
   std::vector<std::string> _tags;
   std::vector<CityObject*> _versionMembers;

};

} //namespace temporal
