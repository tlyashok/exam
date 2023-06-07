#ifndef SINGLETON_H
#define SINGLETON_H

template <class T> class Singleton
{
    protected:
        static inline T * p_instance = nullptr;
        Singleton();
        Singleton(const Singleton& );
        Singleton& operator = (Singleton &);
    public:
        static T* getInstance();
};

template<class T>
Singleton<T>::Singleton()
{

}

template<class T>
T* Singleton<T>::getInstance()
{
    if (!p_instance)
        p_instance = new T();
    return p_instance;
}

#endif // SINGLETON_H
