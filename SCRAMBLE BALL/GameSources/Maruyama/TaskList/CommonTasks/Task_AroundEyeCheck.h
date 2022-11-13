
/*!
@file AroundEyeCheck.h
@brief AroundEyeCheck�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskNodeBase;

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// ���͂��m�F����p�����[�^
		//--------------------------------------------------------------------------------------
		struct AroundEyeCheck_Parametor
		{
			float speed;         //�ڐ��̈ړ��X�s�[�h
			float aroundRadian;  //���͂��m�F����p�x

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			AroundEyeCheck_Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="speed">�ڐ��̈ړ��X�s�[�h</param>
			/// <param name="aroundDegree">���ӂ��m�F����p�x</param>
			AroundEyeCheck_Parametor(const float& speed, const float& aroundDegree);
		};

		//--------------------------------------------------------------------------------------
		/// ���͂��m�F����^�X�N
		//--------------------------------------------------------------------------------------
		class AroundEyeCheck : public TaskNodeBase<GameObject>
		{
		public:
			using Parametor = AroundEyeCheck_Parametor;

		private:
			std::weak_ptr<Parametor> m_paramPtr; //�p�����[�^
			float m_defaultSpeed = 0.0f;       //�����X�s�[�h

			uint16_t m_index = 0;              //��������z��̃C���f�b�N�X
			std::vector<Vec3> m_directs;       //�m�F��������̔z��

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="owner">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="parametor">�p�����[�^</param>
			AroundEyeCheck(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// �������������Q�̃Z�b�e�B���O
			/// </summary>
			void SettingDirects();

			/// <summary>
			/// ���Ɍ������������̃Z�b�g
			/// </summary>
			void NextDirect();

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �I�����f
			/// </summary>
			/// <returns>�X�V���I�������Ȃ�true</returns>
			bool IsEnd();
		};

	}

}

//endbasecross