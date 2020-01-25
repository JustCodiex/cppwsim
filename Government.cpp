#include "Government.h"
#include "LegislativeChamber.h"

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
	m_govType = GovernmentType::Anarchism;

}

void Government::NewGovernment(LegislativeChamber* chamber) {

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

    m_govCoalition = bestCoalition;

}

void Government::AppointGovernment() {

}

void Government::ElectGovernment() {

}
