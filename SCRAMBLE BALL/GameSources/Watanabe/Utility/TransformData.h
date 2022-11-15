#pragma once
#include "stdafx.h"

namespace basecross {
	/**
	 * @brief 位置、大きさ、回転の情報を持つ構造体
	 */
	struct TransformData {
		Vec3 Position;
		Vec3 Scale;
		Vec3 Rotation;
		TransformData() {
			this->Position = Vec3(0.0f);
			this->Scale = Vec3(1.0f);
			this->Rotation = Vec3(0.0f);
		}
		TransformData(const Vec3& position) {
			this->Position = position;
			this->Scale = Vec3(1.0f);
			this->Rotation = Vec3(0.0f);
		}
		TransformData(const Vec3& position, const Vec3& scale) {
			this->Position = position;
			this->Scale = scale;
			this->Rotation = Vec3(0.0f);
		}
		TransformData(const Vec3& position, const Vec3& scale, const Vec3& rotation) {
			this->Position = position;
			this->Scale = scale;
			this->Rotation = rotation;
		}
		TransformData operator +(const TransformData& data) {
			this->Position += data.Position;
			this->Scale += data.Scale;
			this->Rotation += data.Rotation;
			return *this;
		}
	};

	/**
	 * @brief UI用の位置、大きさ、回転の情報を持つ構造体
	 */
	struct RectTransformData {
		Vec2 Position;
		Vec2 Scale;
		float Rotation;
		RectTransformData() {
			this->Position = Vec2(0.0f);
			this->Scale = Vec2(1.0f);
			this->Rotation = 0.0f;
		}
		RectTransformData(const Vec2& position) {
			this->Position = position;
			this->Scale = Vec2(1.0f);
			this->Rotation = 0.0f;
		}
		RectTransformData(const Vec2& position, const Vec2& scale) {
			this->Position = position;
			this->Scale = scale;
			this->Rotation = 0.0f;
		}
		RectTransformData(const Vec2& position, const Vec2& scale, const float& rotation) {
			this->Position = position;
			this->Scale = scale;
			this->Rotation = rotation;
		}

		RectTransformData(const RectTransformData& data) {
			RectTransformData(data.Position, data.Scale, data.Rotation);
		}

		RectTransformData operator +(const RectTransformData& data) {
			this->Position += data.Position;
			this->Scale += data.Scale;
			this->Rotation += data.Rotation;
			return *this;
		}
	};
}
