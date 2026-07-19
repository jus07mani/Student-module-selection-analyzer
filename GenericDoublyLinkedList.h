#pragma once

namespace P5
{
    // Generic container class, implemented as a doubly linked list without
    // dummy elements. Requires T to support operator== (used by Contains /
    // Remove) and operator< (used by Sort). All four data classes (Module,
    // Instructor, SelectedModule, Statistic) provide both; std::string
    // provides both natively.
    //
    // Non-copyable (to avoid accidental double frees of the raw node
    // pointers) but movable, so it can still be returned by value from
    // IOUtils/TaskUtils functions and reassigned, e.g.
    //     Modules = IOUtils::ReadModules(path);
    template <typename T>
    class GenericDoublyLinkedList
    {
        struct Node
        {
            T Data;
            Node* Previous;
            Node* Next;
            Node(const T& data, Node* previous, Node* next)
                : Data(data), Previous(previous), Next(next)
            {
            }
        };

        Node* head = nullptr; // start address
        Node* tail = nullptr; // end address

        void Clear()
        {
            Node* p = head;
            while (p != nullptr)
            {
                Node* next = p->Next;
                delete p;
                p = next;
            }
            head = tail = nullptr;
        }

    public:
        GenericDoublyLinkedList() = default;
        ~GenericDoublyLinkedList() { Clear(); }

        GenericDoublyLinkedList(const GenericDoublyLinkedList&) = delete;
        GenericDoublyLinkedList& operator=(const GenericDoublyLinkedList&) = delete;

        GenericDoublyLinkedList(GenericDoublyLinkedList&& other) noexcept
            : head(other.head), tail(other.tail)
        {
            other.head = other.tail = nullptr;
        }

        GenericDoublyLinkedList& operator=(GenericDoublyLinkedList&& other) noexcept
        {
            if (this != &other)
            {
                Clear();
                head = other.head;
                tail = other.tail;
                other.head = other.tail = nullptr;
            }
            return *this;
        }

        // Number of elements in the list (0 if empty).
        int Count() const
        {
            int k = 0;
            for (Node* p = head; p != nullptr; p = p->Next)
                k++;
            return k;
        }

        // Adds a new element to the front of the list.
        void AddToFront(const T& element)
        {
            Node* p = new Node(element, nullptr, head);
            if (head != nullptr)
                head->Previous = p;
            else
                tail = p;
            head = p;
        }

        // Adds a new element to the end of the list.
        void AddToEnd(const T& element)
        {
            Node* p = new Node(element, tail, nullptr);
            if (head != nullptr)
                tail->Next = p;
            else
                head = p;
            tail = p;
        }

        // True if an equal element (operator==) is already present.
        bool Contains(const T& element) const
        {
            bool t = false;
            for (Node* p = head; p != nullptr; p = p->Next)
                if (p->Data == element)
                    t = true;
            return t;
        }

        // Selection sort (adapts operator<, translated from the CompareTo-based version).
        void Sort()
        {
            for (Node* s1 = head; s1 != nullptr; s1 = s1->Next)
            {
                Node* minv = s1;
                for (Node* s2 = s1; s2 != nullptr; s2 = s2->Next)
                    if (s2->Data < minv->Data)
                        minv = s2;
                T tmp = s1->Data;
                s1->Data = minv->Data;
                minv->Data = tmp;
            }
        }

        // Removes every element equal (operator==) to the given element.
        void Remove(const T& element)
        {
            Node* s = head;
            while (s != nullptr)
            {
                Node* next = s->Next;
                if (s->Data == element)
                {
                    if (s == head)
                        head = head->Next;
                    if (s == tail)
                        tail = tail->Previous;
                    if (s->Previous != nullptr)
                        s->Previous->Next = s->Next;
                    if (s->Next != nullptr)
                        s->Next->Previous = s->Previous;
                    delete s;
                }
                s = next;
            }
        }

        // ---- Forward iteration (head -> tail), for range-based for loops ----
        class Iterator
        {
            Node* current;
        public:
            explicit Iterator(Node* n) : current(n) {}
            T& operator*() const { return current->Data; }
            Iterator& operator++() { current = current->Next; return *this; }
            bool operator!=(const Iterator& other) const { return current != other.current; }
        };

        Iterator begin() { return Iterator(head); }
        Iterator end() { return Iterator(nullptr); }

        // ---- Reverse iteration (tail -> head), used e.g. by Task3 ----
        class ReverseIterator
        {
            Node* current;
        public:
            explicit ReverseIterator(Node* n) : current(n) {}
            T& operator*() const { return current->Data; }
            ReverseIterator& operator++() { current = current->Previous; return *this; }
            bool operator!=(const ReverseIterator& other) const { return current != other.current; }
        };

        struct ReverseRange
        {
            Node* tailNode;
            ReverseIterator begin() const { return ReverseIterator(tailNode); }
            ReverseIterator end() const { return ReverseIterator(nullptr); }
        };

        ReverseRange Reverse() const { return ReverseRange{ tail }; }
    };
}
