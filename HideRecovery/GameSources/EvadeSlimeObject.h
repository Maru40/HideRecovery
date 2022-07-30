/*!
@file EvadeSlimeObject.h
@brief EvadeSlimeObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy {

		namespace EvadeSlime {

			//--------------------------------------------------------------------------------------
			/// ������X���C���I�u�W�F�N�g
			//--------------------------------------------------------------------------------------
			class EvadeSlimeObject : public SlimeObjectBase
			{
			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="stage">���̃N���X����������X�e�[�W</param>
				EvadeSlimeObject(const std::shared_ptr<Stage>& stage);

				void OnCreate() override;

			private:

				void CreateModel() override;
			};

		}
	}
}