/*!
@file Pause.h
@brief ポーズ画面
*/

#pragma once
#include "stdafx.h"
#include "Cursor.h"

namespace basecross {
	//------------------------------------------------------------------------------------------------
	/// ポーズ機能及びゲームオーバークラス
	//------------------------------------------------------------------------------------------------
	class Pause : public GameObject
	{
		//ポーズ画面に配置されるUI				
		const std::vector<CursorUIData> pauseDatas = {
			CursorUIData(UiData(L"Play",	  Vec3(0.0f,  200.0f, 0.0f), Vec3(256.0f, 64.0f, 0.0f), Vec2(0.65f, 0.75f)), [&]() {SetPause(); }, true),
			CursorUIData(UiData(L"Retry",	  Vec3(0.0f,  100.0f, 0.0f), Vec3(256.0f, 64.0f, 0.0f), Vec2(0.8f,  0.8f)),  [&]() {PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMainStage"); }, true),
			CursorUIData(UiData(L"Option",	  Vec3(0.0f,   00.0f, 0.0f), Vec3(256.0f, 64.0f, 0.0f), Vec2(0.8f,  0.8f))),
			CursorUIData(UiData(L"SelectMenu",Vec3(45.0f,-100.0f, 0.0f), Vec3(512.0f, 64.0f, 0.0f), Vec2(1.0f,  1.0f)),  [&]() {PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToStageSelect"); }, true),
			CursorUIData(UiData(L"Title",	  Vec3(0.0f, -200.0f, 0.0f), Vec3(256.0f, 64.0f, 0.0f), Vec2(0.8f,  0.8f)),  [&]() {PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage"); }, true)
		};

		//ゲームオーバー画面に配置されるUI
		const std::vector<CursorUIData> gameOverDatas = {
			CursorUIData(UiData(L"Retry",	  Vec3(0.0f,   25.0f, 0.0f), Vec3(256.0f,    64.0f, 0.0f), Vec2(0.8f, 0.8f)), [&]() {PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMaruTestStage"); }, true),
			CursorUIData(UiData(L"SelectMenu",Vec3(45.0f, -75.0f, 0.0f), Vec3(512.0f,    64.0f, 0.0f), Vec2(1.0f, 1.0f)), [&]() {PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToStageSelect"); }, true),
			CursorUIData(UiData(L"Title",	  Vec3(0.0f, -175.0f, 0.0f), Vec3(256.0f,   64.0f, 0.0f), Vec2(0.8f, 0.8f)), [&]() {PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage"); }, true),
			CursorUIData(UiData(L"GameOver",  Vec3(0.0f,  175.0f, 0.0f), Vec3(512.0f,    64.0f, 0.0f), Vec2(1.0f, 1.0f))),
			CursorUIData(UiData(L"PauseBack", Vec3(0.0f,    0.0f, 0.0f), Vec3(2048.0f, 2048.0f, 0.0f), Vec2(1.0f, 1.0f))),
		};

		bool m_IsGameOver = false; //! ゲームオーバーになっているか

		std::shared_ptr<Cursor> m_Cursor; //! カーソルオブジェクト

	public:
		//-----------------------------------------------------------------------------------------------
		/// コンストラクタ
		Pause::Pause(const std::shared_ptr<Stage>& stage) :GameObject(stage) {}
		//-----------------------------------------------------------------------------------------------
		void OnCreate() override;
		void OnUpdate() override;
		//-----------------------------------------------------------------------------------------------
		/// UIの配置
		void DeployUi();

		/// ポーズ画面の表示と他オブジェクトの停止
		void SetPause();

		/// ゲームオーバー画面の表示と他オブジェクトの停止
		void GameOver();

		/**
		* @brief ステージに配置されている他のオブジェクトの更新状態の設定
		* @param[in] value 更新状態
		*/
		void Pausing(bool value);

		/// カーソルの操作
		void MoveCursor();
		//-----------------------------------------------------------------------------------------------
	};
}
