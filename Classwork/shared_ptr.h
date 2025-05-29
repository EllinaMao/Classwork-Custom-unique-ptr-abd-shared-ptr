#pragma once
#include <iostream>

template <typename T>
class SharedPtr {
	private:
	T* ptr;
	unsigned int* ref_count;

public:
	// пустой конструктор
	explicit SharedPtr(T* p = nullptr) : ptr(p), ref_count(new unsigned int(1)) {}
	// копи конструктор
	SharedPtr(const SharedPtr& other) : ptr(other.ptr), ref_count(other.ref_count) {
		(*ref_count)++;
	}

	// опрератор присваивания
	SharedPtr& operator=(const SharedPtr& other) {
		if (this != &other) {
			if (--(*ref_count) == 0) {
				delete ptr;
				delete ref_count;
			}
			ptr = other.ptr;
			ref_count = other.ref_count;
			(*ref_count)++;
		}
		return *this;
	}
	// деструктор
	~SharedPtr() {
		if (--(*ref_count) == 0) {
			delete ptr;
			delete ref_count;
		}
	}

	T& operator*() { return *ptr; }
	T* operator->() { return ptr; }
	unsigned int use_count() const { return *ref_count; }
};


// Частичная специализация для массивов
template <typename T>
class SharedPtr<T[]> {
private:
	T* ptr;
	unsigned int* ref_count;

public:
	explicit SharedPtr(T* p = nullptr) : ptr(p), ref_count(new unsigned int(1)) {}
	SharedPtr(const SharedPtr& other) : ptr(other.ptr), ref_count(other.ref_count) {
		(*ref_count)++;
	}
	SharedPtr& operator=(const SharedPtr& other) {
		if (this != &other) {
			if (--(*ref_count) == 0) {
				delete[] ptr;
				delete ref_count;
			}
			ptr = other.ptr;
			ref_count = other.ref_count;
			(*ref_count)++;
		}
		return *this;
	}
	~SharedPtr() {
		if (--(*ref_count) == 0) {
			delete[] ptr;
			delete ref_count;
		}
	}
	// Оператор доступа к элементу массива
	T& operator[](size_t index) { return ptr[index]; }
	// Получить сырой указатель
	T* get() const { return ptr; }
	unsigned int use_count() const { return *ref_count; }
};
