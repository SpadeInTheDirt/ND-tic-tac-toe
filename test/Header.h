#pragma once

#include <iostream>
#include <vector>
#include <string>

inline
int int_pow(const int& base, const int& exp) {
	int ret = 1;
	for (int i = 0; i < exp; i++) {
		ret *= base;
	}

	return ret;
}

//inline
template <typename T>
std::vector<T> slice(const std::vector<T>& vector, const int& start, const int& end) {
	std::vector<T> ret(0);

	for (int i = start; i < end; i++) {
		ret.push_back(vector[i]);
	}

	return ret;
}