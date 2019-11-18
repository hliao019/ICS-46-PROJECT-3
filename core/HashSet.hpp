// HashSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"

using namespace std;



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet() noexcept;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    struct ListNode
    {
        ElementType key;
        ListNode* next = nullptr;
        static void deleteList(ListNode * L)
        {
            if (L != nullptr)
                deleteList(L->next);
            delete L;
        }
    };
    HashFunction hashFunction;
    ListNode** hash;
    unsigned int hash_capacity;
    unsigned int hash_size;
    unsigned int elementNumber;
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}



template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
        : hashFunction{hashFunction}
{

    hash_capacity = DEFAULT_CAPACITY;
    hash = new ListNode*[DEFAULT_CAPACITY];
    for(int i =0; i < hash_capacity; i++)
    {
        hash[i] = new ListNode;
    }
    hash_size = 0;
    elementNumber=0;
}



template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{

    for (int i = 0; i < hash_capacity; ++i) {
        ListNode::deleteList(hash[i]);
    }
    delete []hash;


}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
        : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    hash_capacity = s.hash_capacity;
    hash_size = s.hash_size;
    hash = new ListNode*[hash_capacity];
    for(int i =0; i < hash_capacity; i++)
    {
        hash[i] = new ListNode;
    }


    for (int i = 0; i <hash_capacity ; ++i) {
        ListNode* tmp = hash[i];
        ListNode* copyTmp = s.hash[i];
        while(copyTmp->next!= nullptr)
        {
            tmp->next = new ListNode;
            tmp->next->key = copyTmp->next->key;
            copyTmp = copyTmp->next;
            tmp = tmp->next;

        }

    }

}

template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
        : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    hash_capacity = s.hash_capacity;
    hash_size = s.hash_size;
    hash = new ListNode*[hash_capacity];
    for(int i =0; i < hash_capacity; i++)
    {
        hash[i] = new ListNode;
    }


    for (int i = 0; i <hash_capacity ; ++i) {
        ListNode* tmp = hash[i];
        ListNode* copyTmp = s.hash[i];
        while(copyTmp->next!= nullptr)
        {
            tmp->next = new ListNode;
            tmp->next->key = copyTmp->next->key;
            copyTmp = copyTmp->next;
            tmp = tmp->next;

        }

    }

}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    for(int i = 0; i < hash_capacity; i++) {
        ListNode::deleteList(hash[i]);
    }

    delete[] hash;

    hash_size = s.size();
    hash_capacity = s.hash_capacity;

    hash = new ListNode*[hash_capacity]();

    for(int i = 0; i < hash_capacity; i++) {
        hash[i] = new ListNode;
    }

    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    for (int i = 0; i < s.hash_capacity; ++i) {
        while(s.hash[i]->next!= nullptr)
        {
            this->add(s.hash[i]->next->key);
        }
    }
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}

template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    int elementIndex;
    if ((hash_size / hash_capacity) > 0.8) {
        int prevCapacity = hash_capacity;
        hash_capacity = prevCapacity * 2;

        ListNode **oldHash = hash;
        hash = new ListNode *[hash_capacity];
        elementNumber = 0;
        for (int i = 0; i < hash_capacity; i++) {
            hash[i] = new ListNode;
        }

        for (int j = 0; j < prevCapacity; ++j) {
            ListNode *source = oldHash[j];
            while (source->next != nullptr)
            {
                ElementType copyElement = source->next->key;
                elementIndex = hashFunction(copyElement) % hash_capacity;
                if (hash[elementIndex]->next == nullptr) {
                    hash[elementIndex]->next = new ListNode;
                    hash[elementIndex]->next->key = copyElement;
                    elementNumber = elementNumber + 1;
                    hash_size = hash_size + 1;
                } else {
                    if (!this->isElementAtIndex(copyElement, elementIndex)) {
                        ListNode *tmp = hash[elementIndex];
                        while (tmp->next != nullptr) {
                            tmp = tmp->next;
                        }
                        tmp->next = new ListNode;
                        tmp->next->key = copyElement;
                        elementNumber = elementNumber + 1;

                    }
                }
                source = source->next;
            }

        }
    }

    elementIndex = hashFunction(element) % hash_capacity;
    if (hash[elementIndex]->next == nullptr)
    {
        hash[elementIndex]->next = new ListNode;
        hash[elementIndex]->next->key = element;
        elementNumber = elementNumber + 1;
        hash_size = hash_size + 1;
    }
    else
    {
        if (!this->isElementAtIndex(element, elementIndex))
        {
            ListNode *tmp = hash[elementIndex];
            while (tmp->next != nullptr)
            {
                tmp = tmp->next;
            }
            tmp->next = new ListNode;
            tmp->next->key = element;
            elementNumber = elementNumber + 1;

        }
    }


}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    unsigned int index = hashFunction(element) % hash_capacity;
    ListNode* test = hash[index];
    while(test->next != nullptr)
    {
        if(test->next->key == element)
            return true;
        test = test->next;
    }
    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return elementNumber;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{

    int counter = 0;
    ListNode* head = hash[index];
    while(head->next!= nullptr)
    {
        counter =counter+1;
        head = head->next;
    }
    return counter;


}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    ListNode* head = hash[index];
    while(head->next!= nullptr)
    {
        if (element == head->next->key)
            return true;
        head = head->next;
    }
    return false;
}



#endif // HASHSET_HPP

