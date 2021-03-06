/**********************************
*  filelist.h                     *
*                                 *
*  This is where the information  *
*  needed to undo operations      *
*  is stored and retrieved.       *
**********************************/
/****************************************************
*  Whenever a file is sorted, a record of the       *
*  operation is written to a file called 'filelist' *
*  in the new directory. This enables FileMan::     *
*  undo() to read the filelist, find the operation  *
*  and undo it.                                     *
*                                                   *
*  The 'filelist' is designed to be user-readable,  *
*  so that the user can open it up in his favorite  *
*  text editor and find more details about the      *
*  file, e.g. its release group.                    *
*                                                   *
*  The 'filelist' is also used in third party       *
*  applications, such as addic7ed-cli (see --help). *
****************************************************/
#pragma once
#include "args.h"
#include "tinyxml2.h" // XML parsing
#include "console.h"
#include <string>
#include <vector>
#include <stdexcept>
class Record {
public:
	// from and to are just the filenames
	std::string from; // e.g. "Film.3.2010.avi", not "dir/file.avi"
	std::string to; // e.g. "Film 3.avi"
	Args::Action action;
	Record(std::string from, std::string to, Args::Action action);
};
class FileList {
private:
	const Args& args;
	const std::string dir;
	std::vector<Record> records;
	Console console;
	void read();
	int getIndex(std::string to) const;
	static void validateXML(tinyxml2::XMLElement* root,
							std::string file);
public:
	FileList(std::string dir, const Args& args);
	void add(std::string from, std::string to);
	Record get(std::string to) const;
	Record get(size_t index) const;
	Record operator[](size_t index) const;
	bool find(std::string to) const;
	void remove(std::string to);
	void remove(size_t index);
	// get an element and remove it from the list
	Record pop();
	size_t size() const;
	void write();
	class RecordNotFound : public std::runtime_error {
	public:
		RecordNotFound(std::string to);
	};
	class BadXML : public std::runtime_error {
	public:
		BadXML(std::string file);
	};
	class NoFile : public std::runtime_error {
	public:
		NoFile(std::string file);
	};
};








