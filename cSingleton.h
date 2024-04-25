#pragma once
#include <iostream>

template <typename T>
class cSingleton
{
protected:
    // Cto/dTor
    cSingleton() { }
    ~cSingleton() { std::cout << "destroying singleton.\n"; }

public:
    // Public interface
    static T* getInstance()
    {
        if (NULL == _singleton)
        {
            std::cout << "creating singleton.\n";
            _singleton = new T;
        }
        else
        {
            //std::cout << "singleton already created!\n";
        }

        return (static_cast<T*> (_singleton));
    }

    static bool kill()
    {
        if (nullptr != _singleton)
        {
            delete _singleton;
            _singleton = nullptr;
            return true;
        }
        return false;
    }

private:
    // Unique instance
    static T* _singleton;
};

template <typename T>
T* cSingleton<T>::_singleton = nullptr;


