
/*!
@file Mathf.cpp
@brief Mathfクラス実体
担当：丸山裕喜
*/


#include "stdafx.h"
#include "Project.h"
#include "Mathf.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"

namespace basecross {

	namespace maru {

		const float Mathf::INFINITY_FLOAT = 9999999.0f;


		Vec3 Mathf::Mat4RotationAxisX(const Vec3& vec, const float rad)
		{
			Mat4x4 rotMat = {
				{1,0,0,0},
				{0,cosf(rad),-sinf(rad) ,0},
				{0,sinf(rad),cosf(rad),0},
				{0,0,0,1}
			};

			return vec * rotMat;
		}

		//Mat4*4用
		Mat4x4 Mathf::Mat4RotationAxisX(const Mat4x4& mat, const float rad)
		{
			Mat4x4 rotMat = {
				{1,0,0,0},
				{0,cosf(rad),-sinf(rad) ,0},
				{0,sinf(rad),cosf(rad),0},
				{0,0,0,1}
			};

			return mat * rotMat;
		}


		Vec3 Mathf::Mat4RotationAxisY(const Vec3& vec, const float rad)
		{
			Mat4x4 rotMat = {
				{cosf(rad),0,sinf(rad),0},
				{0,1,0,0},
				{-sinf(rad),0,cosf(rad),0},
				{0,0,0,1},
			};

			return vec * rotMat;
		}

		Mat4x4 Mathf::Mat4RotationAxisY(const Mat4x4& mat, const float rad)
		{
			Mat4x4 rotMat = {
				{cosf(rad),0,sinf(rad),0},
				{0,1,0,0},
				{-sinf(rad),0,cosf(rad),0},
				{0,0,0,1},
			};

			return mat * rotMat;
		}

		Vec3 Mathf::Mat4RotationAxisZ(const Vec3& vec, const float rad)
		{
			Mat4x4 rotMat = {
				{cosf(rad),-sinf(rad),0,0},
				{sinf(rad),cosf(rad),0,0},
				{0,0,1,0},
				{0,0,0,1},
			};

			return vec * rotMat;
		}

		Mat4x4 Mathf::Mat4RotationAxisZ(const Mat4x4& mat, const float rad)
		{
			Mat4x4 rotMat = {
				{cosf(rad),-sinf(rad),0,0},
				{sinf(rad),cosf(rad),0,0},
				{0,0,1,0},
				{0,0,0,1},
			};

			return mat * rotMat;
		}

		Vec3 Mathf::RotationDegreeVec(const Vec3& vec, const float& deg, const Vec3& axis) {
			float rad = XMConvertToRadians(deg);
			return RotationRadianVec(vec, rad, axis);
		}

		Vec3 Mathf::RotationRadianVec(const Vec3& vec, const float& rad, const Vec3& axis) {
			Quat quat = (Quat)XMQuaternionRotationAxis(axis, rad);
			return quat * vec;
		}

		float Mathf::CalculateAngle(const Vec3& a, const Vec3& b) {
			//aとbの内積を取得
			float newDot = dot(a.GetNormalized(), b.GetNormalized());
			//acosfを利用して角度を求める
			return std::acosf(newDot);
		}

		Vec3 Mathf::CalculateRotationAxis(const Vec3& a, const Vec3& b) {
			return cross(a.GetNormalized(), b.GetNormalized());
		}

		bool Mathf::IsInAngleDegree(const Vec3& a, const Vec3& b, const float degree) {
			return IsInAngleRadian(a, b, XMConvertToRadians(degree));
		}

		bool Mathf::IsInAngleRadian(const Vec3& a, const Vec3& b, const float radian) {
			auto angle = CalculateAngle(a, b);
			return angle < radian;
		}

		Vec3 Mathf::Lerp(const Vec3& startPos, const Vec3& endPos, const float& delta)
		{
			//if (endTime <= elapsedTime)  //基底以上の数値が入っていたら、動かさない。
			//{
			//	return Vec3(0.0f);
			//}

			auto toEnd = endPos - startPos;

			auto returnPos = toEnd / delta;

			return returnPos;
		}

		bool Mathf::IsNaN(const Vec3& vector)
		{
			return false;
			//if (float.IsNaN(vector.x) ||
			//	float.IsNaN(vector.y) ||
			//	float.IsNaN(vector.z))
			//{
			//	return true;
			//}

			//return false;
		}

		Vec3 Mathf::ClampVec(const Vec3& value, const Vec3& min, const Vec3& max) {
			auto result = Vec3(0.0f);

			result.x = Clamp(value.x, min.x, max.x);
			result.y = Clamp(value.y, min.y, max.y);
			result.z = Clamp(value.z, min.z, max.z);

			return result;
		}

		Vec3 Mathf::AbsVec(const Vec3& value) {
			auto result = Vec3(0.0f);

			result.x = abs(value.x);
			result.y = abs(value.y);
			result.z = abs(value.z);

			return result;
		}

		float Mathf::SmoothDamp(const float& current, float target, float& currentVelocity, float smoothTime) {
			auto delta = App::GetApp()->GetElapsedTime();
			return SmoothDamp(current, target, currentVelocity, smoothTime, Mathf::INFINITY_FLOAT, delta);
		}

		float Mathf::SmoothDamp(const float& current, float target, 
			float& currentVelocity, float smoothTime, 
			const float& maxSpeed, const float& deltaTime) 
		{
			smoothTime = Mathf::Max(0.0001f, smoothTime);
			float num = 2.0f / smoothTime;
			float num2 = num * deltaTime;
			float num3 = 1.0f / (1.0f + num2 + 0.48f * num2 * num2 + 0.235f * num2 * num2 * num2);
			float num4 = current - target;
			float num5 = target;
			float num6 = maxSpeed * smoothTime;
			num4 = Mathf::Clamp(num4, -num6, num6);
			target = current - num4;
			float num7 = (currentVelocity + num * num4) * deltaTime;
			currentVelocity = (currentVelocity - num * num7) * num3;
			float num8 = target + (num4 + num7) * num3;
			if (num5 - current > 0.0f == num8 > num5)
			{
				num8 = num5;
				currentVelocity = (num8 - num5) / deltaTime;
			}

			return num8;
		}

		Vec3 Mathf::SmoothDamp(const Vec3& current, Vec3 target, Vec3& currentVelocity, float smoothTime) {
			auto delta = App::GetApp()->GetElapsedTime();
			return SmoothDamp(current, target, currentVelocity, smoothTime, 1.0f, delta);
		}

		Vec3 Mathf::SmoothDamp(const Vec3& current, Vec3 target, Vec3& currentVelocity, float smoothTime, const float& maxSpeed, const float& deltaTime) {
			float x = SmoothDamp(current.x, target.x, currentVelocity.x, smoothTime, maxSpeed, deltaTime);
			float y = SmoothDamp(current.y, target.y, currentVelocity.y, smoothTime, maxSpeed, deltaTime);
			float z = SmoothDamp(current.z, target.z, currentVelocity.z, smoothTime, maxSpeed, deltaTime);

			return Vec3(x, y, z);
		}
	}
}