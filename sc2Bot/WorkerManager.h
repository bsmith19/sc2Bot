#ifndef __WORKERMANAGER_H__
#define __WORKERMANAGER_H__

namespace sc2
{
	class ObsertaionInterface;
	class ActionInterface;
}

class CWorkerManager
{
public:
	CWorkerManager();
	virtual ~CWorkerManager();

	void DoStep();
};

#endif
