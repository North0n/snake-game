#pragma once
#include <memory>
#include <mutex>

#define SINGLETON(type) Singleton<type>::instance().get()

template <class T>
class Singleton
{
public:
    using pointer_t = std::unique_ptr<T>;

    static const pointer_t& instance()
    {
        if (m_instance == nullptr) {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_instance == nullptr) {
                m_instance = std::make_unique<T>();
            }
        }
        return m_instance;
    }

    static void clean()
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_instance.reset();
    }

private:
    static pointer_t m_instance;
    static std::mutex m_mutex;
};

template <class T>
typename Singleton<T>::pointer_t Singleton<T>::m_instance;

template <class T>
typename std::mutex Singleton<T>::m_mutex;
