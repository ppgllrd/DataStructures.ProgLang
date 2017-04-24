/******************************************************************************
 * Exceptions for linear lists.
 *
 * Pepe Gallardo, 2017
 *
 ******************************************************************************/

#include "ListException.h"

namespace dataStructures {
    namespace list {
        ListException::ListException(const std::string &str) : msg{str} {};

        ListException::~ListException() {};

        std::string ListException::message() const { return (msg); };
    }
}