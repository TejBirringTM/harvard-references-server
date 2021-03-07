#pragma once
#include "_typeHandler.h"


// include handlers
#include "_book.h"
#include "_bookChapter.h"
#include "_conferenceProceeding.h"
#include "_journalArticle.h"
#include "_webpage.h"
#include "_website.h"


// list handlers
inline constexpr const std::array<const ReferenceTypeHandler, 6> handlers {
    book,
    bookChapter,
    conferenceProceeding,
    journalArticle,
    webpage,
    website
};