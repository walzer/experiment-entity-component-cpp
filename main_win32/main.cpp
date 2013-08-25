#include "palApplication.h"

int main() {
    ::cc::pal::Application app;
    ::cc::initApplication(app);
    app.enable();
    app.run();
    app.disable();
    app.done();
    return app.getErrorCode();
}
