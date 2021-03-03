#ifndef HARVARD_REFERENCES_SERVER_NAMES_H
#define HARVARD_REFERENCES_SERVER_NAMES_H
#include <vector>
#include <string>
#include <iostream>
#include "boost/algorithm/string.hpp"
namespace utils::names {


// private stuff
    namespace {
        using StringArray = std::vector<std::string>;
    }


/**
 *
 * Get this author's name in a format suitable for attribution in a Harvard references list,
 * - that is, with the "last name" at the front.
 *
 * i.e. in the format:
 * <Firstname> <Middle Names> <Lastname> => Lastname F.M.N.
 *
 */
    inline std::string reverseAbbreviatedName(const std::string &name) {
        using namespace boost::algorithm;
        StringArray sa;
        boost::algorithm::split(sa, name, is_any_of(" "), token_compress_on);

        // If just one word, return as it is
        if (sa.size() == 1) {
            sa[0][0] = toupper(sa[0][0]);
            return sa[0];
        }

        // Otherwise:
        std::stringstream ss;
        // Capitalize last name
        (sa.back())[0] = toupper((sa.back())[0]);
        ss << sa.back() << ", ";
        // Abbreviate (+ capitalize) all but last name
        for (auto it = sa.begin(); it != sa.end() - 1; ++it) {
            ss.put(toupper((*it)[0]));
            ss << ".";
        }

        return ss.str();
    }


/**
 *
 * Get this author's name in a Harvard-style abbreviated form - with the "last name" at the end,
 * i.e. in the format:
 * <Firstname> <Middle Names> <Lastname> => F.M.N. Lastname
 *
 */
    inline std::string abbreviatedName(const std::string &name) {
        using namespace boost::algorithm;
        StringArray sa;
        boost::algorithm::split(sa, name, is_any_of(" "), token_compress_on);

        // If just one word, return as it is
        if (sa.size() == 1) {
            sa[0][0] = toupper(sa[0][0]);
            return sa[0];
        }


        // Otherwise:
        std::stringstream ss;
        // Abbreviate (+ capitalize) all but last name
        for (auto it = sa.begin(); it != sa.end() - 1; ++it) {
            ss.put(toupper((*it)[0]));
            ss << ".";
        }
        // Capitalize last name
        (sa.back())[0] = toupper((sa.back())[0]);
        ss << " " << sa.back();

        return ss.str();
    }


    inline StringArray reverseAbbreviatedNames(const StringArray &names) {
        std::vector<std::string> retVal;

        for (const auto &name : names) {
            retVal.push_back(reverseAbbreviatedName(name));
        }

        return retVal;
    }


    inline StringArray abbreviatedNames(const StringArray &names) {
        std::vector<std::string> retVal;

        for (const auto &name : names) {
            retVal.push_back(abbreviatedName(name));
        }

        return retVal;
    }


    inline std::string joinList(const StringArray &items) {
        if (items.size() == 1) {
            return items[0];
        } else if (items.size() == 2) {
            return items[0] + " and " + items[1];
        } else {
            StringArray tmp(items);
            const std::string lastItem = tmp.back();
            tmp.pop_back();

            return boost::algorithm::join(tmp, ", ") + " and " + lastItem;
        }
    }




}
#endif //HARVARD_REFERENCES_SERVER_NAMES_H
