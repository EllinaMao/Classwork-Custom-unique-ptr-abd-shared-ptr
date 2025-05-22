#pragma once
#include <iostream>

template <typename T>
class MyUniquePtr {
private:
    T* ptr_;
    Del deleter_;

public:
	MyUniquePtr();
	MyUniquePtr(T* ptr = nullptr) : ptr_(ptr) {}
	MyUniquePtr(nullptr_t Nptr);
	explicit MyUniquePtr(const MyUniquePtr&) = delete;


};

