#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iostream>

#include "../includes_usr/library.h"
#include "../includes_usr/datastructures.h"
#include "../includes_usr/fileIO.h"
using namespace std;

//NOTE: please ensure patron and book data are loaded from disk before calling the following
//NOTE: also make sure you save patron and book data to disk any time you make a change to them
//NOTE: for files where data is stored see constants.h BOOKFILE and PATRONFILE

vector<book> allBooks;
vector<patron> allPatrons;

/*
 * clear books and patrons containers
 * then reload them from disk 
 */
void reloadAllData(){
	allBooks.clear();
	allPatrons.clear();
	loadBooks(allBooks,BOOKFILE.c_str());
	loadPatrons(allPatrons,PATRONFILE.c_str());
}

/* checkout a book to a patron
 * first load books and patrons from disk
 * make sure patron enrolled (patronid is assigned to a patron in patrons container)
 * make sure book in collection (bookid is assigned to a book in books container)
 * 
 * see if patron can check out any more books 
 * 	if not return TOO_MANY_OUT patron has the MAX_BOOKS_ALLOWED_OUT
 * 	
 * if so then check the book out to the patron, set the following fields for the book in the
 * books container
 * book.loaned_to_patron_id = patronid;
 * book.state = OUT;
 * 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 *         PATRON_NOT_ENROLLED
 * 		   BOOK_NOT_IN_COLLECTION
 *         TOO_MANY_OUT patron has the max number of books allowed checked out
 */
int checkout(int bookid, int patronid){
	reloadAllData();

	bool patronExists = false;
	for (int var = 0; var < allPatrons.size(); ++var) {
		if (allPatrons[var].patron_id == patronid) {
			patronExists = true;
		}
	}
	if (patronExists == false) {
		return PATRON_NOT_ENROLLED;
	}

	bool bookExists = false;
	for (int var = 0; var < allBooks.size(); ++var) {
		if (allBooks[var].book_id == bookid) {
			bookExists = true;
		}
	}
	if (bookExists == false) {
		return BOOK_NOT_IN_COLLECTION;
	}

	if (allPatrons[patronid].number_books_checked_out >= MAX_BOOKS_ALLOWED_OUT) {
		return TOO_MANY_OUT;
	}

	allBooks[bookid].loaned_to_patron_id = patronid;
	allBooks[bookid].state = OUT;
	allPatrons[patronid].number_books_checked_out = allPatrons[patronid].number_books_checked_out + 1;

	savePatrons(allPatrons,PATRONFILE.c_str());
	saveBooks(allBooks,BOOKFILE.c_str());

	return SUCCESS;
}

/* check a book back in 
 * first load books and patrons from disk
 * make sure book in collection (bookid is assigned to a book in books container)
 *  	
 * if so find the the patron the book is checked out to and decrement his/hers number_books_checked_out
 * then check the book back in by marking the book.loaned_to_patron_id = NO_ONE and the book.state = IN;
 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 * 		   BOOK_NOT_IN_COLLECTION
 */
int checkin(int bookid){
	reloadAllData();

	bool bookExists = false;
	for (int var = 0; var < allBooks.size(); ++var) {
		if (allBooks[var].book_id == bookid) {
			bookExists = true;
		}
	}
	if (bookExists == false) {
		return BOOK_NOT_IN_COLLECTION;
	}

	allPatrons[allBooks[bookid].loaned_to_patron_id].number_books_checked_out = allPatrons[allBooks[bookid].loaned_to_patron_id].number_books_checked_out - 1;
	allBooks[bookid].loaned_to_patron_id = NO_ONE;
	allBooks[bookid].state = IN;

	savePatrons(allPatrons,PATRONFILE.c_str());
	saveBooks(allBooks,BOOKFILE.c_str());

	return SUCCESS;
}

/*
 * enroll a patron, duplicate names are fine as patrons are uniquely identified by their patronid
 * first load books and patrons from disk
 * create a patron object, initialize its fields as appropriate, assign him/her the nextPatronID
 * then push the patron onto the patrons container
 * save all changes to the patrons container to disk
 * return 
 *    the patron_id of the person added
 */
int enroll(std::string &name){
	reloadAllData();
	patron newPatron;
	newPatron.name = name;
	newPatron.number_books_checked_out = 0;
	newPatron.patron_id = allPatrons.size();
	allPatrons.push_back(newPatron);

	savePatrons(allPatrons,PATRONFILE.c_str());

	return allPatrons.size() - 1;
}

/*
 * the number of books in the books container
 * (ie. if 3 books returns 3)
 * 
 */
int numbBooks(){
	reloadAllData();
	return allBooks.size();
}

/*
 * the number of patrons in the patrons container
 * (ie. if 3 patrons returns 3)
 */
int numbPatrons(){
	reloadAllData();
	return allPatrons.size();
}

/*the number of books patron has checked out
 *
 *returns a positive number indicating how many books are checked out 
 *        or PATRON_NOT_ENROLLED         
 */
int howmanybooksdoesPatronHaveCheckedOut(int patronid){
	reloadAllData();
	bool patronExists = false;
	for (int var = 0; var < allPatrons.size(); ++var) {
		if (allPatrons[var].patron_id == patronid) {
			patronExists = true;
		}
	}
	if (patronExists == false) {
		return PATRON_NOT_ENROLLED;
	}
	return allPatrons[patronid].number_books_checked_out;
}

/* search through patrons container to see if patronid is there
 * if so returns the name associated with patronid in the variable name
 * 
 * returns SUCCESS found it and name in name
 *         PATRON_NOT_ENROLLED no patron with this patronid
 */
int whatIsPatronName(std::string &name,int patronid){
	reloadAllData();
		bool patronExists = false;
		for (int var = 0; var < allPatrons.size(); ++var) {
			if (allPatrons[var].patron_id == patronid) {
				patronExists = true;
			}
		}
		if (patronExists == false) {
			return PATRON_NOT_ENROLLED;
		}
	//return allPatrons[patronid].name;
	return SUCCESS;
}

