/*!
@file ShootSlimeObject.h
@brief ShootSlimeObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy
	{
		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class SlimeObjectBase;

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// �V���b�g�X���C���I�u�W�F�N�g
			//--------------------------------------------------------------------------------------
			class ShootSlimeObject : public SlimeObjectBase
			{
			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="stage">���̃N���X����������X�e�[�W</param>
				ShootSlimeObject(const std::shared_ptr<Stage>& stage);

				void OnCreate() override;

			protected:
				virtual void CreateModel() override;
			};
		}
	}
}