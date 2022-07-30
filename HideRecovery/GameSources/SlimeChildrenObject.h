/*!
@file SlimeChildrenObject.h
@brief SlimeChildrenObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	namespace Metaball {
		class ChildrenObject;
	}

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �X���C���̎q���I�u�W�F�N�g
		//--------------------------------------------------------------------------------------
		class SlimeChildrenObject : public basecross::Metaball::ChildrenObject
		{
		public:
			SlimeChildrenObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;
		};
	}
}