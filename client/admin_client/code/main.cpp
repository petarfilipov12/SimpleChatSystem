#include "console_app.h"

#define REST_API_ENDPOINT "http://127.0.0.1:8080"

int main() {
    cansole_app::ConsoleApp app(REST_API_ENDPOINT);
    app.Run();

    return 0;
}