/*!
@file BigSlimeObject.h
@brief BigSlimeObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	namespace Enemy {
		namespace BigSlime{

			//--------------------------------------------------------------------------------------
			/// ����X���C���I�u�W�F�N�g
			//--------------------------------------------------------------------------------------
			class BigSlimeObject : public SlimeObjectBase
			{
			public:
				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="stage">���̃N���X�����L����X�e�[�W</param>
				BigSlimeObject(const std::shared_ptr<Stage>& stage);

				void OnCreate() override;

			private:
				virtual void CreateModel() override;
				virtual void SettingScalePop() override;

			};
		
		}
	}
}