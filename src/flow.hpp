//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

#ifndef FLOW_HPP
#define FLOW_HPP

#define HUGGLE_EXTENSION
#include <iextension.hpp>
#include <collectable_smartptr.hpp>
#include <webserverquery.hpp>
#include <QAction>
#include <QHash>

class QTimer;

namespace Huggle
{
    class WikiPage;
    class WikiEdit;
    class WikiSite;
    class Query;
}

class flow : public QObject, public Huggle::iExtension
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
        Q_PLUGIN_METADATA(IID "org.huggle.extension.qt" FILE "flow_helper.json")
#endif
    Q_INTERFACES(Huggle::iExtension)
    public:
        flow();
        ~flow();
        bool Register();
        bool IsWorking();
        QString GetExtensionName() { return "Flow"; }
        QString GetExtensionAuthor() { return "Petr Bena"; }
        QString GetExtensionDescription() { return "Support for Flow talk page model"; }
        QString GetExtensionVersion() { return "1.0.0"; }
    public slots:
    private:
};

#endif // FLOW_HPP
