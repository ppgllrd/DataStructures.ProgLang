/******************************************************************************
 * Singly linked linear lists.
 *
 * Pepe Gallardo / Vicente Benjumea, 2017
 *
 ******************************************************************************/

#ifndef DS_LINKEDLIST_H
#define DS_LINKEDLIST_H

#include <assert.h>
#include <iostream>
#include <string>
#include "ListException.h"

namespace dataStructures {
    namespace list {
        template<typename T>
        class LinkedList {

        private:
            template<typename E>
            class Node;

            template<typename E> using NodeP = Node<E> *;

            // Just like in Java, this class can be non-generic and
            // use parameter <T> from outer class
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
                    // *this = std::move(duplicate(that)); // move semantics
                    *this = duplicate(that); // move semantics by default
                }
            }

            // Copy assignment should:
            // 1) release this’s resources
            // 2) make this be a copy of that
            LinkedList<T> &operator=(const LinkedList<T>& that) {
                if (this != &that) {
                    // Copy
                    LinkedList<T> copy(that);

                    // And swap
                    this->swap(copy);

                    // Destruction of copy will release old this’s resources
                }
                return *this;
            }

            // Move constructor should:
            // 1) steal that’s resources
            // 2) reset that
            LinkedList(LinkedList<T>&& that) noexcept
                // Reset this
                : LinkedList() {

                // And swap to steal that’s resources
                this->swap(that);
            }

            // Move assignment should:
            // 1) release this’s resources
            // 2) make this to steal that's resources
            // 3) reset that
            LinkedList<T> &operator=(LinkedList<T>&& that) {
                if (this != &that) {
                    LinkedList<T> tmp;

                    // Swap this and tmp
                    this->swap(tmp);

                    // this is now a reset structure
                    // Destruction of tmp will release old this’s resources

                    // And swap this and that to steal that’s resources
                    this->swap(that);
                }
                return *this;
            }

            // Destructor
            ~LinkedList() {
                while (first != nullptr) {
                    NodeP<T> toDelete = first;
                    first = first->next;
                    delete toDelete;
                }
            }

            // Initializer list
            LinkedList(const std::initializer_list<T> &xs) : LinkedList() {
                if(xs.size() > 0) {
                    // *this = std::move(duplicate(xs)); // move semantics
                    *this = duplicate(xs); // move semantics by default
                }
            }

        private:
            template <typename Iterable>
            static LinkedList<T> duplicate(const Iterable& src) {
                LinkedList<T> copy;
                auto it = src.begin();
                if (it != src.end()) {
                    // copy first node
                    copy.first = new Node<T>(*it, nullptr);
                    copy.last = copy.first;
                    ++copy.sz;
                    ++it;

                    // copy rest of nodes
                    while (it != src.end()) {
                        copy.last->next = new Node<T>(*it, nullptr);
                        copy.last = copy.last->next;
                        ++copy.sz;
                        ++it;
                    }
                }

                // if new above fails due to bad_alloc,
                // destructor for copy will release allocated nodes

                // this makes use of move semantics
                return copy;
            }

            void validateIndex(const int i) const {
                if (i < 0 || i >= size())
                    throw ListException("Invalid position "+std::to_string(i));
            }

            // Pre: i is a valid index
            NodeP<T> atIndex(const int i) const {
                NodeP<T> aux = first;
                for (int pos = 0; pos < i; ++pos)
                    aux = aux->next;

                return aux;
            }

        public:
            int size() const {
                return sz;
            }

            bool isEmpty() const {
                return sz == 0;
            }

            // this operator can be used to read an element in the list
            const T& operator[](const int i) const {
                validateIndex(i);
                return atIndex(i)->elem;
            }

            // this one can be used to modify an element:
            // list[i] = x;
            T& operator[](const int i) {
                validateIndex(i);
                return atIndex(i)->elem;
            }

            // Next 3 methods are not really needed in C++ as operator []
            // can be used for reading/writing an element in the list:
            const T& get(const int i) const {
                validateIndex(i);
                return atIndex(i)->elem;
            }

            T& get(const int i) {
                validateIndex(i);
                return atIndex(i)->elem;
            }

            void set(const int i, const T &x) {
                validateIndex(i);
                atIndex(i)->elem = x;
            }

            void insert(const int i, const T &x) {
                if (i == 0) { // insertion at head
                    first = new Node<T>(x, first);
                    if (i == sz) // insertion at head, and list was empty
                        last = first;
                } else if (i == sz) { // insertion after last element
                    last->next = new Node<T>(x, nullptr);;
                    last = last->next;
                } else { // internal insertion
                    validateIndex(i);
                    NodeP<T> prev = atIndex(i - 1);
                    prev->next = new Node<T>(x, prev->next);
                }
                ++sz;
            }

            void remove(const int i) {
                validateIndex(i);
                if (i == 0) { // removing first element
                    NodeP<T> toDelete = first; // difference wrt Java: node must be freed explicitly
                    first = first->next;
                    delete(toDelete);

                    if (first == nullptr) // was also last element?
                        last = nullptr;
                } else {
                    NodeP<T> prev = atIndex(i - 1);

                    NodeP<T> toDelete = prev->next; // difference wrt Java: node must be freed explicitly
                    prev->next = prev->next->next;
                    delete(toDelete);

                    if (i == (sz - 1)) // was last element?
                        last = prev;
                }
                --sz;
            }

            void append(const T &x) {
                NodeP<T> node = new Node<T>(x, nullptr);
                if (sz == 0) // was list empty?
                    first = node;
                else
                    last->next = node;
                last = node;
                ++sz;
            }

            void prepend(const T &x) {
                first = new Node<T>(x, first);
                if (sz == 0) // was list empty?
                    last = first;
                ++sz;
            }

            void swap(LinkedList<T>& that) {
                std::swap(first, that.first);
                std::swap(last, that.last);
                std::swap(sz, that.sz);
            }

            // Just like in Java, this class can be non-generic and
            // use parameter <T> from outer class
            template<typename E>
            class Iterator {
            private:
                friend class LinkedList<E>;

                NodeP<E> current;

                // only class LinkedList<E> can create this kind of iterators
                Iterator(NodeP<E> node) : current(node) {}

            public:

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

            Iterator<T> begin() const {
                return Iterator<T>(first);
            }

            Iterator<T> end() const {
                return Iterator<T>(nullptr);
            }

            friend std::ostream &operator<<(std::ostream &out, const LinkedList<T> &xs) {
                out << "LinkedList(";
                if (xs.sz > 0) {
                    NodeP<T> aux = xs.first;
                    out << aux->elem;
                    for (int i = 1; i < xs.sz; ++i) {
                        aux = aux->next;
                        out << "," << aux->elem;
                    }
                }
                out << ")";
                return out;
            }
        };
    }
}

#endif //DS_LINKEDLIST_H