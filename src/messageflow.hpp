//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

#ifndef MESSAGE_FLOW_H
#define MESSAGE_FLOW_H

#include <definitions.hpp>
#include <message.hpp>

namespace Huggle
{
    //! This is similar to query, just it's more simple, you can use it to deliver messages to users
    class HUGGLE_EX MessageFlow : public Message
    {
        public:
            //! Creates a new instance of message class that is used to deliver a message to users
            MessageFlow(WikiUser *target, QString MessageText, QString MessageSummary);
            ~MessageFlow();
        protected:
            bool Done();
            void Fail(QString reason);
    };
}

#endif // MESSAGE_H
