/*!
@file ShootSlime_BulletObject.h
@brief ShootSlime_BulletObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		namespace ShootSlime {

			//--------------------------------------------------------------------------------------
			/// �V���b�g�X���C���̒e�I�u�W�F�N�g
			//--------------------------------------------------------------------------------------
			class BulletObject : public GameObject
			{
			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="stage">���̃N���X����������X�e�[�W</param>
				BulletObject(const std::shared_ptr<Stage>& stage);

				void OnCreate() override;

			private:
				/// <summary>
				/// �`��ݒ�
				/// </summary>
				void SettingDraw();

			};

		}
	}
}