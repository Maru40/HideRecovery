/*!
@file DamageEffect.h
@brief DamageEffect�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class BoardObject;

	namespace Builder {
		struct VertexPCTParametor;
	}

	namespace Effect {

		//--------------------------------------------------------------------------------------
		/// �_���[�W�G�t�F�N�g
		//--------------------------------------------------------------------------------------
		class Damage : public Effect::Base
		{
		private:

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			Damage(const std::shared_ptr<GameObject>& objPtr);

			virtual void OnCreate() override;

		private:
			/// <summary>
			/// �����̔|���S���𐶐����鐶���f�[�^�̎擾
			/// </summary>
			/// <returns>�����̔|���S���̐����f�[�^</returns>
			ParticleData CreateLineVData();

			/// <summary>
			/// �_�|���S���̐����f�[�^�̎擾
			/// </summary>
			/// <returns>�_�|���S���̐����f�[�^</returns>
			ParticleData CretaePointData();

		};

	}
}