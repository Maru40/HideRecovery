/*!
@file VillainEnemyObject.h
@brief VillainEnemyObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "EnemyObjectBase.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// ���B�����I�u�W�F�N�g
		//--------------------------------------------------------------------------------------
		class VillainObject : public EnemyObjectBase
		{
		public:
			//using DrawComp = PNTBoneModelDraw;
			using DrawComp = PNTBoneModelDraw;

		private:

		public:
			VillainObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;

		private:
			void SettingModel();
		};
	}
}