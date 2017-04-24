/**
 * @author Pepe Gallardo, Data Structures, Grado en Informática. UMA.
 *
 * List implementation using linked lists
 */package dataStructures.list;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * List implemented using a double ended linked list
 * (with references to first and last elements).
 * @param <T> Type of elements in list.
 */
public class LinkedList<T> implements List<T> {

    private static class Node<E> {
        E elem;
        Node<E> next;

        Node(E x, Node<E> node) {
            elem = x;
            next = node;
        }
    }

    private Node<T> first, last;
    private int sz;

    /**
     * Constructor. Creates an empty list.
     * <p>Time complexity: O(1)
     */
    public LinkedList() {
        first = null;
        last = null;
        sz = 0;
    }

    /**
     * Copy constructor. Creates a new list as a deep copy of that.
     * <p>Time complexity: O(n)
     */
    public LinkedList(LinkedList<T> that) {
        this();

        if(that.sz > 0) {
            // copy first node
            Node<T> firstP = new Node<>(that.first.elem, null);
            Node<T> lastP = firstP;

            // copy rest of nodes
            for(Node<T> aux = that.first.next; aux != null; aux = aux.next) {
                lastP.next = new Node<>(aux.elem, null);
                lastP = lastP.next;
            }

            // safely set new state
            first = firstP;
            last = lastP;
            sz = that.sz;
        }
    }

    private void validateIndex(int i) {
        if (i < 0 || i >= size())
            throw new ListException("Invalid position " + i);
    }

    private Node<T> atIndex(int i) {
        Node<T> aux = first;
        for (int pos = 0; pos < i; pos++)
            aux = aux.next;

        return aux;
    }

    /**
     * {@inheritDoc}
     * <p>Time complexity: O(1)
     */
    public int size() {
        return sz;
    }

    /**
     * {@inheritDoc}
     * <p>Time complexity: O(1)
     */
    public boolean isEmpty() {
        return sz == 0;
    }

    /**
     * {@inheritDoc}
     * <p>Time complexity: O(n)
     * @throws ListException {@inheritDoc} 
     */
    public T get(int i) {
        validateIndex(i);
        return atIndex(i).elem;
    }

    /** 
     * {@inheritDoc}
     * <p>Time complexity: O(n)
     * @throws ListException {@inheritDoc} 
     */
    public void set(int i, T x) {
        validateIndex(i);
        atIndex(i).elem = x;
    }

    /** 
     * {@inheritDoc}
     * <p>Time complexity: O(n)
     * @throws ListException {@inheritDoc} 
     */
    public void insert(int i, T x) {
        if (i == sz) { // insertion after last element
            Node<T> node = new Node<>(x, null);
            if (sz == 0) // was list empty?
                first = node;
            else
                last.next = node;
            last = node;
        } else if (i == 0) { // insertion at head, and list was not empty
            first = new Node<>(x, first);
        } else { // internal insertion
            validateIndex(i);
            Node<T> prev = atIndex(i - 1);
            prev.next = new Node<>(x, prev.next);
        }
        sz++;
    }

    /**
     * {@inheritDoc}
     * <p>Time complexity: O(n)
     * @throws ListException {@inheritDoc}
     */
    public void remove(int i) {
        validateIndex(i);
        if (i == 0) { // removing first element
            first = first.next;

            if (first == null) // was also last element?
                last = null;

        } else {
            Node<T> prev = atIndex(i - 1);

            prev.next = prev.next.next;

            if (i == (sz - 1)) // was last element?
                last = prev;
        }
        sz--;
    }

    /**
     * {@inheritDoc}
     * <p>Time complexity: O(1)
     */
    public void append(T x) {
        Node<T> node = new Node<>(x, null);
        if (sz == 0) // was list empty?
            first = node;
        else
            last.next = node;
        last = node;
        sz++;
    }
    
    /** 
     * {@inheritDoc}
     * <p>Time complexity: O(1)
     */
    public void prepend(T x) {
        first = new Node<>(x, first);
        if(sz == 0)
            last = first;
        sz++;
    }

    /**
     * Iterator over elements in list.
     * Note that {@code remove} method is not supported. Note also 
     * that list structure should not be modified during iteration as 
     * iterator state may become inconsistent.
     * @see java.lang.Iterable#iterator()
     */
    public Iterator<T> iterator() {
        return new LinkedListIterator();
    }

    private class LinkedListIterator implements Iterator<T> {
        Node<T> current;

        public LinkedListIterator(){
            current = first;
        }
        
        public boolean hasNext() {
            return current != null;
        }

        public T next() {
            if (!hasNext())
                throw new NoSuchElementException();
            
            T x = current.elem;
            current = current.next;
            return x;
        }
    }

    /**
     * Returns representation of list as a String.
     */
    @Override
    public String toString() {
        String className = getClass().getSimpleName();
        StringBuilder sb = new StringBuilder(className+"(");
        Node<T> aux = first;
        for (int i = 0; i < sz - 1; i++) {
            sb.append(aux.elem).append(",");
            aux = aux.next;
        }
        if(sz > 0)
            sb.append(aux.elem);
        sb.append(")");
        return sb.toString();
    }
}
