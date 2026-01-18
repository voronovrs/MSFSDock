#include "UIManager.hpp"

UIManager& UIManager::Instance() {
    static UIManager instance;
    return instance;
}

void UIManager::Register(IUIUpdatable* ui) {
    if (!ui) return;

    std::lock_guard<std::mutex> lock(uiMutex_);
    uiObjects_.insert(ui);
}

void UIManager::Unregister(IUIUpdatable* ui) {
    if (!ui) return;

    std::lock_guard<std::mutex> lock(uiMutex_);
    uiObjects_.erase(ui);
}

void UIManager::UpdateAll() {
    std::lock_guard<std::mutex> lock(uiMutex_);

    for (auto* ui : uiObjects_) {
        if (ui) {
            ui->UpdateImage();
        }
    }
}
