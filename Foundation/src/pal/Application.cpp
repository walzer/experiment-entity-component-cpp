#include "pch.h"

#include "pal/Application.h"

namespace cc {;

Application *Application::s_instance;

Application::Application() {
    s_instance = this;
}

Application::~Application() {
    s_instance = nullptr;
}

}   // namespace cc
