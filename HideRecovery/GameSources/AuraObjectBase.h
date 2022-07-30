/*!
@file AuraObjectBase.h
@brief AuraObjectBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Effect {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		struct Aura_Parametor;

		//--------------------------------------------------------------------------------------
		/// �I�[���I�u�W�F�N�g�̊��N���X
		//--------------------------------------------------------------------------------------
		class AuraObjectBase : public GameObject
		{
			std::unique_ptr<Aura_Parametor> m_auraParamPtr;  //�I�[���p�����[�^

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="stage">���̃N���X����������X�e�[�W</param>
			/// <param name="parametor">�I�[���p�����[�^</param>
			AuraObjectBase(const std::shared_ptr<Stage>& stage, const Aura_Parametor& parametor);

			void OnCreate() override;
		};

	}
}