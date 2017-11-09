//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

#include "messageflow.hpp"
#include "apiqueryresult.hpp"
#include "collectable.hpp"
#include "configuration.hpp"
#include "exception.hpp"
#include "localization.hpp"
#include "generic.hpp"
#include "querypool.hpp"
#include "syslog.hpp"
#include "wikisite.hpp"
#include "wikiuser.hpp"
#include <QUrl>

using namespace Huggle;

MessageFlow::MessageFlow(WikiUser *target, QString MessageText, QString MessageSummary) : Message(target, MessageText, MessageSummary)
{
    // Flow doesn't really care about this
    this->RequireFresh = false;
}

MessageFlow::~MessageFlow()
{
    delete this->User;
}

bool MessageFlow::Done()
{
    return false;
}

void MessageFlow::Fail(QString reason)
{

}


