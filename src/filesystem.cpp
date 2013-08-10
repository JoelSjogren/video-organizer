#include <algorithm>
#include <set>
#include <string>
#include <cassert>
using std::string;
using std::set;
using std::find;
/**********************************************
*  Curious 									  *
**********************************************/
/*void addCurious(Curious* curious) {
    friends.push_back(curious);
}
void removeCurious(Curious* curious) {
    onRemoval(curious); // should this really be done first?
    vector<Curious*>::iterator i;
    i = find(friends.begin(), friends.end(), curious);
    if (i != friends.end()) {
        friends.erase(i);
    } else {
        // is this an error?
    }
}
~Curious() {
    while (friends.size() != 0) {
        Curious* i = *friends.rbegin();
        friends.pop_back();
        i->removeCurious(this);
    }
}*/
/**********************************************
*  File             						  *
**********************************************/
string getName() { return name; }
Directory* getParent() { return parent; }
/**********************************************
*  RegularFile      						  *
**********************************************/
bool type() { return REGULAR; }
size_t getSize() { return size; }
size_t getChildren() { return 0; }
File* getChild(int index) { assert(0); }
/**********************************************
*  Directory    							  *
**********************************************/
bool type() { return DIRECTORY; }
size_t getSize() { return 0; }
size_t getChildren() { return children.size(); }
File* getChild(int index) { return children[index]; }
/**********************************************
*  RootDirectory   							  *
**********************************************/
RootDirectory::RootDirectory() : parent(this) {}
/**********************************************
*  Link 									  *
**********************************************/

/**********************************************
*  Filesystem								  *
**********************************************/

