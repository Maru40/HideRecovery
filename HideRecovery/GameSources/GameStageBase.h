/*!
@file GameStageBase.h
@brief GameStageBase
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class PlayerObject;
	class MainCamera;
	class MainCameraObject;
	class StartCamera;
	class StageObjectBase;
	class SlimeRenderTarget;

	class GameStageBase : public Stage {
	protected:
		//--------------------------------------------------------------------------------------
		/// �悭�g����I�u�W�F�N�g
		//--------------------------------------------------------------------------------------

		ex_weak_ptr<PlayerObject> m_player;
		std::vector<std::shared_ptr<GameObject>> m_floors; // ���ɑΉ�����I�u�W�F�N�g

		//--------------------------------------------------------------------------------------
		/// �J����
		//--------------------------------------------------------------------------------------

		//���C���J����
		//std::shared_ptr<SingleView> m_mainView;                    //�r���[
		//std::shared_ptr<MainCamera> m_mainCamera;                  //�J����
		//std::shared_ptr<MainCameraObject> m_mainCameraObject;      //�I�u�W�F�N�g
		std::shared_ptr<SingleView> m_mainView;						//�r���[
		std::shared_ptr<Camera> m_mainCamera;						//�J����

		//�X�^�[�g�J����
		std::shared_ptr<SingleView> m_startView;					//�r���[
		std::shared_ptr<StartCamera> m_startCamera;					//�J����

		//�����_�[�^�[�Q�b�g
		std::shared_ptr<SlimeRenderTarget> m_slimeRenderTarget;		//�X���C���p�̃����_�[�^�[�Q�b�g

		/// <summary>
		/// ���C���J�����̐���
		/// </summary>
		virtual void CreateMainCamera();

		/// <summary>
		/// �X�^�[�g�J�����̐���
		/// </summary>
		/// <param name="stageName">�X�e�[�W��</param>
		virtual void CreateStartCamera(const wstring& stageName);

		/// <summary>
		/// �}�b�v�̐���
		/// </summary>
		/// <param name="fileName">�}�b�v�t�@�C���̖��O</param>
		virtual void CreateMap(const wstring& fileName);

		/// <summary>
		/// UI�̐���
		/// </summary>
		/// <param name="fileName">UI���C�A�E�g�t�@�C���̖��O</param>
		virtual void CreateUI(const wstring& fileName);

		/// <summary>
		/// �O���̃R���W�����𕡐���������֐�
		/// </summary>
		/// <param name="objects">��������x�[�X�ƂȂ鏰�I�u�W�F�N�g</param>
		template<class T>
		void CreateMapOutCollisions(const std::vector<std::shared_ptr<T>>& objects) {
			for (const auto& object : objects) {
				auto objectTrans = object->GetComponent<Transform>();
				auto position = objectTrans->GetPosition();
				auto scale = objectTrans->GetScale();
				auto halfScale = scale * 0.5f;

				constexpr float Width = 0.5f;
				//���s����
				auto forwardPosition = position + (Vec3::Forward() * halfScale.z);
				CreateMapOutCollision(forwardPosition, Vec3::Forward(), scale.x, Width);

				//��O������
				auto backPosition = position + (-Vec3::Forward() * halfScale.z);
				CreateMapOutCollision(backPosition, -Vec3::Forward(), scale.x, Width);

				//�E��
				auto rightPosition = position + (Vec3::Right() * halfScale.x);
				CreateMapOutCollision(rightPosition, Vec3::Right(), scale.z, Width);

				//����
				auto leftPosition = position + (-Vec3::Right() * halfScale.x);
				CreateMapOutCollision(leftPosition, -Vec3::Right(), scale.z, Width);
			}
		}

		/// <summary>
		/// �O���̃R���W�����𐶐�����
		/// </summary>
		/// <param name="startPosition">�����X�^�[�g�ꏊ</param>
		/// <param name="forward">����</param>
		/// <param name="length">����</param>
		/// <param name="width">����</param>
		/// <param name="height">����</param>
		void CreateMapOutCollision(const Vec3& startPosition, const Vec3& forward, const float& length, const float& width, const float& height = 15.0f);

	public:
		GameStageBase();
		virtual ~GameStageBase() {}

		virtual void OnCreate() override;
		virtual void RenderStage() override;

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �v���C���[�̎擾
		/// </summary>
		/// <returns>�v���C���[</returns>
		std::shared_ptr<PlayerObject> GetPlayer() const;

		/// <summary>
		/// �J�����̎擾
		/// </summary>
		template<class T>
		std::shared_ptr<T> GetCamera() const
		{
			auto view = GetView();
			auto camera = view->GetTargetCamera();
			auto tCam = dynamic_pointer_cast<T>(camera);
			return tCam;
		}

		/// <summary>
		/// ���C���J�����ɐ؂�ւ�
		/// </summary>
		/// <returns>���C���J����</returns>
		std::shared_ptr<Camera> ChangeMainCamera();

		/// <summary>
		/// ���C���J�����I�u�W�F�N�g�̎擾
		/// </summary>
		/// <returns>���C���J�����I�u�W�F�N�g</returns>
		//std::shared_ptr<MainCameraObject> GetMainCameraObject() const;

		/// <summary>
		/// �X�^�[�g�J�����ɐ؂�ւ�
		/// </summary>
		/// <returns>�X�^�[�g�J����</returns>
		std::shared_ptr<StartCamera> ChangeStartCamera();

		/// <summary>
		/// ���ɑΉ�����I�u�W�F�N�g��ǉ�
		/// </summary>
		/// <param name="obj">�I�u�W�F�N�g</param>
		void AddFloorObject(std::shared_ptr<GameObject> obj) {
			m_floors.push_back(obj);
		}
	};
}
//end basecross