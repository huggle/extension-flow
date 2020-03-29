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
#include <huggle_core/iextension.hpp>
#include <huggle_core/collectable_smartptr.hpp>
#include <huggle_core/webserverquery.hpp>
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

/*!
 * \brief The flow class, how it works:
 *
                       +---------------------------------+
                       |New message delivery is requested|
                       +------------+--------------------+
                                    |
                                    |
                                    |  Extension hook
                                    |
                                    |
                                    v
           Nope        +------------+-------------+           Yes
         +-------------+Is flow supported on wiki?+---------+
         |             +--------------------------+         |
         |                                                  |
         |                                                  |
         |                                          +-------+-------------------+
         |                                          |Override with MessageFlow  |
         |                                          |delivery class and initiate|
         v                                          |asynchronous query to check|
+--------+------------+                             |if target user has flow    |
|Standard delivery    +<-------------+              +-------+-------------------+
+---------------------+              |     <- Nope          |
                                     +---------------+------+------------------+
+----------------------+                             |Query finished, does user|
|Deliver using flow API+<----------------------------+have flow on talk?       |
+----------------------+                     <- Yes  +-------------------------+
 */
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
        bool RequestCore() { return true; }
        bool RequestNetwork() { return true; }
        bool RequestConfiguration() { return true; }
        void Hook_MainWindowOnLoad(void *window);
        void *Hook_MessageUser(void *User, QString Text, QString Title, QString Summary, bool InsertSection = true,
                               void *Dependency = nullptr, bool NoSuffix = false, bool SectionKeep = false,
                               bool Autoremove = true, QString BaseTimestamp = "", bool CreateOnly = false, bool FreshOnly = false);
    public slots:
    private:
        bool IsSupported(Huggle::WikiSite *site);
        QHash<Huggle::WikiSite*, bool> supported;
};

#endif // FLOW_HPP
