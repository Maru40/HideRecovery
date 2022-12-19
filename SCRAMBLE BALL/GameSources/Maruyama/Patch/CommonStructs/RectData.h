
/*!
@file ImpactMap.h
@brief ImpactMap�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�l�p�`�f�[�^(��������͈͂ɗ��p)
		//--------------------------------------------------------------------------------------
		struct Rect
		{
			Vec3 startPosition;   //�J�n�ꏊ
			Vec3 centerPosition;  //���S�ʒu
			float width;          //���̃T�C�Y
			float depth;          //���s�̃T�C�Y

			Rect();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="centerPosition">���S�ʒu</param>
			/// <param name="width">���̒���</param>
			/// <param name="depth">���s�̒���</param>
			Rect(const Vec3& centerPosition, const float& width, const float& depth);

			/// <summary>
			/// �J�n�ʒu���擾
			/// </summary>
			/// <returns>�J�n�ʒu</returns>
			Vec3 CalculateStartPosition() const;

			/// <summary>
			/// �l�p�`���ɂ��邩�ǂ���
			/// </summary>
			/// <returns>�l�p�`���Ȃ�true</returns>
			bool IsInRect(const Vec3& position) const;
		};

	}
}