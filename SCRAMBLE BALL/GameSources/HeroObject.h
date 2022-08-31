/*!
@file HeroObject.h
@brief HeroObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "EnemyObjectBase.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �q�[���[�I�u�W�F�N�g
		//--------------------------------------------------------------------------------------
		class HeroObject : public EnemyObjectBase
		{
		public:
			//using DrawComp = PNTBoneModelDraw;
			using DrawComp = PNTBoneModelDraw;

		private:

		public:
			HeroObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;

		private:
			void SettingModel();
		};
	}
}