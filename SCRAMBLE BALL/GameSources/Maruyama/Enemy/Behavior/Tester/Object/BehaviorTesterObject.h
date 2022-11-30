/*!
@file BehaviorTesterObject.h
@brief BehaviorTesterObject�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			///	�O���錾
			//--------------------------------------------------------------------------------------
			template<class EnumType>
			class BehaviorTree;

			//--------------------------------------------------------------------------------------
			///	�e�X�g�p�̃r�w�C�r�A�I�u�W�F�N�g
			//--------------------------------------------------------------------------------------
			class BehaviorTesterObject : public GameObject
			{
				

			public:
				BehaviorTesterObject(const std::shared_ptr<Stage>& stage);

				void OnCreate() override;
			};

		}
	}
}