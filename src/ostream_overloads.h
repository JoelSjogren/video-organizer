/**********************************
*  ostream_test.h                 *
*                                 *
*  Overloads ostream::operator<<  *
*  for simplified printing.       *
**********************************/
#pragma once
#include <ostream>
#include <vector>
template <class T>
std::ostream&
operator<<(std::ostream& os, const std::vector<T>& vec) {
	if (vec.size() == 0) return os << "[]";
	os << "[";
	int i;
	for (i = 0; i < vec.size() - 1; i++)
		os << vec[i] << ", ";
	os << vec[i] << "]";
	return os;
}
