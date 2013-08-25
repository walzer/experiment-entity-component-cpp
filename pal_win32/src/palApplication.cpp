#include "pch.h"

#include "palApplication.h"

namespace cc {;

namespace pal {;

CC_IMPL_COMPONENT_TYPE_INFO(Application, nullptr, nullptr);

Application::Application() {
}

Application::~Application() {
}

int Application::getErrorCode() {
    return 0;
}

void Application::run() {
}

bool Application::_onInit(const IDom *args) {
    return true;
}

void Application::_onEnable() {
}

void Application::_onDisable() {
}

void Application::_onDone() {
}

}   // namespace pal

}   // namespace cc
