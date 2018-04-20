//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

#include "messageflow.hpp"
#include <huggle_core/apiquery.hpp>
#include <huggle_core/apiqueryresult.hpp>
#include <huggle_core/collectable.hpp>
#include <huggle_core/configuration.hpp>
#include <huggle_core/exception.hpp>
#include <huggle_core/localization.hpp>
#include <huggle_ui/mainwindow.hpp>
#include <huggle_ui/history.hpp>
#include <huggle_core/historyitem.hpp>
#include <huggle_core/generic.hpp>
#include <huggle_core/querypool.hpp>
#include <huggle_core/syslog.hpp>
#include <huggle_core/wikisite.hpp>
#include <huggle_core/wikiuser.hpp>
#include <QUrl>

using namespace Huggle;

MessageFlow::MessageFlow(WikiUser *target, QString MessageText, QString MessageSummary) : Message(target, MessageText, MessageSummary)
{

}

MessageFlow::~MessageFlow()
{

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
    if (this->isFinished)
        return true;

    if (this->IsFailed())
        return true;

    if (this->userPageInfoFinished && !this->usingFlow)
        return Message::IsFinished();

    // We don't know yet if target is using flow or not, let's check if our query that was supposed to check it is finished yet
    if (!this->userPageInfoFinished && this->qCheck->IsProcessed())
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
            this->qFlowNewMessage = new ApiQuery(ActionCustom, this->User->GetSite());
            this->qFlowNewMessage->Target = "Delivering message to " + this->User->Username;
            this->qFlowNewMessage->UsingPOST = true;
            this->qFlowNewMessage->SetCustomActionPart("flow", true, true, false);
            this->qFlowNewMessage->Parameters = "submodule=new-topic&page=" + QUrl::toPercentEncoding(this->User->GetTalk()) +
                    "&nttopic=" + QUrl::toPercentEncoding(this->Title) + "&ntcontent=" + QUrl::toPercentEncoding(this->Text) +
                    "&ntformat=wikitext&token=" + QUrl::toPercentEncoding(this->User->GetSite()->GetProjectConfig()->Token_Csrf);
            HUGGLE_QP_APPEND(this->qFlowNewMessage);
            this->qFlowNewMessage->Process();
            return false;
        } else
        {
            this->usingFlow = false;
            // Let's fallback to standard MW message
            Message::Send();
        }
        return false;
    }

    if (this->usingFlow && this->qFlowNewMessage->IsProcessed())
    {
        if (this->qFlowNewMessage->IsFailed())
        {
            // Check if error isn't "bad token"
            ApiQueryResultNode *error = this->qFlowNewMessage->GetApiQueryResult()->GetNode("error");
            if (error && error->GetAttribute("code") == "badtoken")
            {
                Configuration::Logout(this->qFlowNewMessage->GetSite());
                // We have to exit here because we don't know the token for query
                this->Fail(_l("editquery-invalid-token", this->User->GetTalk()));
                return true;
            }
            this->Fail("Unable to send message to user: " + this->qFlowNewMessage->GetFailureReason());
            return true;
        }
        HistoryItem *item = new HistoryItem();
        item->Result = _l("successful");
        item->NewPage = this->CreateOnly;
        item->Site = this->User->GetSite();
        item->Type = HistoryMessage;
        item->Target = User->Username;
        item->IsRevertable = false;
        if (this->Dependency != nullptr && this->Dependency->HI != nullptr)
        {
            this->Dependency->HI->UndoDependency = item;
            item->ReferencedBy = this->Dependency->HI;
        }
        if (MainWindow::HuggleMain != nullptr)
        {
            MainWindow::HuggleMain->_History->Prepend(item);
        }
        // write something to talk page in case it was empty
        if (this->User->TalkPage_GetContents().isEmpty())
            this->User->TalkPage_SetContents(this->Text);
        this->isFinished = true;
        return true;
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


