
#pragma once
/*!
@file StartCamera.h
@brief StartCamera�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class GameStageBase;

	//--------------------------------------------------------------------------------------
	///	�J�����̈ړ��Ǘ��p�����[�^
	//--------------------------------------------------------------------------------------
	struct CameraMoveParametor {
		Vec3 startPosition;   //�J�n�ʒu
		Vec3 endPosition;     //�I���ʒu
		float speed;          //�ړ��X�s�[�h

		/// <summary>
		/// �R�X�g���N�^
		/// </summary>
		CameraMoveParametor();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="startPosition">�J�n�ʒu</param>
		/// <param name="speed">�ړ��X�s�[�h</param>
		CameraMoveParametor(
			const Vec3& startPosition,
			const float& speed = 1.0f
		);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="startPosition">�J�n�ʒu</param>
		/// <param name="endPosition">�I���ʒu</param>
		/// <param name="speed">�ړ��X�s�[�h</param>
		CameraMoveParametor(
			const Vec3& startPosition,
			const Vec3& endPosition,
			const float& speed = 1.0f
		);
	};

	//--------------------------------------------------------------------------------------
	///	�J�n���K�p�̃J����
	//--------------------------------------------------------------------------------------
	class StartCamera : public Camera
	{
	public:

		/// <summary>
		/// �J�n�J�������o�p�̃p�����[�^
		/// </summary>
		struct ShowParametor
		{
			CameraMoveParametor eye; //�J�����ʒu�̈ړ��p�����[�^
			CameraMoveParametor at;  //�J�����̎��_�ړ��p�����[�^
			float lookTime;          //�ǂ̈ʂ̎��Ԃ����邩

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			ShowParametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="eye">�J�����ʒu�̈ړ��p�����[�^</param>
			/// <param name="at">�J�����̎��_�ړ��p�����[�^</param>
			/// <param name="lookTime">�ǂ̂��炢�̎��Ԃ����邩</param>
			ShowParametor(const CameraMoveParametor& eye,
				const CameraMoveParametor& at,
				const float lookTime = 3.0f
			);
		};

	private:
		std::vector<ShowParametor> m_params; //���o�ړ��p�̃p�����[�^�Q
		int m_index;                         //���݂̃C���f�b�N�X

		float m_timeElapsed;                 //�v������

	public:

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="params">�J�n�J�������o�̃p�����[�^</param>
		StartCamera(const vector<ShowParametor>& params) :
			Camera(),m_params(params),m_index(0),m_timeElapsed(0.0f)
		{}

		void OnCreate() override;
		void OnUpdate() override;

	private:

		/// <summary>
		/// �Q�[���X�e�[�W�̎擾
		/// </summary>
		/// <returns>�Q�[���X�e�[�W</returns>
		std::shared_ptr<GameStageBase> GetGameStage();

		/// <summary>
		/// �J�����ʒu�ړ�
		/// </summary>
		void EyeMove();

		/// <summary>
		/// ���_�ړ�
		/// </summary>
		void AtMove();

	public:

		/// <summary>
		/// ���o�X�L�b�v
		/// </summary>
		void Skip();

		/// <summary>
		/// �J�����̕ύX
		/// </summary>
		void ChangeCamera();

		/// <summary>
		/// ���o���I�����Ă��邩���f
		/// </summary>
		/// <returns>�I�����o</returns>
		bool IsEnd() {  return m_params.size() <= m_index; }

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �J�n���o�̃p�����[�^�Q�̃Z�b�g
		/// </summary>
		/// <param name="params">�p�����[�^�Q</param>
		void SetShowParametor(const std::vector<ShowParametor>& params) noexcept { m_params = params; }
	};

}

//endbasecross