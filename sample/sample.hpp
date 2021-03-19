#include <string>

namespace Engine {
    class [[Serialize]] GameObject {
    private:
        int id;
        std::string name;
    };
}

namespace User {
    class [[Serialize]] Error {
    private:
        int id;
        std::string name;
    };
}