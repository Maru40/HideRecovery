#include "MatchStageCoreObject.h"
#include "OnlineMatching.h"
#include "MatchStageTransitioner.h"
#include "Watanabe/UI/Numbers.h"
#include "Watanabe/StageObject/PlayerSpawnPointObject.h"
#include "Watanabe/Component/MatchingSyncPlayerObject.h"

namespace basecross
{
namespace StageObject
{
	MatchStageCoreObject::MatchStageCoreObject(const std::shared_ptr<Stage>& stage) :
		GameObject(stage)
	{

	}

	void MatchStageCoreObject::OnCreate()
	{
		auto onlineMatching = AddComponent<Online::OnlineMatching>();
		auto matchStageTransitioner = AddComponent<MatchStageTransitioner>();

		// スポーンオブジェクトを取得
		vector<shared_ptr<PlayerSpawnPointObject>> spawnPointObjects;
		for (auto gameObject : GetStage()->GetGameObjectVec()) {
			if (auto spawnObject = dynamic_pointer_cast<PlayerSpawnPointObject>(gameObject)) {
				spawnPointObjects.push_back(spawnObject);
			}
		}
		// IDでソート
		sort(spawnPointObjects.begin(), spawnPointObjects.end(),
			[](const shared_ptr<PlayerSpawnPointObject>& a, const shared_ptr<PlayerSpawnPointObject>& b) {
				return a->GetID() < b->GetID();
			}
		);

		AddComponent<MatchingSyncPlayerObject>(spawnPointObjects);
	}
}
}