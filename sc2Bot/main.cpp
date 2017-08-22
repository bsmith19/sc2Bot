#define _SCL_SECURE_NO_WARNINGS

#include <sc2api\sc2_api.h>
#include <iostream>
#include "WorkerManager.h"

class Bot : public sc2::Agent
{
public:
	Bot()
	{
		overloadContstructing = false;
		mWorkerManager = new CWorkerManager(this->Observation(), this->Actions());
	}
	//! Called when a game is started after a load. Fast restarting will not call this.
	virtual void OnGameFullStart() {}

	//! Called when a game is started or restarted.
	virtual void OnGameStart() {}

	//! Called when a game has ended.
	virtual void OnGameEnd() {}

	//! This event will only get called when stepping. It will not get called in a real time game.
	//! In a real time game the user will be responsible for calling GetObservation() via the ObservationInterface.
	virtual void OnStep() 
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

	//! Called whenever one of the player's units has been destroyed.
	//!< \param unit The destroyed unit.
	virtual void OnUnitDestroyed(const sc2::Unit& unit) {}

	//! Called when a Unit has been created by the player.
	//!< \param unit The created unit.
	virtual void OnUnitCreated(const sc2::Unit& unit)
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

	//! Called when a unit becomes idle, this will only occur as an event so will only be called when the unit becomes
	//! idle and not a second time. Being idle is defined by having orders in the previous step and not currently having
	//! orders or if it did not exist in the previous step and now does, a unit being created, for instance, will call both
	//! OnUnitCreated and OnUnitIdle if it does not have a rally set.
	//!< \param unit The idle unit.
	virtual void OnUnitIdle(const sc2::Unit& unit)
	{
		switch (unit.unit_type.ToType())
		{
			case sc2::UNIT_TYPEID::ZERG_HATCHERY:
			{
				//this->Actions()->UnitCommand(unit, sc2::ABILITY_ID::TRAIN_DRONE);
				break;
			}
		}
	}

	//! Called when an upgrade is finished, warp gate, ground weapons, baneling speed, etc.
	//!< \param upgrade The completed upgrade.
	virtual void OnUpgradeCompleted(sc2::UpgradeID upgrade) {}

	//! Called when the unit in the previous step had a build progress less than 1.0 but is greater than or equal to 1.0 in the current step.
	//!< \param unit The constructed unit.
	virtual void OnBuildingConstructionComplete(const sc2::Unit& unit) {}

	//! Called when a nydus is placed.
	virtual void OnNydusDetected() {}

	//! Called when a nuclear launch is detected.
	virtual void OnNuclearLaunchDetected() {}

	//! Called when an enemy unit enters vision from out of fog of war.
	//!< \param unit The unit entering vision.
	virtual void OnUnitEnterVision(const sc2::Unit& unit) {}

	//! Called for various errors the library can encounter. See ClientError enum for possible errors.
	virtual void OnError(const std::vector<sc2::ClientError>& client_errors, const std::vector<std::string>& protocol_errors = {}) {}

private:
	bool overloadContstructing;
	CWorkerManager* mWorkerManager;
};

int main(int argc, char* argv[])
{
	sc2::Coordinator coord;
	coord.LoadSettings(argc, argv);
	coord.SetRealtime(true);
	
	Bot* b = new Bot();
	coord.SetParticipants({ sc2::CreateParticipant(sc2::Race::Zerg, b), sc2::CreateComputer(sc2::Race::Zerg) });

	coord.LaunchStarcraft();
	std::string m = sc2::kMapBelShirVestigeLE;
	coord.StartGame(m);

	while (coord.Update())
	{
	}

	return 0;
}