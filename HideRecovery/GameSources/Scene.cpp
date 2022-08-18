/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

#include "MainStage.h"
#include "LoadStage.h"
//#include "ResultStage.h"
//#include "TitleStage.h"
//#include "StageSelect.h"
#include "Watanabe/TestStage/WatanabeStage.h"
class LoadStage;

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------

	const WORD ResetCommand = XINPUT_GAMEPAD_RIGHT_SHOULDER + XINPUT_GAMEPAD_LEFT_SHOULDER + XINPUT_GAMEPAD_START + XINPUT_GAMEPAD_BACK;

	Scene::~Scene() { }

	void Scene::OnCreate() {
		try {
			auto& app = App::GetApp();
			auto path = app->GetDataDirWString();
			// テクスチャの読み込み
			auto texPath = path + L"Textures/";

			//クリアする色を設定
			Col4 Col;
			Col.set(31.0f / 255.0f, 30.0f / 255.0f, 71.0f / 255.0f, 255.0f / 255.0f);
			SetClearColor(Col);
			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToLoadStage");
		}
		catch (...) {
			throw;
		}
	}

	void Scene::OnUpdate() {
		SceneBase::OnUpdate();

		//リセット管理
		auto& app = App::GetApp();
		auto device = app->GetInputDevice();
		const auto& pad = device.GetControlerVec()[0];

		if (pad.wButtons == ResetCommand)
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
		}
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//最初のアクティブステージの設定
			//ResetActiveStage<GameStage>();
		}
		else if (event->m_MsgStr == L"ToLoadStage") {
			ResetActiveStage<LoadStage>(L"ToWatanabeStage");
		}
		else if (event->m_MsgStr == L"ToMainStage") {
			ResetActiveStage<MainStage>();
		}
		else if (event->m_MsgStr == L"ToWatanabeStage") {
			ResetActiveStage<WatanabeStage>();
		}
		//else if (event->m_MsgStr == L"ToResultStage") {
		//	ResetActiveStage<ResultStage>();
		//}
		//else if (event->m_MsgStr == L"ToTitleStage") {
		//	ResetActiveStage<TitleStage>();
		//}
		//else if (event->m_MsgStr == L"ToStageSelect") {
		//	ResetActiveStage<StageSelect>();
		//}
	}
}
//end basecross