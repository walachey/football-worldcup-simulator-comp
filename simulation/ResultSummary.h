#ifndef _RESULTSUMMARY_H
#define _RESULTSUMMARY_H

namespace sim
{
class Simulation;

class ResultSummary
{
public:
	ResultSummary(Simulation *simulation);
	~ResultSummary();

	void printResults();
	double getChanceForKnockout(int teamID, int bofRound);
	int getParticipationCount(int teamID, int bofRound);
private:
	Simulation *simulation;
};


} // namespace sim
#endif