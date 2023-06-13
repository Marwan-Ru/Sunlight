// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include <citygmls/CityObject.h>

namespace temporal
{

enum TransactionValue
{
	INSERT,
	DEL,
	REPLACE
};
class Transaction: public Object
{
public:
	Transaction(const std::string& id);
	void setType(TransactionValue);
	void setOldFeature(CityObject*);
	void setNewFeature(CityObject*);
private:
	TransactionValue _type;
	CityObject* _newFeature;
	CityObject* _oldFeature;
};

} //namespace temporal