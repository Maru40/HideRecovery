#include"ActionTimer.h"

namespace itbs
{
	namespace Utility
	{
		void ActionTimer::AddAction(const ActionData& actionData)
		{
			m_actionDatas.push_back(actionData);
		}

		void ActionTimer::AdvanceTime(const float advancedTime)
		{
			auto actionDataItr = m_actionDatas.begin();

			while (actionDataItr != m_actionDatas.end())
			{
				actionDataItr->advancedTime += advancedTime;

				if (actionDataItr->advancedTime >= actionDataItr->invokeTime)
				{
					actionDataItr->action();

					actionDataItr = m_actionDatas.erase(actionDataItr);

					continue;
				}

				actionDataItr++;
			}
		}
	}
}