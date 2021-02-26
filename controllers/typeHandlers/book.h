#ifndef HARVARD_REFERENCES_SERVER_BOOK_H
#define HARVARD_REFERENCES_SERVER_BOOK_H
#include "../harvard-references.h"
#include "../../includes/json.h"
#include "../../schema/fields.h"
#include "../../includes/utils.h"

inline controllers::harvardReferences::ReferenceTypeHandler book = {
        "book",
        {
            fields["book title"].asMandatory(),

            fields["translated title"],
            fields["language of original"],
            fields["translators"],
            fields["year translated"],

            fields["authors"],
            fields["corporate author"],
            fields["editors"],

            fields["volume #"],
            fields["edition #"],
            fields["publisher"].asMandatory(),
            fields["publisher location"].asMandatory(),
            fields["year published"].asMandatory(),

            fields["series title"],
            fields["# in series"],

            fields["url"],
            fields["date accessed"],
            fields["doi"]
        },
        [](const nlohmann::json &req, crow::response &res) {
            send_response(res, nlohmann::json({
                {"string", "special"},
                {"html", "special"}
            }));
        }
};

#endif //HARVARD_REFERENCES_SERVER_BOOK_H
