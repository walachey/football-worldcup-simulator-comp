#include "stdafx.h"
#include "ResultSummary.h"

#include "Simulation.h"

namespace sim
{

ResultSummary::ResultSummary(Simulation *simulation)
{
	this->simulation = simulation;
}


ResultSummary::~ResultSummary()
{
}

double ResultSummary::getChanceForKnockout(int teamID, int bofRound)
{
	int totalLosses = 0;

	// go through all different clusters (all types of matches)
	for (auto &cluster : simulation->clusterMatchResultStatisticsLists)
	{
		auto &results = cluster.second;
		if (results.bofRound != bofRound) continue;
		
		// sum up losses of team over all matches in the specified best-of-X round
		totalLosses += simulation->clusterTeamResults[cluster.first][teamID].getLosses();
	}

	return (double)totalLosses / (double)simulation->numberOfRuns;
}

int ResultSummary::getParticipationCount(int teamID, int bofRound)
{
	int totalParticipations = 0;

	// go through all different clusters (all types of matches)
	for (auto &cluster : simulation->clusterMatchResultStatisticsLists)
	{
		auto &results = cluster.second;
		if (results.bofRound != bofRound) continue;

		// sum up losses of team over all matches in the specified best-of-X round
		totalParticipations += simulation->clusterTeamResults[cluster.first][teamID].getMatchCount();
	}

	return totalParticipations;
}

void ResultSummary::printResults()
{
	struct Results
	{
		int teamID;

		double knockoutGroup;
		double knockoutLast16;
		double knockoutQuarters;
		double knockoutSemi;
		double runnerUp;
		double winner;

		Results() : 
			teamID(-1),
			knockoutGroup(-1), 
			knockoutLast16(-1), 
			knockoutQuarters(-1), 
			knockoutSemi(-1), 
			runnerUp(-1), 
			winner(-1)
		{

		}

	};
	
	std::vector<Results> resultsList;
	for (auto &team : simulation->teams)
	{
		Results results;
		results.teamID = team.id;

		results.winner = (double)simulation->clusterTeamResults["all"][team.id].getPlaceCount(1) / (double)simulation->numberOfRuns;
		results.runnerUp = (double)simulation->clusterTeamResults["all"][team.id].getPlaceCount(2) / (double)simulation->numberOfRuns;

		results.knockoutSemi = getChanceForKnockout(team.id, 2);
		results.knockoutQuarters = getChanceForKnockout(team.id, 4);
		results.knockoutLast16 = getChanceForKnockout(team.id, 8);

		// the chance of being  knocked out in the group phase is the chance of not participating in the round-of-16
		results.knockoutGroup = 1.0 - ((double)getParticipationCount(team.id, 8) / (double)simulation->numberOfRuns);

		resultsList.push_back(results);
	}

	// Finally print results, for that we will need a map with the team names as required by the contest...
	// Note that the simulation usually does not care about the team names.
	// This array reflects the team IDs that are found in the input data.
	std::vector<std::string> teamNames = {"empty",
		"Brazil", "Croatia", "Mexico", "Cameroon", 
		"Spain", "Netherlands", "Chile", "Australia", 
		"Colombia", "Greece", "Côte dIvoire", "Japan", 
		"Uruguay", "Costa Rica", "England", "Italy", 
		"Switzerland", "Ecuador", "France", "Honduras", 
		"Argentina", "Bosnia-Herzegovina", "Iran", "Nigeria", 
		"Germany", "Portugal", "Ghana", "United States", 
		"Belgium", "Algeria", "Russia", "Korea Republic"
	};
	for (auto &result : resultsList)
	{
		std::cout
			<< teamNames[result.teamID] << "\t"
			<< result.knockoutGroup << "\t"
			<< result.knockoutLast16 << "\t"
			<< result.knockoutQuarters << "\t"
			<< result.knockoutSemi << "\t"
			<< result.runnerUp << "\t"
			<< result.winner << "\t"
			<< "\n";
	}
}

} // namespace sim