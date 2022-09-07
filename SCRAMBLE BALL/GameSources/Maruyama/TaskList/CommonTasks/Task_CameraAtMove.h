/*!
@file Task_CameraAtMove.h
@brief Task_CameraAtMove�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�p���ړ��^�X�N
	//--------------------------------------------------------------------------------------
	class Task_CameraAtMove : public TaskNodeBase<Camera>
	{
	public:
		//--------------------------------------------------------------------------------------
		///	�ړ��^�C�v
		//--------------------------------------------------------------------------------------
		enum class MoveType 
		{
			Lerp,      //���
			Transform, //�ʏ�g�����X�t�H�[��
		};

		//--------------------------------------------------------------------------------------
		///	���Ԍv���^�C�v
		//--------------------------------------------------------------------------------------
		enum class DeltaType
		{
			Normal,    //�ʏ�
			Real       //RealTime
		};

		//--------------------------------------------------------------------------------------
		///	�ړ��p�����[�^
		//--------------------------------------------------------------------------------------
		struct Parametor
		{
			Vec3 startPosition;   //�J�n�ʒu
			Vec3 endPosition;     //�I���ʒu
			float speed;          //�ړ����x
			MoveType moveType;    //�ړ��^�C�v
			DeltaType deltaType;  //���Ԍv���^�C�v

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="startPosition">�J�n�ʒu</param>
			/// <param name="endPosition">�I���ʒu</param>
			/// <param name="speed">�ړ����x</param>
			/// <param name="moveType">�ړ��^�C�v</param>
			/// <param name="deltaType">���Ԍv���^�C�v</param>
			Parametor(const Vec3& startPosition, const Vec3& endPosition, const float& speed, 
				const MoveType moveType, const DeltaType& deltaType = DeltaType::Normal);

			/// <summary>
			/// �I���ʒu�̕������擾
			/// </summary>
			/// <returns>�I���ʒu�̕���</returns>
			Vec3 GetToEndDirection() const {
				return endPosition - startPosition;
			}
		};

	private:
		std::vector<Parametor> m_params;	//�ړ��p�����[�^�z��
		int m_currentIndex = 0;				//���݂̃C���f�b�N�X

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="camera">�J�����̃|�C���^</param>
		Task_CameraAtMove(const std::shared_ptr<Camera> camera);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="camera">�J�����̃|�C���^</param>
		/// <param name="parametor">�ړ��p�����[�^</param>
		Task_CameraAtMove(const std::shared_ptr<Camera> camera, const Parametor& parametor);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="camera">�J�����̃|�C���^</param>
		/// <param name="parametors">�ړ��p�����[�^�̔z��</param>
		Task_CameraAtMove(const std::shared_ptr<Camera> camera, const vector<Parametor>& parametors);

		void OnStart() override;
		bool OnUpdate() override;
		void OnExit() override;

	private:
		/// <summary>
		/// �ړ��X�V����
		/// </summary>
		void MoveUpdate();

		/// <summary>
		/// �⊮�ړ�����
		/// </summary>
		/// <param name="param">�ړ��p�����[�^</param>
		void LerpMove(const Parametor& param);

		/// <summary>
		/// Transform�ړ�����
		/// </summary>
		/// <param name="param">�ړ��p�����[�^</param>
		void TransformMove(const Parametor& param);

		/// <summary>
		/// ���̖ړI�n�Ɉڍs���邩�ǂ���
		/// </summary>
		/// <param name="param">�ړ��p�����[�^</param>
		/// <returns>���̖ړI�n�Ɉڍs����Ȃ�true</returns>
		bool IsNext(const Parametor& param);

		/// <summary>
		/// �v�����Ԃ̎擾
		/// </summary>
		/// <param name="param">�ړ��p�����[�^</param>
		/// <returns>�v������</returns>
		float GetElapsedTime(const Parametor& param);

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �I�����f
		/// </summary>
		/// <returns>�S�Ă̈ړ�������������true</returns>
		bool IsEnd();

		/// <summary>
		/// �ړ��p�����[�^�̒ǉ�
		/// </summary>
		/// <param name="parametor">�ړ��p�����[�^</param>
		void AddParametor(const Parametor& parametor);

		/// <summary>
		/// ���݂̈ړ��p�����[�^�̎擾
		/// </summary>
		/// <returns>���݂̈ړ��p�����[�^</returns>
		Parametor GetNowParametor() const;

		/// <summary>
		/// �ړ��p�����[�^�z��̐ݒ�
		/// </summary>
		/// <param name="parametors">�ړ��p�����[�^�̔z��</param>
		void SetParametors(const vector<Parametor>& parametors);

		/// <summary>
		/// �ړ��p�����[�^�̔z��̎擾
		/// </summary>
		/// <returns>�ړ��p�����[�^�̔z��</returns>
		vector<Parametor> GetParametors() const;
	};

}

//endbasecross