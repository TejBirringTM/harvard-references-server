#pragma once
#include <exception>


struct SchemaError : public std::runtime_error {
public:
    explicit SchemaError(const std::string&& what) :
            runtime_error(what)
    {}
};


