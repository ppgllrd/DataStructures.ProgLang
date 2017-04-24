/******************************************************************************
 * Exceptions for linear lists.
 *
 * Pepe Gallardo, 2017
 *
 ******************************************************************************/

#ifndef DS_LISTEXCEPTION_H
#define DS_LISTEXCEPTION_H

#include<string>

namespace dataStructures {
    namespace list {
        class ListException {
        private:
            std::string msg;
        public:
            ListException(const std::string &str);

            ~ListException();

            std::string message() const;
        };
    }
}

#endif //DS_LISTEXCEPTION_H
