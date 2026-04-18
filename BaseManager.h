#pragma once

class BaseManager {
public:
    virtual ~BaseManager() {}

    virtual void printAll() const = 0; 
};