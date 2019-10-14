#include "../includes_usr/fileIO.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
/* clears, then loads books from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries in books
 * 			SUCCESS if all data is loaded
 * */
int loadBooks(std::vector<book> &books, const char* filename)
{
	books.clear();

	ifstream myfile;
	myfile.open(filename);
	if(!myfile.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}
	else if (myfile.peek() == std::ifstream::traits_type::eof()) {
		return NO_BOOKS_IN_LIBRARY;
	}
	else {
		std::string line;
		std::string token;
		std::string token2;
		std::string token3;
		book myBook;
		stringstream ss;

		while (getline(myfile, line)) {
			book myBook;
			ss.str(line);

			getline(ss, token, ',');
			myBook.book_id = stoi(token);

			getline(ss, myBook.title, ',');

			getline(ss, myBook.author, ',');

			getline(ss, token2, ',');
			myBook.state = static_cast<book_checkout_state>(stoi(token2));

			getline(ss, token3, ',');
			myBook.loaned_to_patron_id = stoi(token3);

			books.push_back(myBook);
			ss.clear();
		}

	}
	return SUCCESS;
}

/* serializes books to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries books (do not create file)
 * 			SUCCESS if all data is saved
 * */
int saveBooks(std::vector<book> &books, const char* filename)
{
	ofstream myFile;
	myFile.open(filename);

	if (!myFile.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}
	else if (books.empty()){
		return NO_BOOKS_IN_LIBRARY;
	}
	else{
		string myData;

		for (int var = 0; var < books.size(); ++var) {
			myData = to_string(books[var].book_id) + "," + books[var].title + "," + books[var].author + "," + to_string(books[var].state) + "," + to_string(books[var].loaned_to_patron_id);
			myFile<<myData<<std::endl;
		}
		if (myFile.is_open()) {
			myFile.close();
		}
	}


	return SUCCESS;
}

/* clears, then loads patrons from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons
 * 			SUCCESS if all data is loaded
 * */
int loadPatrons(std::vector<patron> &patrons, const char* filename)
{
	patrons.clear();

	ifstream myfile;
	myfile.open(filename);
	if(!myfile.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}
	else if (myfile.peek() == std::ifstream::traits_type::eof()) {
		return NO_PATRONS_IN_LIBRARY;
	}
	else {
		std::string line;
		std::string token;
		std::string token2;
		patron myPatron;
		stringstream ss;

		while (getline(myfile, line)) {
			patron myPatron;
			ss.str(line);

			getline(ss, token, ',');
			myPatron.patron_id = stoi(token);

			getline(ss, myPatron.name, ',');

			getline(ss, token2, ',');
			myPatron.number_books_checked_out = stoi(token2);

			patrons.push_back(myPatron);
			ss.clear();
		}

	}

	return SUCCESS;
}

/* serializes patrons to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons  (do not create file)
 * 			SUCCESS if all data is saved
 * */
int savePatrons(std::vector<patron> &patrons, const char* filename)
{
	ofstream myFile;
	myFile.open(filename);

	if (!myFile.is_open()) {
		return COULD_NOT_OPEN_FILE;
	}
	else if (patrons.empty()){
		return NO_PATRONS_IN_LIBRARY;
	}
	else{
		string myData;

		for (int var = 0; var < patrons.size(); ++var) {
			myData = to_string(patrons[var].patron_id) + "," + patrons[var].name + "," + to_string(patrons[var].number_books_checked_out);
			myFile<<myData<<std::endl;
		}
		if (myFile.is_open()) {
			myFile.close();
		}
	}


	return SUCCESS;
}
