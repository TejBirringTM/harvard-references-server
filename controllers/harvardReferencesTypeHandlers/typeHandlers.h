#ifndef HARVARD_REFERENCES_SERVER_TYPEHANDLERS_H
#define HARVARD_REFERENCES_SERVER_TYPEHANDLERS_H
#include "typeHandler.h"
#include <map>
#include <memory>


#include "book.h"
#include "bookChapter.h"
#include "conferenceProceeding.h"
#include "journalArticle.h"
#include "webpage.h"
#include "website.h"




constexpr const std::array<const ReferenceTypeHandler, 6> handlers {
    book,
    bookChapter,
    conferenceProceeding,
    journalArticle,
    webpage,
    website
};




#endif //HARVARD_REFERENCES_SERVER_TYPEHANDLERS_H
