#ifndef HARVARD_REFERENCES_SERVER_DATE_TIME_H
#define HARVARD_REFERENCES_SERVER_DATE_TIME_H
#include "../../../schema/fields.h"
#include "boost/regex.hpp"
#include "../../../includes/html.h"
#include <string>
#include <iostream>
#include <chrono>
#include <exception>

inline std::string monthToString(const unsigned month) {
    switch(month) {
        case 1:
            return "January";
        case 2:
            return "February";
        case 3:
            return "March";
        case 4:
            return "April";
        case 5:
            return "May";
        case 6:
            return "June";
        case 7:
            return "July";
        case 8:
            return "August";
        case 9:
            return "September";
        case 10:
            return "October";
        case 11:
            return "November";
        case 12:
            return "December";
    }
    throw std::invalid_argument("Invalid month!"); // means: tighten input/regex!
}

inline std::string dayToString(const unsigned day) {
    if (day > 0 && day < 32) {
        return html::ord(day);
    }
    throw std::invalid_argument("Invalid day!"); // means: tighten input/regex!
}

/*
 *      Convert date represented by 'dateString' to full date string.
 *      dateString: yyyy-mm-dd
 */
inline std::string toLongDateString(const std::string& dateString) {
    using namespace std;
    static const auto rDate = boost::regex(REGEX_DATE);
    // parse string
    boost::smatch m;
    boost::regex_search(dateString, m, rDate);
    // m[1] => year
    // m[2] => month
    // m[3] => day
    const unsigned uYear = stoul( m[1] );
    const unsigned uMonth = stoul( m[2] );
    const unsigned uDay = stoul( m[3] );
    //
    return dayToString(uDay) + " " + monthToString(uMonth) + " " + to_string(uYear);
}
#endif //HARVARD_REFERENCES_SERVER_DATE_TIME_H
