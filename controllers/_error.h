#pragma once
#include <exception>


struct ControllerError : public std::runtime_error {
public:
    explicit ControllerError(const std::string&& what) :
    runtime_error(what)
    {}
};

