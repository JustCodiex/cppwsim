#include "SocialClass.h"
#include <math.h>

const float unit_square_dst = sqrtf(2.0f);

float SocialClass_BaseCalculator(float lib, float col, bool isCoastal, bool hasUniversity, float libMod, float colMod, float libmin, float libmax, float colmin, float colmax) {

	// Score of the ideology
	float score = 0.0f;

	float lib_min = libmin + libMod; // Minimum liberal acceptance
	float lib_max = libmax + libMod; // Maximum liberal acceptance

	float col_min = colmin + colMod; // Minimum collectivist acceptance
	float col_max = colmax + colMod; // Maximum collectivist acceptance

	// If liberal stance is within the stance of the social class
	if (lib_min < lib && lib < lib_max) {
		
		// Add a full point to score
		score++;

		// If there's a university, we'll favour liberals
		if (lib < 0.1f && hasUniversity) {
			score += 0.125f;
		} else if (lib >= 0.1f && !hasUniversity) { // if tthere's not, and party is not liberal, favour conservative/auth
			score += 0.125f;
		}
	}

	// If conservative stance is within stance of the social class
	if (col_min < col && col < col_max) {

		// Add a full point to score
		score++;

		// If coastal, favour collectivism, otherwise not
		if (col < 0.0f && isCoastal) {
			score += 0.125f;
		} else if (col >= 0.0f && !isCoastal) {
			score += 0.125f;
		}
	}

	float libdist = fabsf(lib - libMod); // Calculate liberal distance to country's core
	float coldist = fabsf(col - colMod); // Calculate collectivist distance to country's core
	score += unit_square_dst - sqrtf((libdist * libdist) + (coldist * coldist)); // Add distance modifier to score

	return score / (2.25f + unit_square_dst);

}

float SocialClass_SmallCity_WorkingClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = -0.1f;
	const float libmax = 0.4f;
	const float colmin = 0.1f;
	const float colmax = 0.3f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}

float SocialClass_SmallCity_LowerMiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = 0.0f;
	const float libmax = 0.2f;
	const float colmin = -0.1f;
	const float colmax = 0.2f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}

float SocialClass_SmallCity_MiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = 0.0f;
	const float libmax = 0.7f;
	const float colmin = 0.1f;
	const float colmax = 0.3f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}

float SocialClass_SmallCity_UpperMiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = 0.3f;
	const float libmax = 0.5f;
	const float colmin = 0.5f;
	const float colmax = 0.8f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}

float SocialClass_SmallCity_UpperClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = -0.2f;
	const float libmax = 0.2f;
	const float colmin = 0.8f;
	const float colmax = 1.0f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}

float SocialClass_LargeCity_WorkingClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = -1.0f;
	const float libmax = 0.4f;
	const float colmin = -1.0f;
	const float colmax = -0.6f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}

float SocialClass_LargeCity_LowerMiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = 0.0f;
	const float libmax = 0.2f;
	const float colmin = -0.1f;
	const float colmax = 0.2f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}

float SocialClass_LargeCity_MiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = 0.0f;
	const float libmax = 0.7f;
	const float colmin = 0.1f;
	const float colmax = 0.3f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}

float SocialClass_LargeCity_UpperMiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = 0.3f;
	const float libmax = 0.5f;
	const float colmin = 0.5f;
	const float colmax = 0.8f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}

float SocialClass_LargeCity_UpperClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = -0.2f;
	const float libmax = 0.2f;
	const float colmin = 0.8f;
	const float colmax = 1.0f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}

float SocialClass_Metropolis_WorkingClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = -1.0f;
	const float libmax = 0.4f;
	const float colmin = -1.0f;
	const float colmax = -0.6f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}

float SocialClass_Metropolis_LowerMiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = 0.0f;
	const float libmax = 0.2f;
	const float colmin = -0.1f;
	const float colmax = 0.2f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}

float SocialClass_Metropolis_MiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = 0.0f;
	const float libmax = 0.7f;
	const float colmin = 0.1f;
	const float colmax = 0.3f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}

float SocialClass_Metropolis_UpperMiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = 0.3f;
	const float libmax = 0.5f;
	const float colmin = 0.5f;
	const float colmax = 0.8f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}

float SocialClass_Metropolis_UpperClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier) {
	const float libmin = -0.2f;
	const float libmax = 0.2f;
	const float colmin = 0.8f;
	const float colmax = 1.0f;
	return SocialClass_BaseCalculator(lib, col, isCoastal, hasUniversity, libModifier, colModifier, libmin, libmax, colmin, colmax);
}
