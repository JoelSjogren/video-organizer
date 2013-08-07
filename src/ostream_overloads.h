/**********************************
*  ostream_test.h                 *
*                                 *
*  Overloads ostream::operator<<  *
*  for simplified printing.       *
**********************************/
#pragma once
#include <ostream>
#include <vector>
// For vector-like containers
template <class T,
    template <class T,
        class=std::allocator<T> > class Container>
std::ostream&
operator<<(std::ostream& os, const Container<T>& cont) {
    typename Container<T>::const_iterator i = cont.begin();
	if (i == cont.end()) return os << "[]";
	os << "[" << *i++;
	for (/* i is set */; i != cont.end(); i++)
		os << ", " << *i;
    os << "]";
	return os;
}
// For set-like containers
template <class T,
    template <class T,
        class=std::less<T>,
        class=std::allocator<T> > class Container>
std::ostream&
operator<<(std::ostream& os, const Container<T>& cont) {
    typename Container<T>::const_iterator i = cont.begin();
	if (i == cont.end()) return os << "[]";
	os << "[" << *i++;
	for (/* i is set */; i != cont.end(); i++)
		os << ", " << *i;
    os << "]";
	return os;
}


