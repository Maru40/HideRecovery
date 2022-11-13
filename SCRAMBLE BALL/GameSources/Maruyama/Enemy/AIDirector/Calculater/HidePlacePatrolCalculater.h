
/*!
@file HidePlacePatorl.h
@brief HidePlacePatorl�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		namespace Calculater {

			class HidePlacePatorl {

			public:
				//�^�[�Q�b�g�̕]���l���v�Z���ĕԂ��B
				static float CalculateEvalutionValue(
					const std::shared_ptr<GameObject>& selfObject,
					const std::shared_ptr<GameObject>& target
				);

			};
		}
	}
}