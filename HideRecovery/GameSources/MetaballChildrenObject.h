/*!
@file MetaballChildrenObject.h
@brief MetaballChildrenObject�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Metaball {

		//--------------------------------------------------------------------------------------
		///	���^�{�[���̎q���I�u�W�F�N�g
		//--------------------------------------------------------------------------------------
		class ChildrenObject : public GameObject
		{
		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="stage">���̃N���X����������X�e�[�W</param>
			ChildrenObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;
		};

	}
}

//endbasecross