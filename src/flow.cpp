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

void *flow::Hook_MessageUser(void *User, QString Text, QString Title, QString Summary, bool InsertSection, void *Dependency, bool NoSuffix, bool SectionKeep, bool Autoremove, QString BaseTimestamp, bool CreateOnly, bool FreshOnly)
{
    WikiUser *user = (WikiUser*)User;
    if (!this->IsSupported(user->GetSite()))
        return nullptr;

    return nullptr;
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

