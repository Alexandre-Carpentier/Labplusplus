#pragma once
#include <iostream>

template <typename T>
class cSingleton
{
protected:
    // Constructeur/destructeur
    cSingleton() { }
    ~cSingleton() { std::cout << "destroying singleton.\n"; }

public:
    // Interface publique
    static T* getInstance()
    {
        if (NULL == _singleton)
        {
            std::cout << "creating singleton.\n";
            _singleton = new T;
        }
        else
        {
            std::cout << "singleton already created!\n";
        }

        return (static_cast<T*> (_singleton));
    }

    static void kill()
    {
        if (nullptr != _singleton)
        {
            delete _singleton;
            _singleton = nullptr;
        }
    }

private:
    // Unique instance
    static T* _singleton;
};

template <typename T>
T* cSingleton<T>::_singleton = nullptr;


