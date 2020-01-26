#include "Government.h"
#include "LegislativeChamber.h"
#include "Country.h"
#include "RoyalFamily.h"

std::string const getGovTypeName(GovernmentType type) {
	switch (type) {
    case GovernmentType::Democracy:
        return "Democracy";
    case GovernmentType::Oligarchy:
        return "Oligarchy";
    case GovernmentType::ConstitutionalMonarchism:
        return "Constitutional Monarchy";
    case GovernmentType::AbsoluteMonarchism:
        return "Absolute Monarchy";
    case GovernmentType::Anarchism:
        return "Anarchism";
    default:
        return "Unknown";
	}
}

Government::Government() {

    m_targetCountry = 0;
    m_govTerm = 0;
    m_govSupport = 0.0f;
	m_govType = GovernmentType::Anarchism;
    m_electionSystem = GovernmentElectoralSystem::ElectoralCollege;

}

void Government::NewGovernment(LegislativeChamber* chamber, TimeDate date) {

    // Clear ministires
    m_govMinistries.clear();

    // Get the coalitions
    std::vector<LegislativeCoalition> coalitions = chamber->GetCoalitions();
    std::vector<LegislativeCoalition> coalitionCandidates;

    bool hasMajorityCoalition = false;
    LegislativeCoalition bestCoalition;

    // For each coalition
    for (LegislativeCoalition coalition : coalitions) {

        // If coalition is not in opposition
        if (coalition.type != CoalitionType::Opposition) {
            if (coalition.type == CoalitionType::Majority) {
                bestCoalition = coalition;
                hasMajorityCoalition = true;
                break;
            } else {
                coalitionCandidates.push_back(coalition);
            }
        }

    }

    if (!hasMajorityCoalition && coalitionCandidates.size() > 0) {

        bestCoalition = *coalitionCandidates.begin();

        for (LegislativeCoalition coalition : coalitionCandidates) {

            if (coalition.seatMajority > bestCoalition.seatMajority && coalition.stability > bestCoalition.stability) {
                bestCoalition = coalition;
            }

        }

    } else if (!hasMajorityCoalition) { // No coalition can command a majority => pick the least worst

        bestCoalition = *coalitions.begin();

        for (LegislativeCoalition coalition : coalitions) {

            if (coalition.seatMajority > bestCoalition.seatMajority&& coalition.stability > bestCoalition.stability) {
                bestCoalition = coalition;
            }

        }

    }

    // The max minister count
    int maxMinisterCount = (int)PolicyArea::_COUNT_;

    // Available ministers
    std::vector<Politician*> ministers;
    std::map<PoliticalParty*, unsigned short> maxMinisters;

    if (bestCoalition.partners.size() > 0) {

        std::map<PoliticalParty*, unsigned short> seatAllocation;
        unsigned short total = seatAllocation[bestCoalition.leader] = chamber->GetPartySeats(bestCoalition.leader);

        for (auto p : bestCoalition.partners) {
            total += seatAllocation[p] = chamber->GetPartySeats(p);
        }

        for (auto p : seatAllocation) {
            maxMinisters[p.first] = (unsigned short)(maxMinisterCount * (p.second / (float)total));
        }

        for (auto p : maxMinisters) {
            std::vector<Politician*> candidates = p.first->GetMinisterialCandidates();
            for (int i = 0; i < p.second && i < candidates.size(); i++) {
                ministers.push_back(candidates[i]);
            }
        }

    } else {
        
        ministers = bestCoalition.leader->GetMinisterialCandidates();
        maxMinisters[bestCoalition.leader] = maxMinisterCount;

    }

    for (int i = 0; i < maxMinisterCount; i++) {

        PolicyArea tp = (PolicyArea)i;
        Politician* minister = 0;

        for (Politician* pol : ministers) {

            if (pol->GetSpeciality() == tp) {

                if (tp == PolicyArea::State && pol->GetParty() == bestCoalition.leader) {
                    minister = pol;
                } else {
                    minister = pol;
                }

                maxMinisters[pol->GetParty()]--;

                ministers.erase(std::find(ministers.begin(), ministers.end(), pol));
                break;

            }

        }

        m_govMinistries[tp] = GovernmentMinistry();
        m_govMinistries[tp].SetMinister(minister);

    }

    // Remember the government coalition
    m_govCoalition = bestCoalition;

    // Remember the form date
    m_govFormDate = date;

}

void Government::AppointGovernment(Random random, TimeDate date) {

    // Firstly we calculate the government rating
    this->GetGovernmentRating();

    // Get the monarch, as they appoint the government
    Royal* monarch = m_targetCountry->GetRoyalFamily()->GetHeadOfFamily();

    // The max minister count
    int maxMinisterCount = (int)PolicyArea::_COUNT_;

    // No need to consider parliamnetary composition and our popularity
    if (m_govType == GovernmentType::AbsoluteMonarchism) {

        // Is the government already appointed? (eg. do we event want to appoint a new government?)
        if (m_govMinistries.size() > 0) {

            // Does the government currently have support?
            if (m_govSupport > 0.3f) {
                return; // meh, no need to appoint a new government
            }

        }

        // For all minister types
        for (int i = 0; i < maxMinisterCount; i++) {

            // Ministry area
            PolicyArea area = (PolicyArea)i;

            // Does ministry already exist?
            if (m_govMinistries.find(area) != m_govMinistries.end()) {

                // Get ministry
                GovernmentMinistry ministry = m_govMinistries[area];

                // If ministry is "popular" then no need to reappoint, otherwise we appoint new
                if (ministry.GetMinistryApproval() > 0.35f) {
                    continue;
                } else {
                    AppointNewMinister(random, area, monarch, false);
                }

            } else { // The ministry does not exist

                if (i <= (int)PolicyArea::Justice) { // We need at least the first four ministries
                    AppointNewMinister(random, area, monarch, false);
                } else {

                    // Should we make this ministry?
                    if (random.NextBool(0.33f)) {
                        AppointNewMinister(random, area, monarch, false);
                    }

                }

            }

        }

    } else { // We have to consider parliamentary composition and our popularity

        // For all minister types
        for (int i = 0; i < maxMinisterCount; i++) {

            // Ministry area
            PolicyArea area = (PolicyArea)i;

            if (i <= (int)PolicyArea::Justice) { // We need at least the first four ministries
                AppointNewMinister(random, area, monarch, true);
            } else {

                // Should we make this ministry?
                if (random.NextBool(0.33f)) {
                    AppointNewMinister(random, area, monarch, true);
                }

            }

        }

    }

    // Update dates
    m_govFormDate = date;
    m_govNextFormDate = date.addYears(m_govTerm);

}

void Government::AppointNewMinister(Random random, PolicyArea area, Royal* royal, bool considerParliament) {

    // Get the political parties
    std::vector<Country::Party> politicalParties = m_targetCountry->GetPartyList();

    // Clear the ministry if we can
    if (m_govMinistries.find(area) != m_govMinistries.end()) {
        this->RemoveMinistry(&m_govMinistries[area]);
    }

    // The best political party
    PoliticalParty* bestParty = NULL;

    // Find the political party showing the most support for authority
    for (auto nationalParty : politicalParties) {
        if (bestParty == NULL || nationalParty.party->GetIdeology()->GetLibertarianStance() > bestParty->GetIdeology()->GetLibertarianStance()) {
            bestParty = nationalParty.party;
        }
    }

    // Consider parliamentary composition
    if (considerParliament) {

        // Size of party majority
        int bestPartyMajority = 0;
        
        // Does party have any kind of majority
        bool hasMajority = m_targetCountry->GetLegislature()->GetChamber(true)->CanCommandMajority(bestParty, bestPartyMajority);

        // Do they have a majority, are we popular enough to ignore results OR did the party we like get the most seats?
        if (hasMajority || royal->GetPopularity() <= 0.35f || bestParty == m_targetCountry->GetLegislature()->GetChamber(true)->GetLargestParty()) {

            // Generate new minister
            Politician* minister = new Politician(Sex::Male, random);
            minister->SetSpeciality(area);
            minister->SetParty(bestParty, 0.75f);

            // Add minister to party
            bestParty->AddMember(minister);

            // Renew the ministry
            GovernmentMinistry ministry = GovernmentMinistry();
            ministry.SetMinister(minister);

            // Add ministry
            m_govMinistries[area] = ministry;

        } else { // Concession time

            // The party leader of the biggest coalition
            PoliticalParty* pParty = m_targetCountry->GetLegislature()->GetChamber(true)->GetBiggestCoalitionLeader();

            // Make sure it's valid
            if (pParty) {


                // Generate new minister
                Politician* minister = new Politician(Sex::Male, random);
                minister->SetSpeciality(area);
                minister->SetParty(pParty, 0.25f); // We pick someone who's less loyal to the party

                // Add minister to party
                pParty->AddMember(minister);

                // Renew the ministry
                GovernmentMinistry ministry = GovernmentMinistry();
                ministry.SetMinister(minister);

                // Add ministry
                m_govMinistries[area] = ministry;


            } // else, we dont make this ministry....

        }

    } else {

        // Generate new minister
        Politician* minister = new Politician(Sex::Male, random);
        minister->SetSpeciality(area);

        // Is this minsiter part of the most supporting political party?
        if (random.NextBool(0.4f)) {

            // Set ministers party
            minister->SetParty(bestParty, 0.75f);

            // Add minister to party
            bestParty->AddMember(minister);

        }

        // Renew the ministry
        GovernmentMinistry ministry = GovernmentMinistry();
        ministry.SetMinister(minister);

        // Add ministry
        m_govMinistries[area] = ministry;

    }

}

void Government::RemoveMinistry(GovernmentMinistry* ministry) {

    Politician* minister = ministry->GetMinister();
    if (minister->GetParty() == NULL) {
        delete minister;
    }

}

void Government::ElectGovernment(TimeDate date) {
    
    // Update dates
    m_govFormDate = date;
    m_govNextFormDate = date.addYears(m_govTerm);

}

void Government::GetGovernmentRating() {
    
    if (m_govType == GovernmentType::Democracy) { // based on the individual



    } else { // Based on the government as a whole

        m_govSupport = 0.0f;

        for (auto ministry : m_govMinistries) {
            m_govSupport += ministry.second.GetMinistryApproval();
        }

        m_govSupport /= (float)m_govMinistries.size();

    }

}
