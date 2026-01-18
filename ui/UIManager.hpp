#pragma once

#include <unordered_set>
#include <mutex>

// All UI objects should inherit this for broadcast updates
struct IUIUpdatable {
    virtual ~IUIUpdatable() = default;
    virtual void UpdateImage() = 0;
};

class UIManager {
public:
    static UIManager& Instance();

    void Register(IUIUpdatable* ui);
    void Unregister(IUIUpdatable* ui);

    void UpdateAll();

private:
    UIManager() = default;
    ~UIManager() = default;

    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;

    std::unordered_set<IUIUpdatable*> uiObjects_;
    std::mutex uiMutex_;
};
