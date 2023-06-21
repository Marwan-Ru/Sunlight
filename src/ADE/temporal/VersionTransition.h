// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#pragma once

#include "Transaction.h"
#include "Version.h"

namespace temporal
{
enum TransitionValue
{
    PLANNED,
    REALIZED,
    HISTORICAL_SUCCESSION,
    FORK,
    MERGE
};

class VersionTransition : public Object
{
public:
    VersionTransition (const std::string& id);
    virtual ~VersionTransition() override;

    void setReason(std::string);
    void setClone(bool);
    void setType(TransitionValue);
    void setFrom(Version*);
    Version* from();
    void setTo(Version*);
    Version* to();

    void addTransaction(Transaction*);
    std::vector<Transaction*>* getTransactions();

protected:

    bool _clonePredecessor;
    std::string _reason;
    TransitionValue _type;

    Version* _from;
    Version* _to;

    std::vector<Transaction*> _transactions;
};

} //namespace temporal
