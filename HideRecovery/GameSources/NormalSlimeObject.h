/*!
@file NormalSlimeObject.h
@brief NormalSlimeObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class SlimeObjectBase;

		//--------------------------------------------------------------------------------------
		/// �ʏ�X���C���I�u�W�F�N�g
		//--------------------------------------------------------------------------------------
		class NormalSlimeObject : public SlimeObjectBase
		{
		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="stage">���̃N���X����������X�e�[�W</param>
			NormalSlimeObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;

		private:
			virtual void CreateModel();

			
		};
	}
}