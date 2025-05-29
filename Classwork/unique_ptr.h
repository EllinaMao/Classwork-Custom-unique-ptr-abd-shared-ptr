#pragma once
#include <iostream>
#include <memory>

// Умный указатель с уникальным владением (аналог std::unique_ptr)
// T - тип управляемого объекта
// Del - тип делитера (по умолчанию std::default_delete<T>)
// Делитер - это объект, который отвечает за удаление управляемого объекта
// при уничтожении умного указателя. Он может быть пользовательским или стандартным.


template <typename T, typename Del = std::default_delete<T>>
class MyUniquePtr {
private:
    T* ptr_ = nullptr;      // Указатель на управляемый объект
    Del deleter_;           // Делитер для удаления объекта

public:
    // Конструктор по умолчанию. ptr_ = nullptr, делитер по умолчанию
    MyUniquePtr();

    // Конструктор от сырого указателя. Делитер по умолчанию
    explicit MyUniquePtr(T* ptr = nullptr) : ptr_(ptr) {}

    // Конструктор от указателя и пользовательского делитера
    MyUniquePtr(T* ptr, Del deleter) : ptr_(ptr), deleter_(std::move(deleter)) {}

    // Конструктор от nullptr. Делитер по умолчанию
    explicit MyUniquePtr(std::nullptr_t) : ptr_(nullptr), deleter_(Del()) {}

    // Копирующий конструктор удалён (уникальное владение)
    MyUniquePtr(const MyUniquePtr& Right) = delete;

    // Копирующее присваивание удалено (уникальное владение)
    MyUniquePtr& operator=(const MyUniquePtr& Right) = delete;

    // Перемещающий конструктор. Передаёт владение от другого указателя
    MyUniquePtr(MyUniquePtr&& other) noexcept;

    // Перемещающее присваивание. Освобождает текущий ресурс и принимает владение
    MyUniquePtr& operator=(MyUniquePtr&& other) noexcept {
        if (this != &other) {
            if (ptr_) {
                deleter_(ptr_);
            }
            ptr_ = other.ptr_;
            deleter_ = std::move(other.deleter_);
            other.ptr_ = nullptr;
        }
        return *this;
    }

    // Получить сырой указатель
    T* get() const { return ptr_; }

    // Отказаться от владения и вернуть указатель
    T* release() {
        T* temp = ptr_;
        ptr_ = nullptr;
        return temp;
    }

    // Заменить управляемый объект, удалить старый если нужно
    void reset(T* ptr = nullptr);

    // Оператор разименования
    T& operator*() const { return *ptr_; }

    // Оператор доступа к члену
    T* operator->() const { return ptr_; }

    // Деструктор. Удаляет объект через делитер
    ~MyUniquePtr();
};

// Реализация конструктора по умолчанию
template<typename T, typename Del>
inline MyUniquePtr<T, Del>::MyUniquePtr() : deleter_(Del()) {}

// Реализация перемещающего конструктора
template<typename T, typename Del>
inline MyUniquePtr<T, Del>::MyUniquePtr(MyUniquePtr&& other) noexcept
    : ptr_(other.ptr_), deleter_(std::move(other.deleter_)) {
    other.ptr_ = nullptr;
}

// Реализация метода reset
template<typename T, typename Del>
inline void MyUniquePtr<T, Del>::reset(T* ptr)
{
    if (ptr_ && ptr_ != ptr) {
        deleter_(ptr_);
        ptr_ = ptr;
    }
}

// Реализация деструктора
template<typename T, typename Del>
inline MyUniquePtr<T, Del>::~MyUniquePtr()
{
    if (ptr_) {
        deleter_(ptr_);
    }
}


// Частичная специализация для массивов
template <class T, class D>
class MyUniquePtr<T[], D> {
private:
    T* ptr_ = nullptr;      // Указатель на управляемый массив
    D deleter_;             // Делитер для удаления массива
public:
    // Конструктор по умолчанию
    MyUniquePtr() : ptr_(nullptr), deleter_(D()) {}

    // Конструктор от указателя
    explicit MyUniquePtr(T* ptr) : ptr_(ptr), deleter_(D()) {}

    // Конструктор от указателя и пользовательского делитера
    MyUniquePtr(T* ptr, D deleter) : ptr_(ptr), deleter_(std::move(deleter)) {}

    // Конструктор от nullptr
    explicit MyUniquePtr(std::nullptr_t) : ptr_(nullptr), deleter_(D()) {}

    // Копирующий конструктор удалён
    MyUniquePtr(const MyUniquePtr& Right) = delete;

    // Копирующее присваивание удалено
    MyUniquePtr& operator=(const MyUniquePtr& Right) = delete;

    // Перемещающий конструктор
    MyUniquePtr(MyUniquePtr&& other) noexcept;

    // Перемещающее присваивание
    MyUniquePtr& operator=(MyUniquePtr&& other) noexcept;

    // Получить указатель
    T* get() const { return ptr_; }

    // Отказаться от владения
    T* release();

    // Заменить управляемый массив
    void reset(T* ptr = nullptr);

    // Оператор доступа к элементу массива
    T& operator[](size_t index) const { return ptr_[index]; }

    // Деструктор
    ~MyUniquePtr();
};