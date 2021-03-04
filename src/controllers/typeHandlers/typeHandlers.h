#pragma once
#include "typeHandler.h"


// include handlers
#include "book.h"
#include "bookChapter.h"
#include "conferenceProceeding.h"
#include "journalArticle.h"
#include "webpage.h"
#include "website.h"


// list handlers
constexpr const std::array<const ReferenceTypeHandler, 6> handlers {
    book,
    bookChapter,
    conferenceProceeding,
    journalArticle,
    webpage,
    website
};