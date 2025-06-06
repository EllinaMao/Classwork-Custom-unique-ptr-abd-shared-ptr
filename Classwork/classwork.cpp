﻿#include <iostream>
#include "unique_ptr.h"
#include "shared_ptr.h"
using namespace std;
/*
Строки и умные указатели
Задание
Создайте пользовательские классы умных указателей. Один из классов должен повторять функциональность стандартного класса unique_ptr, а другой shared_ptr. Напишите код по тестированию ваших классов.


*/
//#define UNIQUE
//#define SHARED

int main()
{
    struct Test {
        Test() { cout << "Test created\n"; }
        ~Test() { cout << "Test destroyed\n"; }
        void hello() { cout << "Hello from Test!\n"; }
    };

#ifdef UNIQUE

    {
        MyUniquePtr<Test> ptr1(new Test()); 
        ptr1->hello();                      
        // Проверка reset
		cout << "Resetting the pointer\n";
        ptr1.reset(new Test());
        // Проверка release
		cout << "Releasing ownership of the pointer\n";
        Test* raw = ptr1.release();

        if (ptr1.get()) {
        cout << "ptr1 not empty\n";
        }
        delete raw;
    }
    //array
    {
        MyUniquePtr<Test[]> arrPtr(new Test[3]);
        for (int i = 0; i < 3; ++i) {
            arrPtr[i].hello();
        }
        cout << "Resetting array pointer\n";
        arrPtr.reset(new Test[2]); // Новый массив из 2 элементов
        for (int i = 0; i < 2; ++i) {
            arrPtr[i].hello();
        }

        cout << "Releasing ownership of the array\n";
        Test* rawArr = arrPtr.release();
        if (arrPtr.get()) {
            cout << "arrPtr not empty\n";
        }
        delete[] rawArr;
    }

#endif // UNIQUE
#ifdef SHARED
    {
        SharedPtr<Test> sp1(new Test());
        cout << "sp1 use_count: " << sp1.use_count() << endl;
        {
            SharedPtr<Test> sp2 = sp1;
            cout << "sp1 use_count after copy: " << sp1.use_count() << endl;
            cout << "sp2 use_count: " << sp2.use_count() << endl;
            sp2->hello();
        }
        cout << "sp1 use_count after exit: " << sp1.use_count() << endl;
    }

    {
        SharedPtr<Test[]> arrSp(new Test[3]);
        cout << "arrSp use_count: " << arrSp.use_count() << endl;
        {
            SharedPtr<Test[]> arrSp2 = arrSp;
            cout << "arrSp use_count after copy: " << arrSp.use_count() << endl;
            cout << "arrSp2 use_count: " << arrSp2.use_count() << endl;
            for (int i = 0; i < 3; ++i) {
                arrSp2[i].hello();
            }
        }
        cout << "arrSp use_count: " << arrSp.use_count() << endl;
    }
    



#endif // SHARED
    return 0;
}

