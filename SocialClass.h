#pragma once

typedef unsigned long long PopSize;

enum class SOCIAL_CLASS : int {
	
	LOWER_CLASS,
	WORKING_CLASS = LOWER_CLASS,
	LOWER_MIDDLE_CLASS,
	MIDDLE_CLASS,
	UPPER_MIDDLE_CLASS,
	UPPER_CLASS,

	CLASS_COUNT,

};

float SocialClass_SmallCity_WorkingClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);
float SocialClass_SmallCity_LowerMiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);
float SocialClass_SmallCity_MiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);
float SocialClass_SmallCity_UpperMiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);
float SocialClass_SmallCity_UpperClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);

float SocialClass_LargeCity_WorkingClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);
float SocialClass_LargeCity_LowerMiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);
float SocialClass_LargeCity_MiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);
float SocialClass_LargeCity_UpperMiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);
float SocialClass_LargeCity_UpperClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);

float SocialClass_Metropolis_WorkingClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);
float SocialClass_Metropolis_LowerMiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);
float SocialClass_Metropolis_MiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);
float SocialClass_Metropolis_UpperMiddleClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);
float SocialClass_Metropolis_UpperClass(float lib, float col, bool isCoastal, bool hasUniversity, float libModifier, float colModifier);
