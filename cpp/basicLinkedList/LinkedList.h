/******************************************************************************
 * Singly linked linear lists.
 *
 * Pepe Gallardo, 2017
 *
 ******************************************************************************/

#ifndef DS_LINKEDLIST_H
#define DS_LINKEDLIST_H

#include <assert.h>
#include <iostream>
#include "ListException.h"

namespace dataStructures {
    namespace list {
        template<typename T>
        class LinkedList {

        private:
            template<typename E>
            class Node;

            template<typename E> using NodeP = Node<E> *;

            template<typename E>
            class Node {
            public:
                E elem;
                NodeP<E> next;

                Node(const E &x, const NodeP<E> &node) : elem{x}, next{node} {}
            };

            NodeP<T> first, last;
            int sz;

        public:
            // Default constructor
            LinkedList() : first(nullptr), last(nullptr), sz(0) {}

            // Copy constructor
            LinkedList(const LinkedList<T> &that) : LinkedList() {
                if(that.sz > 0) {
                    // copy first node
                    NodeP<T> firstP = new Node<T>(that.first->elem, nullptr);
                    NodeP<T> lastP = firstP;

                    // copy rest of nodes
                    for(NodeP<T> aux = that.first->next; aux != nullptr; aux = aux->next) {
                        lastP->next = new Node<T>(aux->elem, nullptr);
                        lastP = lastP->next;
                    }

                    // safely set new state
                    first = firstP;
                    last = lastP;
                    sz = that.sz;
                }
            }

            // Copy assignment should:
            // 1) release this’s resources
            // 2) make this be a copy of that
            LinkedList<T> &operator=(const LinkedList<T>& that) {
                // Copy
                LinkedList<T> copy(that);

                // And swap
                std::swap(first, copy.first);
                std::swap(last, copy.last);
                std::swap(sz, copy.sz);

                // Destruction of copy will release old this’s resources
                return *this;
            }

            // Move constructor should:
            // 1) steal that’s resources
            // 2) reset that
            LinkedList(LinkedList<T>&& that) noexcept
            // Reset this
                    : LinkedList() {

                // And swap to steal that’s resources
                std::swap(first, that.first);
                std::swap(last, that.last);
                std::swap(sz, that.sz);
            }

            // Move assignment should:
            // 1) release this’s resources
            // 2) make this to steal that's resources
            // 3) reset that
            LinkedList<T> &operator=(LinkedList<T>&& that) {
                if (this != &that) {
                    LinkedList<T> tmp;

                    // Swap this and tmp
                    std::swap(first, tmp.first);
                    std::swap(last, tmp.last);
                    std::swap(sz, tmp.sz);

                    // this is now a reset structure
                    // Destruction of tmp will release old this’s resources

                    // And swap this and that to steal that’s resources
                    std::swap(first, that.first);
                    std::swap(last, that.last);
                    std::swap(sz, that.sz);
                }
                return *this;
            }

            // Destructor
            ~LinkedList() {
                NodeP<T> aux = first;
                while (aux != nullptr) {
                    NodeP<T> toDelete = aux;
                    aux = aux->next;
                    delete toDelete;
                }
            }

            // Initializer list
            LinkedList(const std::initializer_list<T> &xs) noexcept : sz(xs.size()) {
                if(sz == 0) {
                    first = nullptr;
                    last = nullptr;
                } else {
                    // copy first node
                    auto it = xs.begin();
                    first = new Node<T>(*it, nullptr);
                    it++;
                    last = first;

                    // copy rest of nodes
                    while(it != xs.end()) {
                        last->next = new Node<T>(*it, nullptr);
                        it++;
                        last = last->next;
                    }
                }
            }

        private:
            void validateIndex(const int i) const {
                if (i < 0 || i >= size())
                    throw ListException("Invalid position " + i);
            }

            // Pre: i is a valid index
            NodeP<T> atIndex(const int i) const {
                NodeP<T> aux = first;
                for (int pos = 0; pos < i; pos++)
                    aux = aux->next;

                return aux;
            }

        public:
            int size() const {
                return sz;
            }

            T &operator[](const int i) const {
                return atIndex(i)->elem;
            }

            T get(const int i) const {
                return atIndex(i)->elem;
            }

            void set(const int i, const T &x) {
                validateIndex(i);
                atIndex(i)->elem = x;
            }

            void insert(const int i, const T &x) {
                if (i == sz) { // insertion after last element
                    NodeP<T> node = new Node<T>(x, nullptr);
                    if (sz == 0) // was list empty?
                        first = node;
                    else
                        last->next = node;
                    last = node;
                } else if (i == 0) { // insertion at head, and list was not empty
                    first = new Node<T>(x, first);
                } else { // internal insertion
                    validateIndex(i);
                    NodeP<T> prev = atIndex(i - 1);
                    prev->next = new Node<T>(x, prev->next);
                }
                sz++;
            }

            void remove(const int i) {
                validateIndex(i);
                if (i == 0) { // removing first element
                    NodeP<T> toDelete = first; // difference wrt Java: node must be freed explicitly
                    delete(toDelete);

                    first = first->next;

                    if (first == nullptr) // was also last element?
                        last = nullptr;

                } else {
                    NodeP<T> prev = atIndex(i - 1);

                    NodeP<T> toDelete = prev->next; // difference wrt Java: node must be freed explicitly
                    delete(toDelete);

                    prev->next = prev->next->next;

                    if (i == (sz - 1)) // was last element?
                        last = prev;
                }
                sz--;
            }

            void append(const T &x) {
                NodeP<T> node = new Node<T>(x, nullptr);
                if (sz == 0) // was list empty?
                    first = node;
                else
                    last->next = node;
                last = node;
                sz++;
            }

            void prepend(const T &x) {
                first = new Node<T>(x, first);
                if (sz == 0) // was list empty?
                    last = first;
                sz++;
            }

            template<typename E>
            class Iterator {
            private:
                NodeP<E> current;
            public:
                Iterator(NodeP<E> node) : current(node) {}

                Iterator &operator++() {
                    current = current->next;
                    return *this;
                }

                Iterator operator++(int) {
                    Iterator it(*this);
                    current = current->next;
                    return it;
                }

                E &operator * () const {
                    assert(current != nullptr && "Invalid iterator dereference!");
                    return current->elem;
                }

                bool operator==(const Iterator &it) const { return current == it.current; }

                bool operator!=(const Iterator &it) const { return current != it.current; }

            };

            Iterator<T> begin() {
                return Iterator<T>(first);
            }

            Iterator<T> end() {
                return Iterator<T>(nullptr);
            }

            friend std::ostream &operator<<(std::ostream &out, const LinkedList<T> &xs) {
                out << "LinkedList(";
                NodeP<T> aux = xs.first;
                for (int i = 0; i < xs.sz - 1; i++) {
                    out << aux->elem << ",";
                    aux = aux->next;
                }
                if (xs.sz > 0)
                    out << aux->elem;
                out << ")";
            }
        };
    }
}

#endif //DS_LINKEDLIST_H
