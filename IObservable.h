#pragma once
#include <string>
#include <vector>

class IGameObserver {
public:
    virtual ~IGameObserver() = default;
    virtual void onGameEvent(const std::string& message) = 0;
};

class IObservable {
protected:
    std::vector<IGameObserver*> observers;
public:
    virtual ~IObservable() = default;

    void addObserver(IGameObserver* o) {
        observers.push_back(o);
    }

    void notify(const std::string& message) {
        for (auto* o : observers) {
            o->onGameEvent(message);
        }
    }
};