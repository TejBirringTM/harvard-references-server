#pragma onoce
#include "../../schema/schema.h"
#include "boost/regex.hpp"
#include "html.h"
#include <string>
#include <iostream>
#include <chrono>
namespace utils::dateTime {


    inline std::string monthToString(const unsigned month) {
        switch (month) {
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
        throw ControllerError("Invalid month!"); // means: tighten input/regex!
    }


    inline std::string dayToString(const unsigned day) {
        if (day > 0 && day < 32) {
            return html::ord(day);
        }
        throw ControllerError("Invalid day of month!"); // means: tighten input/regex!
    }


/*
 *      Convert date represented by 'dateString' to full date string.
 *      dateString: yyyy-mm-dd
 */
    inline std::string toLongDateString(const std::string &dateString) {
        using namespace std;
        static const auto rDate = boost::regex(schema::REGEX_DATE);
        // parse string to regex match
        // m[1] => year
        // m[2] => month
        // m[3] => day
        boost::smatch m;
        boost::regex_search(dateString, m, rDate);
        // parse to uint
        const unsigned uYear = stoul(m[1]);
        const unsigned uMonth = stoul(m[2]);
        const unsigned uDay = stoul(m[3]);
        //
        return dayToString(uDay) + " " + monthToString(uMonth) + " " + to_string(uYear);
    }


/*
 *  beginDate: yyyy-mm-dd
 *  endDate: yyyy-mm-dd
 */
    inline std::string toDateRangeString(const std::string &beginDate, const std::string &endDate) {
        using namespace std;
        static const auto rDate = boost::regex(schema::REGEX_DATE);
        // parse strings to regex matches
        // m*[1] => year
        // m*[2] => month
        // m*[3] => day
        boost::smatch mBegin;
        boost::regex_search(beginDate, mBegin, rDate);
        boost::smatch mEnd;
        boost::regex_search(endDate, mEnd, rDate);
        // parse to uints
        const unsigned uYearBegin = stoul(mBegin[1]);
        const unsigned uMonthBegin = stoul(mBegin[2]);
        const unsigned uDayBegin = stoul(mBegin[3]);
        const unsigned uYearEnd = stoul(mEnd[1]);
        const unsigned uMonthEnd = stoul(mEnd[2]);
        const unsigned uDayEnd = stoul(mEnd[3]);
        //
        const bool dayMatch = (uDayBegin == uDayEnd);
        const bool monthMatch = (uMonthBegin == uMonthEnd);
        const bool yearMatch = (uYearBegin == uYearEnd);
        // if same day
        if (yearMatch && monthMatch && dayMatch) {
            return toLongDateString(beginDate);
        }
            // if same month
        else if (yearMatch && monthMatch) {
            return dayToString(uDayBegin) + "-" + dayToString(uDayEnd)
                   + " " + monthToString(uMonthBegin) + ", " + to_string(uYearBegin);
        }
            // if same year
        else if (yearMatch) {
            return dayToString(uDayBegin) + " " + monthToString(uMonthBegin) + "-"
                   + dayToString(uDayEnd) + " " + monthToString(uMonthEnd) + ", "
                   + to_string(uYearBegin);
        }
            // different
        else {
            return toLongDateString(beginDate) + "-" + toLongDateString(endDate);
        }
    }


}