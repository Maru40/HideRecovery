#include "OtherPlayerObject.h"
#include "Watanabe/BoardPoly/HPGaugeBP.h"
#include "Watanabe/BoardPoly/PlayerLabelBP.h"
#include "Watanabe/Component/PlayerStatus.h"
#include "Watanabe/Component/SyncObject.h"
#include "Maruyama/Player/Component/Teleport.h"
#include "Maruyama/Utility/Component/ToTargetMove.h"

namespace basecross
{
namespace StageObject
{
	OtherPlayerObject::OtherPlayerObject(const std::shared_ptr<Stage>& stage) :
		PlayerObject(stage)
	{

	}

	void OtherPlayerObject::OnCreate()
	{
		PlayerObject::OnCreate();

		auto playerStatus = GetComponent<PlayerStatus>();

		// 他プレイヤーは板ポリHPゲージをつける
		//（位置の同期はHPGaugeBP内で設定してある）
		GetStage()->AddGameObject<HPGaugeBP>(playerStatus);

		auto teleport = GetComponent<Teleport>();
		auto cameraObject = GetStage()->AddGameObject<GameObject>();
		auto toTargetMove = cameraObject->AddComponent<ToTargetMove>();
		teleport->SetToTargetMove(toTargetMove);
	}

	void OtherPlayerObject::OnlineSetting(int gameNumber, int playerNumber)
	{
		PlayerObject::OnlineSetting(gameNumber, playerNumber);

		auto playerStatus = GetComponent<PlayerStatus>();
		auto playerTeam = playerStatus->GetTeam();

		// (playerNumber % 3) + 1で1～6が 1,2,3 1,2,3になる
		auto label = GetStage()->AddGameObject<PlayerLabelBP>(team::GetTeamTypeString(playerTeam), (gameNumber % 3) + 1);

		auto syncComp = label->AddComponent<SyncObject>();
		syncComp->SetTarget(transform);
		syncComp->SetOffsetPosition(Vec3(0, 1.7f, 0));
	}
}
}