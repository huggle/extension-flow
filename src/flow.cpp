//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

#include "flow.hpp"
#include <wikiedit.hpp>
//#include <wikiuser.hpp>
#include <wikisite.hpp>
#include <wikipage.hpp>
#include <configuration.hpp>
#include <querypool.hpp>
#include <generic.hpp>
#include <syslog.hpp>
#include <QTimer>

#define SCORING_TIMEOUT this->GetConfig("timeout", "7")

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

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2("org.huggle.extension.qt", flow)
#endif

