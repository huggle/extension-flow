//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

#include "flow.hpp"
#include "messageflow.hpp"
#include <wikiedit.hpp>
#include <wikiuser.hpp>
#include <wikisite.hpp>
#include <wikipage.hpp>
#include <configuration.hpp>
#include <mainwindow.hpp>
#include <querypool.hpp>
#include <generic.hpp>
#include <syslog.hpp>
#include <QTimer>

using namespace Huggle;

flow::flow()
{
}

flow::~flow()
{
}

bool flow::Register()
{
    this->Init();
    return true;
}

bool flow::IsWorking()
{
    return true;
}

void flow::Hook_MainWindowOnLoad(void *window)
{
    // This is necessary because of some flaws in a way how extensions are loaded
    MainWindow::HuggleMain = (MainWindow*)window;
}

void *flow::Hook_MessageUser(void *User, QString Text, QString Title, QString Summary, bool InsertSection, void *Dependency, bool NoSuffix, bool SectionKeep, bool Autoremove, QString BaseTimestamp, bool CreateOnly, bool FreshOnly)
{
    WikiUser *user = (WikiUser*)User;
    if (!this->IsSupported(user->GetSite()))
        return nullptr;

    if (User == nullptr)
    {
        Huggle::Syslog::HuggleLogs->Log("FLOW: Cowardly refusing to message NULL user");
        return nullptr;
    }

    if (Title.isEmpty())
    {
        InsertSection = false;
        SectionKeep = false;
    }

    MessageFlow *m = new MessageFlow(user, Text, Summary);
    m->Title = Title;
    m->Dependency = (Query*)Dependency;
    m->CreateInNewSection = InsertSection;
    m->BaseTimestamp = BaseTimestamp;
    m->SectionKeep = SectionKeep;
    m->RequireFresh = FreshOnly;
    m->CreateOnly = CreateOnly;
    m->Suffix = !NoSuffix;
    QueryPool::HugglePool->Messages.append(m);
    m->RegisterConsumer(HUGGLECONSUMER_CORE);
    if (!Autoremove)
    {
        m->RegisterConsumer(HUGGLECONSUMER_CORE_MESSAGE);
    }
    m->Send();
    HUGGLE_DEBUG("FLOW: Sending message to user " + user->Username, 1);
    return m;
}

bool flow::IsSupported(WikiSite *site)
{
    if (this->supported.contains(site))
        return this->supported[site];

    bool supports = false;
    foreach (WikiSite_Ext extension, site->Extensions)
    {
        if (extension.Name == "Flow")
        {
            supports = true;
            break;
        }
    }
    this->supported.insert(site, supports);
    return supports;
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2("org.huggle.extension.qt", flow)
#endif

