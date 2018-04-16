#include "Bot.h"
#include "WorkerManager.h"

CBot::CBot()
{
	overloadContstructing = false;
	mWorkerManager = new CWorkerManager(this->Observation(), this->Actions(), sc2::Race::Zerg);
}

void CBot::OnStep()
{
	if (!overloadContstructing && (this->Observation()->GetFoodUsed() >= (this->Observation()->GetFoodCap() - 1)))
	{
		if (this->Observation()->GetMinerals() >= 100)
		{
			for (auto u : this->Observation()->GetUnits(sc2::Unit::Alliance::Self))
			{
				if (u.unit_type.ToType() == sc2::UNIT_TYPEID::ZERG_LARVA)
				{
					this->Actions()->UnitCommand(u, sc2::ABILITY_ID::TRAIN_OVERLORD);
					overloadContstructing = true;
				}
			}
		}
	}
	else if (this->Observation()->GetMinerals() > 50)
	{
		sc2::Filter f = [](sc2::Unit u) { return u.unit_type.ToType() == sc2::UNIT_TYPEID::ZERG_LARVA; };
		for (auto u : this->Observation()->GetUnits(sc2::Unit::Alliance::Self, f))
		{
			if (u.unit_type.ToType() == sc2::UNIT_TYPEID::ZERG_LARVA)
			{
				this->Actions()->UnitCommand(u, sc2::ABILITY_ID::TRAIN_DRONE);
			}
		}
	}
}

void CBot::OnUnitCreated(const sc2::Unit& unit)
{
	if (unit.unit_type.ToType() == sc2::UNIT_TYPEID::ZERG_OVERLORD)
	{
		overloadContstructing = false;
	}
	else if (unit.unit_type.ToType() == sc2::UNIT_TYPEID::ZERG_DRONE)
	{
		mWorkerManager->WorkerCreated(&unit);
	}
}

void CBot::OnUnitIdle(const sc2::Unit& unit)
{
	switch (unit.unit_type.ToType())
	{
	case sc2::UNIT_TYPEID::ZERG_HATCHERY:
	{
		this->Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_DRONE);
		break;
	}
	}
}