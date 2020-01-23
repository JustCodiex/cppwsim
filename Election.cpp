#include "Election.h"
#include "Ballot.h"
#include "Country.h"
#include "City.h"
#include "PoliticalParty.h"
#include "ElectoralDistrict.h"

namespace Election {

	const float MAX_SCORE = 20.0f;

	float scoreCityParty(City* pCity, PoliticalParty* pParty, Country* pCountry) {

		float score = 0.0f;
		CountryProfile* pProfile = pCountry->GetProfile();

		float libPos = pParty->GetIdeology()->GetLibertarianStance();
		float colPos = pParty->GetIdeology()->GetCollectiveStance();

		if (pCity->GetCitySize() == City::Size::CITYSIZE_SMALL) {

			for (size_t i = 0; i < (size_t)SOCIAL_CLASS::CLASS_COUNT; i++) {

				switch ((SOCIAL_CLASS)i) {
				case SOCIAL_CLASS::LOWER_CLASS:
					score += SocialClass_SmallCity_WorkingClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				case SOCIAL_CLASS::LOWER_MIDDLE_CLASS:
					score += SocialClass_SmallCity_LowerMiddleClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				case SOCIAL_CLASS::MIDDLE_CLASS:
					score += SocialClass_SmallCity_MiddleClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				case SOCIAL_CLASS::UPPER_MIDDLE_CLASS:
					score += SocialClass_SmallCity_UpperMiddleClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				case SOCIAL_CLASS::UPPER_CLASS:
					score += SocialClass_SmallCity_UpperClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				default:
					break;
				}

			}

		} else if (pCity->GetCitySize() == City::Size::CITYSIZE_LARGE) {

			for (size_t i = 0; i < (size_t)SOCIAL_CLASS::CLASS_COUNT; i++) {

				switch ((SOCIAL_CLASS)i) {
				case SOCIAL_CLASS::LOWER_CLASS:
					score += SocialClass_LargeCity_WorkingClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				case SOCIAL_CLASS::LOWER_MIDDLE_CLASS:
					score += SocialClass_LargeCity_LowerMiddleClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				case SOCIAL_CLASS::MIDDLE_CLASS:
					score += SocialClass_LargeCity_MiddleClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				case SOCIAL_CLASS::UPPER_MIDDLE_CLASS:
					score += SocialClass_LargeCity_UpperMiddleClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				case SOCIAL_CLASS::UPPER_CLASS:
					score += SocialClass_LargeCity_UpperClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				default:
					break;
				}

			}

		} else if (pCity->GetCitySize() == City::Size::CITYSIZE_METROPOLIS) {

			for (size_t i = 0; i < (size_t)SOCIAL_CLASS::CLASS_COUNT; i++) {

				switch ((SOCIAL_CLASS)i) {
				case SOCIAL_CLASS::LOWER_CLASS:
					score += SocialClass_Metropolis_WorkingClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				case SOCIAL_CLASS::LOWER_MIDDLE_CLASS:
					score += SocialClass_Metropolis_LowerMiddleClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				case SOCIAL_CLASS::MIDDLE_CLASS:
					score += SocialClass_Metropolis_MiddleClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				case SOCIAL_CLASS::UPPER_MIDDLE_CLASS:
					score += SocialClass_Metropolis_UpperMiddleClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				case SOCIAL_CLASS::UPPER_CLASS:
					score += SocialClass_Metropolis_UpperClass(libPos, colPos, pCity->IsCoastal(), pCity->HasUniversity(), pProfile->countryLibertarianModifier, pProfile->countryCollectivismModifier);
					break;
				default:
					break;
				}

			}

		}

		return score;

	}

	float scoreNationalParty(PoliticalParty* pParty, ElectoralDistrict* pDistrict, Country* pCountry) {

		float score = 0.0f;
		float cityScores = 0.0f;

		for (auto city : pDistrict->GetCities()) {
			cityScores += scoreCityParty(city.city, pParty, pCountry) * city.partition;
		}

		score = (float)(cityScores / pDistrict->GetCities().size());

		return score;

	}

	float scoreNationalCandidate(Politician* pPolitician) {

		float score = 0.0f;



		return fminf(fmaxf(score, 0.0f), 10.0f);

	}

	ElectionResult National(Ballot* pBallot, ElectoralDistrict* pDistrict, Country* pCountry) {

		ElectionResult results;
		unsigned int popSize = (unsigned int)(pDistrict->GetVoterCount() * (1.0f - pCountry->GetProfile()->countryPoliticalApathy));

		std::map<Politician*, float> scores;
		float scoreSum = 0.0f;

		for (auto candidate : pBallot->GetCandidates()) {

			float nationalPartyScore = 0.0f;
			
			if (!candidate->IsIndependent()) {
				nationalPartyScore = scoreNationalParty(candidate->GetParty(), pDistrict, pCountry) * candidate->GetPartyLoyalty();
			}

			float nationalCandidateScore = scoreNationalCandidate(candidate);

			scores[candidate] = (nationalPartyScore + nationalCandidateScore);
			scoreSum += scores[candidate];

		}

		int totalPseudoVotes = 0;
		std::map<Politician*, int> pseudoVoteScore;

		for (auto voteScore : scores) {
			pseudoVoteScore[voteScore.first] = (int)(voteScore.second * (scoreSum / (float)pBallot->GetBallotSize()) * popSize);
			totalPseudoVotes += pseudoVoteScore[voteScore.first];
		}

		for (auto vote : pseudoVoteScore) {
			results.votes[vote.first] = (int)((vote.second / (double)totalPseudoVotes) * popSize);
		}

		return results;

	}

}
