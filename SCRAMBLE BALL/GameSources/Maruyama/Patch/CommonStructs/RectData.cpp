
/*!
@file ImpactMap.cpp
@brief ImpactMap�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "RectData.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�l�p�`�f�[�^(��������͈͂ɗ��p)
		//--------------------------------------------------------------------------------------

		Rect::Rect()
			: Rect(Vec3(0.0f), 0.0f, 0.0f)
		{}

		Rect::Rect(const Vec3& centerPosition, const float& width, const float& depth)
			: centerPosition(centerPosition), width(width), depth(depth)
		{}

		Vec3 Rect::CalculateStartPosition() const {
			auto position = centerPosition;
			auto scale = Vec3(width, 0.0f, depth);
			auto halfScale = scale * 0.5f;
			float x = position.x - halfScale.x;
			float y = position.y;
			float z = position.z - halfScale.z;
			Vec3 startPosition(x, y, z);

			return startPosition;
		}

		bool Rect::IsInRect(const Vec3& position) const {
			auto rectStartPosition = CalculateStartPosition();

			if (position.x >= rectStartPosition.x &&
				position.x <= rectStartPosition.x + this->width &&
				position.z >= rectStartPosition.z &&
				position.z <= rectStartPosition.z + this->depth
				) {
				return true;
			}

			return false;
		}

	}
}