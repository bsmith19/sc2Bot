#include "WorkerManager.h"
//#include <sc2api\sc2_api.h>

CWorkerManager::CWorkerManager(const sc2::ObservationInterface* obsInt, sc2::ActionInterface* actInt, const sc2::Race race)
	: mObservation(obsInt), mAction(actInt), mRace(race)
{
}

CWorkerManager::~CWorkerManager()
{
	mObservation = nullptr;
	mAction = nullptr;
}

void CWorkerManager::DoStep()
{

}

void CWorkerManager::WorkerCreated(const sc2::Unit* newWorker)
{
	
	if (newWorker != nullptr)
	{
		mWorkerList.push_back(newWorker);
	}
}

void CWorkerManager::WorkerBuild()
{
	
	sc2::Filter f;
	sc2::AbilityID id;
	switch (mRace)
	{
	case sc2::Race::Protoss:
		f = [](sc2::Unit u) { return u.unit_type.ToType() == sc2::UNIT_TYPEID::PROTOSS_NEXUS; };
		id = sc2::ABILITY_ID::TRAIN_PROBE;
		break;
	case sc2::Race::Terran:
		f = [](sc2::Unit u) { return u.unit_type.ToType() == sc2::UNIT_TYPEID::TERRAN_COMMANDCENTER; };
		id = sc2::ABILITY_ID::TRAIN_SCV;
		break;
	case sc2::Race::Zerg:
		f = [](sc2::Unit u) { return u.unit_type.ToType() == sc2::UNIT_TYPEID::ZERG_LARVA; };
		id = sc2::ABILITY_ID::TRAIN_DRONE;
	}

	sc2::Units builders = mObservation->GetUnits(sc2::Unit::Alliance::Self, f);
	if (!builders.empty())
	{
		mAction->UnitCommand(builders.front(), id);
	}
}

void CWorkerManager::BuildStructure(sc2::AbilityID id)
{
	//mAction->UnitCommand(mWorkerList.front()->tag, id);
}
