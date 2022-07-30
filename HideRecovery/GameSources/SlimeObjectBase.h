/*!
@file SlimeObjectBase.h
@brief SlimeObjectBase�Ȃ�
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
		class EnemyObjectBase;

		//--------------------------------------------------------------------------------------
		/// �X���C���I�u�W�F�N�g���N���X
		//--------------------------------------------------------------------------------------
		class SlimeObjectBase : public EnemyObjectBase
		{
		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="stage">���̃N���X�����L����X�e�[�W</param>
			SlimeObjectBase(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;

		protected:

			/// <summary>
			/// ���f���̐���
			/// </summary>
			virtual void CreateModel();

			/// <summary>
			/// ���^�{�[���Ǐ]�Z�b�e�B���O
			/// </summary>
			virtual void SettingSeek();

			/// <summary>
			/// ScalePop�Z�b�e�B���O
			/// </summary>
			virtual void SettingScalePop();

			/// <summary>
			/// �ǉ���Z�b�e�B���O
			/// </summary>
			virtual void SettingWallEvasion();

			/// <summary>
			/// �e���}�b�v�̃Z�b�e�B���O
			/// </summary>
			virtual void SettingSoleImpactMap();
		};
	}
}

//endbasecross