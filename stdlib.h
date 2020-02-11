#pragma once
// STD library methods
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

// Project Structures
#include "Random.h"
#include "TimeDate.h"
#include "StringUtilities.h"

// Type large enough for population sizes
// May also be used to represent how many of a population do something (like votes)
typedef unsigned long long PopSize;

// Unsigned integer (32 bit)
typedef unsigned int UInt32;

// Unsigned integer (64 bit)
typedef unsigned long long UInt64;

// Signed Integer (64 bit)
typedef signed long long Int64;

// Unsigned short (16 bit)
typedef unsigned short UInt16;

// Try and find an element in a vector
#define FIND_VECTOR_ELEMENT(v, e) std::find(v.begin(), v.end(), e)

// Does the vector contain the specified element
#define CONTAINS_VECTOR_ELEMENT(v, e) (std::find(v.begin(), v.end(), e) != v.end())
