/*!
@file Pause.h
@brief �|�[�Y���
*/

#pragma once
#include "stdafx.h"
#include "Cursor.h"

namespace basecross {
	//------------------------------------------------------------------------------------------------
	/// �|�[�Y�@�\�y�уQ�[���I�[�o�[�N���X
	//------------------------------------------------------------------------------------------------
	class Pause : public GameObject
	{
		//�|�[�Y��ʂɔz�u�����UI				
		const std::vector<CursorUIData> pauseDatas = {
			CursorUIData(UiData(L"Play",	  Vec3(0.0f,  200.0f, 0.0f), Vec3(256.0f, 64.0f, 0.0f), Vec2(0.65f, 0.75f)), [&]() {SetPause(); }, true),
			CursorUIData(UiData(L"Retry",	  Vec3(0.0f,  100.0f, 0.0f), Vec3(256.0f, 64.0f, 0.0f), Vec2(0.8f,  0.8f)),  [&]() {PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMainStage"); }, true),
			CursorUIData(UiData(L"Option",	  Vec3(0.0f,   00.0f, 0.0f), Vec3(256.0f, 64.0f, 0.0f), Vec2(0.8f,  0.8f))),
			CursorUIData(UiData(L"SelectMenu",Vec3(45.0f,-100.0f, 0.0f), Vec3(512.0f, 64.0f, 0.0f), Vec2(1.0f,  1.0f)),  [&]() {PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToStageSelect"); }, true),
			CursorUIData(UiData(L"Title",	  Vec3(0.0f, -200.0f, 0.0f), Vec3(256.0f, 64.0f, 0.0f), Vec2(0.8f,  0.8f)),  [&]() {PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage"); }, true)
		};

		//�Q�[���I�[�o�[��ʂɔz�u�����UI
		const std::vector<CursorUIData> gameOverDatas = {
			CursorUIData(UiData(L"Retry",	  Vec3(0.0f,   25.0f, 0.0f), Vec3(256.0f,    64.0f, 0.0f), Vec2(0.8f, 0.8f)), [&]() {PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMaruTestStage"); }, true),
			CursorUIData(UiData(L"SelectMenu",Vec3(45.0f, -75.0f, 0.0f), Vec3(512.0f,    64.0f, 0.0f), Vec2(1.0f, 1.0f)), [&]() {PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToStageSelect"); }, true),
			CursorUIData(UiData(L"Title",	  Vec3(0.0f, -175.0f, 0.0f), Vec3(256.0f,   64.0f, 0.0f), Vec2(0.8f, 0.8f)), [&]() {PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage"); }, true),
			CursorUIData(UiData(L"GameOver",  Vec3(0.0f,  175.0f, 0.0f), Vec3(512.0f,    64.0f, 0.0f), Vec2(1.0f, 1.0f))),
			CursorUIData(UiData(L"PauseBack", Vec3(0.0f,    0.0f, 0.0f), Vec3(2048.0f, 2048.0f, 0.0f), Vec2(1.0f, 1.0f))),
		};

		bool m_IsGameOver = false; //! �Q�[���I�[�o�[�ɂȂ��Ă��邩

		std::shared_ptr<Cursor> m_Cursor; //! �J�[�\���I�u�W�F�N�g

	public:
		//-----------------------------------------------------------------------------------------------
		/// �R���X�g���N�^
		Pause::Pause(const std::shared_ptr<Stage>& stage) :GameObject(stage) {}
		//-----------------------------------------------------------------------------------------------
		void OnCreate() override;
		void OnUpdate() override;
		//-----------------------------------------------------------------------------------------------
		/// UI�̔z�u
		void DeployUi();

		/// �|�[�Y��ʂ̕\���Ƒ��I�u�W�F�N�g�̒�~
		void SetPause();

		/// �Q�[���I�[�o�[��ʂ̕\���Ƒ��I�u�W�F�N�g�̒�~
		void GameOver();

		/**
		* @brief �X�e�[�W�ɔz�u����Ă��鑼�̃I�u�W�F�N�g�̍X�V��Ԃ̐ݒ�
		* @param[in] value �X�V���
		*/
		void Pausing(bool value);

		/// �J�[�\���̑���
		void MoveCursor();
		//-----------------------------------------------------------------------------------------------
	};
}
