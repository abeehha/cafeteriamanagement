#ifndef MYVECTOR_H
#define MYVECTOR_H

template <typename T>
class MyVector {
    T* data;
    int capacity;
    int currentSize;

    void resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (int i = 0; i < currentSize; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    MyVector() : capacity(4), currentSize(0) {
        data = new T[capacity];
    }

    MyVector(const MyVector& other) : capacity(other.capacity), currentSize(other.currentSize) {
        data = new T[capacity];
        for (int i = 0; i < currentSize; ++i) {
            data[i] = other.data[i];
        }
    }

    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] data;
            capacity = other.capacity;
            currentSize = other.currentSize;
            data = new T[capacity];
            for (int i = 0; i < currentSize; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~MyVector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (currentSize >= capacity) resize();
        data[currentSize++] = value;
    }

    void pop_back() {
        if (currentSize > 0) --currentSize;
    }

    int size() const {
        return currentSize;
    }

    int getCapacity() const {
        return capacity;
    }

    bool empty() const {
        return currentSize == 0;
    }

    T& operator[](int index) {
        if (index < 0 || index >= currentSize) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];

    }

    const T& operator[](int index) const {
        static T dummy = T();
        if (index < 0 || index >= currentSize) return dummy;
        return data[index];
    }

    void clear() {
        currentSize = 0;
    }

    void erase_at(int index) {
        if (index < 0 || index >= currentSize) return;

        for (int i = index; i < currentSize - 1; ++i) {
            data[i] = data[i + 1];
        }

        --currentSize;
    }

    T& back() {
        return data[currentSize - 1];
    }

    const T& back() const {
        return data[currentSize - 1];
    }
};

#endif