# Harvard References Server #
A (high-performance) C++ based HTTP server that implements a RESTful API which generates  
(plain-text or HTML-formatted) Harvard-style citations, references, and bibliographies, 
for a myriad types of sources: books, journal articles, conference proceedings, videos, etc.

The motivation behind the project was to enable apps to present (research) articles with 
citations, references, and bibliographies. Useful for projects that 
have to deal with managing or listing academic sources.

---

### Features ### 
This codebase is still 'work-in-progress'. 
The following reference features have been implemented:

* Generate HTML-formatted reference strings for:
  * Books
  * Book Chapters
  * Journal Articles
  * Conference Proceedings
  * Websites
  * Webpages

The following features need to be developed:
* Generate HTML-formatted reference strings for:
  * Videos
  * Podcasts
  * Music
  * (Entire) Conferences
  * Standards
  * Reports
  * Maps
  * Interviews/Correspondences
  * Newspaper/Magazine Articles
* Generate HTML-formatted citation strings.
* Generate HTML-formatted bibliographies.

---

### Testing ###
You can test the current de-facto release (versioning system has not been implemented yet) using the following information:

Endpoint: http://harvard-references-server.herokuapp.com/api/v1.0

1. Use the OPTIONS request to fetch a JSON-formatted schema of acceptable 'reference objects'.
2. Submit a GET request with a JSON-formatted 'reference object' *or* array of 'reference objects' to obtain HTML-formatted reference string(s).

