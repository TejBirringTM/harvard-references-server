#pragma once
#include <string>
#include "boost/regex.hpp"


template<typename T>
inline std::string singleQuote(const T& str) {
    return std::string("'" + std::to_string(str) + "'");
}
inline std::string singleQuote(const std::string_view& str) {
    return std::string("'" + std::string(str) + "'");
}
inline std::string singleQuote(const std::string& str) {
    return std::string("'" + str + "'");
}


template<size_t N>
std::string listFields(const std::array<std::string_view, N>& fields) {
    std::string tmp;
    for (int i=0; i<N; ++i) {
        if ( fields[i].empty() ) break;
        if (i>0 && i<N)
            tmp += ", ";
        tmp += fields[i];
    }
    tmp = boost::regex_replace(tmp, boost::regex(R"((^))"), "'");
    tmp = boost::regex_replace(tmp, boost::regex(R"((, ))"), "', '");
    tmp = boost::regex_replace(tmp, boost::regex(R"(($))"), "'");
    return std::move( tmp );
}


template<size_t N>
std::string listFields(const std::string_view& field, const std::array<std::string_view, N>& fields) {
    std::string tmp(field);
    for (int i=0; i<N; ++i) {
        if ( fields[i].empty() ) break;
        if (i<N)
            tmp += ", ";
        tmp += fields[i];
    }
    tmp = boost::regex_replace(tmp, boost::regex(R"((^))"), "'");
    tmp = boost::regex_replace(tmp, boost::regex(R"((, ))"), "', '");
    tmp = boost::regex_replace(tmp, boost::regex(R"(($))"), "'");
    return std::move( tmp );
}


template<typename T, size_t N, typename M>
std::vector<T> exclude(const std::array<T,N>& in, const M valToExclude) {
    using namespace std;
    vector<T> ret;
    for_each(in.cbegin(), in.cend(), [&ret, &valToExclude](const T& el){
        if (el != valToExclude)
            ret.push_back(el);
    });
    return std::move( ret );
}