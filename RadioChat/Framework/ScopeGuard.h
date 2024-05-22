#pragma once

#include <functional>

class ScopeGuard {
public: 
    template<class Callable>
    explicit ScopeGuard(Callable&& func) noexcept : func(std::forward<Callable>(func)) {}
    ~ScopeGuard()
    {
        if (func) {
            func(); // must not throw
        }
    }

private:
    std::function<void()> func;
};
