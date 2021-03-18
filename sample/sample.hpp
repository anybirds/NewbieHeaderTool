#include <string>

class [[generate::serialize]] GameObject {
private:
    int id;
    std::string name;
};
