#define _SCL_SECURE_NO_WARNINGS

#include <sc2api\sc2_api.h>
#include <iostream>
#include "Bot.h"


int main(int argc, char* argv[])
{
	sc2::Coordinator coord;
	coord.LoadSettings(argc, argv);
	coord.SetRealtime(true);
	
	CBot* b = new CBot();
	coord.SetParticipants({ sc2::CreateParticipant(sc2::Race::Zerg, b), sc2::CreateComputer(sc2::Race::Zerg) });

	coord.LaunchStarcraft();
	std::string m = sc2::kMapBelShirVestigeLE;
	coord.StartGame(m);

	while (coord.Update())
	{
	}

	return 0;
}