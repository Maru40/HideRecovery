
/*!
@file CombatCalculater.h
@brief CombatCalculater�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		namespace Calculater {

			//--------------------------------------------------------------------------------------
			/// �o�g�����̌v�Z�n
			//--------------------------------------------------------------------------------------
			class Combat 
			{
			public:
				/// <summary>
				/// ���̃^�[�Q�b�g��_���]���l���v�Z���ĕԂ��B
				/// </summary>
				/// <param name="selfObject">�������g�̃I�u�W�F�N�g</param>
				/// <param name="target">�_���^�[�Q�b�g</param>
				/// <returns>�^�[�Q�b�g��_���]���l</returns>
				static float CalculateEvalutionValue(
					const std::shared_ptr<GameObject>& selfObject,
					const std::shared_ptr<GameObject>& target
				);

				/// <summary>
				/// ���̃^�[�Q�b�g��_���]���l���v�Z���ĕԂ��B
				/// </summary>
				/// <param name="selfTransform">�������g�̃g�����X�t�H�[��</param>
				/// <param name="target">�_���^�[�Q�b�g</param>
				/// <returns>�^�[�Q�b�g��_���]���l</returns>
				static float CalculateEvalutionValue(
					const std::shared_ptr<Transform>& selfTransform,
					const std::shared_ptr<GameObject>& target
				);

			};

		}

	}
}