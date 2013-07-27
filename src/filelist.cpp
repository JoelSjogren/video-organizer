/**********************************
*  filelist.cpp                  *
**********************************/
#include "filelist.h"
#include "args.h"
#include "tinyxml2.h"
#include <stdexcept>
using std::string;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLError;
using std::logic_error;

FileList::FileList(string pdir, const Args& pargs)
	: dir(pdir), args(pargs), console(args.verbosity) {
	read();
}
int FileList::getIndex(string to) const {
	for (size_t i = 0; i < records.size(); i++)
		if (records[i].to == to)
			return i;
	throw logic_error("no such record");
}
void FileList::add(string from, string to) {
	// Ensure there is only one record with the same 'to'; overwrite
	if (find(to)) remove(to);
	records.push_back(Record(from, to, args.action));
}
bool FileList::find(string to) const {
	for (size_t i = 0; i < records.size(); i++)
		if (records[i].to == to)
			return true;
	return false;
}
Record FileList::get(string to) const {
	return records[getIndex(to)];
}
Record FileList::get(size_t index) const {
	return records[index];
}
Record FileList::operator[](size_t index) const {
	return records[index];
}
void FileList::remove(size_t index) {
	records.erase(records.begin() + index);
}
void FileList::remove(string to) {
	records.erase(records.begin() + getIndex(to));
}
size_t FileList::size() const {
	return records.size();
}
Record FileList::pop() {
	Record& result = records[0];
	records.erase(records.begin());
	return result;
}
void FileList::validateXML(tinyxml2::XMLElement* root, string file) {
	if (root == NULL)
		throw NoFile(file);
	if (strcmp(root->Value(), "filelist") != 0)
		throw BadXML(file);
	if (strcmp(root->Attribute("application"),
			   "video-organizer") != 0)
		throw BadXML(file);
	if (strcmp(root->Attribute("version"), "1.0") != 0)
		throw BadXML(file);
}
void FileList::read() {
	XMLDocument doc;
	const string filepath = dir + "filelist";
	doc.LoadFile(filepath.c_str());
	XMLElement* filelist = doc.FirstChildElement();
	try { validateXML(filelist, filepath); }
	catch (NoFile) { return; }
	for (XMLElement* i = filelist->FirstChildElement(); i != NULL;
		 i = (XMLElement*) i->NextSibling()) {
		const string from = i->Attribute("from");
		const string to = i->Attribute("to");
		const Args::Action action =
			static_cast<Args::Action>(i->IntAttribute("action"));
		records.push_back(Record(from, to, action));
	}
}
void FileList::write() {
	if (args.simulate) return;
	XMLDocument doc;
	{
		XMLElement* filelist = doc.NewElement("filelist");
		filelist->SetAttribute("application", "video-organizer");
		filelist->SetAttribute("version", "1.0");
		for (int i = 0; i < records.size(); i++) {
			XMLElement* record = doc.NewElement("record");
			record->SetAttribute("from", records[i].from.c_str());
			record->SetAttribute("to", records[i].to.c_str());
			record->SetAttribute("action", records[i].action);
			filelist->InsertEndChild(record);
		}
		doc.InsertEndChild(filelist);
	}
	XMLError ret = doc.SaveFile((dir + "filelist").c_str());
	if (ret) console.e("Error writing filelist: %s", ret);
}
Record::Record(string pfrom, string pto, Args::Action paction)
	: from(pfrom), to(pto), action(paction) {}
FileList::RecordNotFound::RecordNotFound(string to)
	: runtime_error("Record not found: " + to) {}
FileList::BadXML::BadXML(string file)
	: runtime_error("Could not open file: " + file) {}
FileList::NoFile::NoFile(string file)
	: runtime_error("File does not exist: " + file) {}
