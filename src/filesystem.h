/**********************************
*  filesystem.h                   *
*                                 *
*  Provides a simulated           *
*  filesystem that may be used    *
*  when simulating a run.         *
**********************************/
/****************************************************
*                                                   *
****************************************************/
#include <string>
#include <vector>
#include <set>
/*template <class T>
class Nullifier {
    T** ptrptr;
public:
    Nullifier(T** pptrptr) : ptrptr(pptrptr) {}
    ~Nullifier() { *ptrptr = NULL; }
}
class Curious {
    std::set<Curious*> friends;
public:
    void addCurious(Curious* curious);
    void removeCurious(Curious* curious);
    ~Curious();
    virtual void onRemoval(Curious* removed) {};
};*/
class Directory;
class File {
//    std::string path;
    std::string name;
protected:
    Directory* parent;
public:
    enum Type {
        REGULAR,
        DIRECTORY,
        LINK,
    };
    virtual ~File() = 0;
    virtual std::string getName();
//    virtual std::string getPath();
    virtual Directory* getParent();
    virtual bool type() = 0;
    // applicable to files
    virtual size_t getSize() = 0;
    // applicable to directories
    virtual size_t getChildren() = 0; // number of children
    virtual File* getChild(int index) = 0;
};
class RegularFile : public File {
    Directory* parent;
    std::string name;
    size_t size;
public:
    virtual std::string getName();
    virtual Directory* getParent();
    virtual bool type();
    virtual size_t getSize();
    virtual size_t getChildren();
    virtual File* getChild(int index);
};
class Directory : public File {
public:
    virtual bool type();
};
class SubDirectory : public Directory {
    virtual bool type()
};
class RootDirectory : public Directory {
public:
    virtual Directory* getParent();
}
class Link : File {
    std::string target;
public:
    
};
class Filesystem {
    Directory root;
/*    class Record{
    public:
        File* file;
        bool exists;
    };*/
public:
    
};










