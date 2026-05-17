#ifndef BASE_MANAGER_H
#define BASE_MANAGER_H

#include <string>

class BaseManager {
public:
    virtual ~BaseManager() {}
    virtual void loadFromFile(const std::string& filename) = 0;
    virtual void saveToFile(const std::string& filename) const = 0;
    virtual int size() const = 0;
};

#endif