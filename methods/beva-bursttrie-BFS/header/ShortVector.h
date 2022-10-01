#include <string>
#include <iostream>
#include <iterator>
#include <stdio.h>

using namespace std;

namespace beva_bursttrie_bfs {

    template<class KeyType>
    class ShortVectorIteratorType;

    template<class KeyType>
    class ShortVector;

    const unsigned int MAX_CAPACITY = 0xFF;
    const unsigned int SIZE = 4;

    template<class KeyType>

    class ShortVector {
    public:
        friend class ShortVectorIteratorType<KeyType>;

        typedef ShortVectorIteratorType<KeyType> iterator;
        typedef ptrdiff_t differenceType;
        typedef unsigned char sizeType;
        typedef KeyType valueType;
        typedef KeyType *pointer;
        typedef KeyType &reference;

        KeyType *data;
        sizeType size;
        sizeType capacity;
        bool isLeaf;
        unsigned char value;

        ShortVector() {
            this->data = NULL;
            this->size = 0;
            this->capacity = 0;
        }

        ~ShortVector() {}

        void dealoc() {
            free(this->data);
            this->data = NULL;
            this->size = 0;
            this->capacity = 0;
        }

        KeyType *getData() { return this->data; }

        sizeType getSize() { return this->size; }

        sizeType getCapacity() { return this->capacity; }

        sizeType getMaxCapacity() { return MAX_CAPACITY; }

        iterator begin() { return iterator(this->data); }

        iterator end() { return iterator(&this->data[this->size]); }

        inline KeyType &operator[](const sizeType pos) {
            return this->data[pos];
        }

        void init() {
            this->data = NULL;
            this->size = 0;
            this->capacity = 0;
        }

        iterator insert(iterator position, const valueType &item) {
            iterator x, y;

            if (this->size == this->capacity) {
                sizeType pos = (position.operator->() - this->data);

                if (this->capacity < MAX_CAPACITY) {
                    if (this->capacity == 0) {
                        this->capacity = 8;
                    } else {
                        this->capacity += SIZE;
                    }
                    this->data = (KeyType *) realloc(this->data, sizeof(KeyType) * this->capacity);
                    if ((this->capacity) && (this->data == NULL)) exit(1);
                    position.setPos(this->data + pos);
                } else {
                    return position;
                }
            }
            x = this->end();

            while (x != position) {
                y = x;
                x--;
                *y = *x;
            }
            *position = item;
            this->size++;

            return position;
        }

        iterator erase(iterator position) {
            iterator x, y;

            if (this->size) {
                x = position;
                while (x != this->end()) {
                    y = x;
                    x++;
                    *y = *x;
                }
                this->size--;
            }
            return position;
        }

        void push_back(const KeyType &item) {
            if (this->size == this->capacity) {
                if (this->capacity < MAX_CAPACITY) {
                    this->capacity += SIZE;
                } else {
                    return;
                }
                this->data = (KeyType *) realloc(this->data, sizeof(KeyType) * this->capacity);
            }
            this->data[this->size] = item;
            this->size++;
        }

        void reserve(const sizeType newSize) {
            if ((newSize > this->capacity) && (newSize < MAX_CAPACITY)) {
                this->capacity = newSize;
                this->data = (KeyType *) realloc(this->data, sizeof(KeyType) * this->capacity);
            }
        }

        void shrink_to_fit() {
            if (this->capacity > this->size) {
                this->capacity = this->size;
                this->data = (KeyType *) realloc(this->data, sizeof(KeyType) * this->capacity);
            }
        }

        KeyType &at(const sizeType position) {
            return this->data[position];
        }

        KeyType &front() {
            return this->data[0];
        }

        bool empty() {
            return this->size == 0;
        }

        void resize(sizeType newSize) {
            if (newSize < this->size) {
                this->size = newSize;
            }
        }

        KeyType &back() {
            return this->data[this->size - 1];
        }

        void clear() {
            if (this->capacity) free(this->data);
            this->size = 0;
            this->capacity = 0;
        }
    };

// Define the iterator
    template<class KeyType>

    class ShortVectorIteratorType {
    public:
        KeyType *position;

        ShortVectorIteratorType(KeyType *l) {
            this->position = l;
        }

        ShortVectorIteratorType() {
            this->position = NULL;
        }

        void setPos(KeyType *addr) {
            this->position = addr;
        }

        bool operator==(const ShortVectorIteratorType<KeyType> &param) const {
            return param.position == this->position;
        }

        bool operator!=(const ShortVectorIteratorType<KeyType> &param) const {
            return (param.position != this->position);
        }

        KeyType &operator*() {
            return *this->position;
        }

        KeyType *operator->() {
            return this->position;
        }

        ShortVectorIteratorType<KeyType> &operator=(const ShortVectorIteratorType<KeyType> &param) {
            this->position = param.position;
            return *this;
        }

        void swap(ShortVectorIteratorType<KeyType> &param) {
            KeyType tmp;
            tmp = *(this->position);
            *(this->position) = *param.position;
            *param.position = tmp;
        }

        ShortVectorIteratorType<KeyType> &operator++(int) { // it++
            this->position++;
            return *this;
        }

        ShortVectorIteratorType<KeyType> &operator--(int) { // it--
            this->position--;
            return *this;
        }

        ShortVectorIteratorType<KeyType> &operator++() { // ++it
            ++this->position;
            return *this;
        }

        ShortVectorIteratorType<KeyType> &operator--() { // --it
            --this->position;
            return *this;
        }

        ShortVectorIteratorType<KeyType> &operator+(size_t i) {
            this->position += i;
            return *this;
        }
    };

}
