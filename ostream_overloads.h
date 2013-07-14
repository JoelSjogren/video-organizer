#pragma once
#include <ostream>
#include <vector>
#include <string>
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