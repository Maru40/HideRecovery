#include "stdafx.h"
#include "MatchStageRoomer.h"
#include "Watanabe/DebugClass/Debug.h"

namespace basecross
{
	MatchStageRoomer::MatchStageRoomer(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{

	}

	void MatchStageRoomer::OnCreate()
	{
		Online::OnlineManager::Connect();
	}

	void MatchStageRoomer::OnConnected()
	{
		Online::OnlineManager::JoinRandomOrCreateRoom(ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(6));
	}

	void MatchStageRoomer::OnCreateRoom()
	{
		Debug::GetInstance()->Log(L"部屋の作成に成功しました");
	}

	void MatchStageRoomer::OnJoinRoom()
	{
		Debug::GetInstance()->Log(L"部屋の参加に成功しました");
	}

	void MatchStageRoomer::OnCreateRoomFailed(int errorCode)
	{
		MessageBox(0, L"オンラインの部屋へのアクセスに失敗しました", L"エラー", 0);
		return;
	}
}