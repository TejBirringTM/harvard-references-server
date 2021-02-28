#ifndef HARVARD_REFERENCES_SERVER_HTML_H
#define HARVARD_REFERENCES_SERVER_HTML_H
#include <string>

namespace html {
    using namespace std;

    const string em = "<em>";
    const string _em = "</em>";
    const string sup = "<sup>";
    const string _sup = "</sup>";

    const string str(const string& in) { return in; }

    const string lnk(const string& url) {
        return "<a href='" + url + "' target='_blank' rel='noreferrer noopener nofollow'>" + url + "</a>";
    }

    const string ord(const unsigned n) {
        const unsigned j = n % 10;
        const unsigned k = n % 100;

        if (j==1 && k!=11) {
            return to_string(n) + sup + "st" + _sup;
        }
        else if (j==2 && k!=12) {
            return to_string(n) + sup + "nd" + _sup;
        }
        else if (j==3 && k!=13) {
            return to_string(n) + sup + "rd" + _sup;
        }
        else {
            return to_string(n) + sup + "th" + _sup;
        }
    }
}
#endif //HARVARD_REFERENCES_SERVER_HTML_H