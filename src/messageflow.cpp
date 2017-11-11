//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

#include "messageflow.hpp"
#include "apiquery.hpp"
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

void MessageFlow::Send()
{
    // First of all we need to check if user has flow on their page
    // Create an API query to get the information about talk page
    this->qCheck = new ApiQuery(ActionQuery, this->User->GetSite());
    this->qCheck->Parameters = "titles=" + QUrl::toPercentEncoding(this->User->GetTalk()) + "&prop=info";
    this->qCheck->Target = "Checking if " + this->User->Username + " has flow enabled";
    HUGGLE_QP_APPEND(this->qCheck);
    this->qCheck->Process();
}

bool MessageFlow::IsFinished()
{
    if (this->IsFailed())
        return true;

    if (this->userPageInfoFinished)
        return Message::IsFinished();

    // We don't know yet if target is using flow or not, let's check if our query that was supposed to check it is finished yet
    if (this->qCheck->IsProcessed())
    {
        if (this->qCheck->IsFailed())
        {
            // The query failed for some reason
            this->Fail("Can't check flow usage: " + this->qCheck->GetFailureReason());
            return true;
        }
        // Let's check if target user is using flow or not
        ApiQueryResultNode *pages = this->qCheck->GetApiQueryResult()->GetNode("pages");
        if (pages == nullptr || pages->ChildNodes.isEmpty())
        {
            this->Fail("Can't check flow usage: no page info returned by wiki");
            return true;
        }
        ApiQueryResultNode *page_meta = pages->ChildNodes.at(0);
        QString content_model = page_meta->GetAttribute("contentmodel", "unknown");
        HUGGLE_DEBUG("Content model for " + this->User->GetTalk() + ": " + content_model, 3);
        this->userPageInfoFinished = true;
        if (content_model == "flow-board")
        {
            // Yikes, user has flow!
            this->usingFlow = true;
            throw new Huggle::Exception("Flow code not yet implemented", BOOST_CURRENT_FUNCTION);
            return false;
        } else
        {
            this->usingFlow = false;
            // Let's fallback to standard MW message
            Message::Send();
        }
        return false;
    }
    // It's still running
    return false;
}

bool MessageFlow::Done()
{
    return false;
}

void MessageFlow::Fail(QString reason)
{
    Message::Fail(reason);
}


