#ifndef __WORKERMANAGER_H__
#define __WORKERMANAGER_H__

#define _SCL_SECURE_NO_WARNINGS

#include <list>
#include <sc2api\sc2_api.h>

class CWorkerManager
{
public:
	CWorkerManager(const sc2::ObservationInterface* obsInt, sc2::ActionInterface* actInt, const sc2::Race race);
	virtual ~CWorkerManager();

	void DoStep();
	void WorkerCreated(const sc2::Unit* newWorker);
	void WorkerBuild();
	void BuildStructure(sc2::AbilityID id);
private:
	const sc2::ObservationInterface* mObservation;
	sc2::ActionInterface* mAction;
	const sc2::Race mRace;

	std::list<const sc2::Unit*> mWorkerList;
};
#endif
