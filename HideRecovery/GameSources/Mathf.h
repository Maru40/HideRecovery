
/*!
@file Mathf.h
@brief Mathfクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	数学系クラス
		//--------------------------------------------------------------------------------------
		class Mathf
		{

		public:
			static const float INFINITY_FLOAT; //とても大きいfloat値

			/// <summary>
			/// Xを軸に渡されたVec3をrad分回転させた数値を返す
			/// </summary>
			/// <param name="vec">回転させたいベクトル</param>
			/// <param name="rad">回転させたい角度</param>
			/// <returns>回転させたベクトル</returns>
			static Vec3 Mat4RotationAxisX(const Vec3& vec, const float rad);

			/// <summary>
			/// Xを軸にmat4x4をrad分回転させる。
			/// </summary>
			/// <param name="mat">回転させたいmad4x4</param>
			/// <param name="rad">回転させたい角度</param>
			/// <returns>回転させた4x4</returns>
			static Mat4x4 Mat4RotationAxisX(const Mat4x4& mat, const float rad);

			/// <summary>
			/// Yを軸に渡されたVec3をrad分回転させた数値を返す
			/// </summary>
			/// <param name="vec">回転させたいベクトル</param>
			/// <param name="rad">回転させたい角度</param>
			/// <returns>回転させたベクトル</returns>
			static Vec3 Mat4RotationAxisY(const Vec3& vec, const float rad);

			/// <summary>
			/// Yを軸にmat4x4をrad分回転させる。
			/// </summary>
			/// <param name="mat">回転させたいmad4x4</param>
			/// <param name="rad">回転させたい角度</param>
			/// <returns>回転させた4x4</returns>
			static Mat4x4 Mat4RotationAxisY(const Mat4x4& mat, const float rad);

			/// <summary>
			/// Zを軸にVec3をrad分回転させる
			/// </summary>
			/// <param name="vec">回転させたいベクトル</param>
			/// <param name="rad">回転させたい角度</param>
			/// <returns>回転させたベクトル</returns>
			static Vec3 Mathf::Mat4RotationAxisZ(const Vec3& vec, const float rad);

			/// <summary>
			/// Zを軸にmat4x4をrad分回転させる。
			/// </summary>
			/// <param name="mat">回転させたいmad4x4</param>
			/// <param name="rad">回転させたい角度</param>
			/// <returns>回転させた4x4</returns>
			static Mat4x4 Mat4RotationAxisZ(const Mat4x4& mat, const float rad);

			/// <summary>
			/// デグリー角を元に渡したベクトルを回転させて返す。
			/// </summary>
			/// <param name="vec">回転したいベクトル</param>
			/// <param name="deg">回転したいデグリー角</param>
			/// <param name="axis">回転軸</param>
			/// <returns>回転させたベクトル</returns>
			static Vec3 RotationDegreeVec(const Vec3& vec, const float& deg, const Vec3& axis);

			/// <summary>
			/// ラジアン角を元に渡したベクトルを回転させて返す。
			/// </summary>
			/// <param name="vec">回転したいベクトル</param>
			/// <param name="rad">回転したいラジアン角</param>
			/// <param name="axis">回転軸</param>
			/// <returns>回転させたベクトル</returns>
			static Vec3 RotationRadianVec(const Vec3& vec, const float& rad, const Vec3& axis);

			/// <summary>
			/// 二つのベクトルの角度差を求める
			/// </summary>
			/// <param name="a">ベクトル</param>
			/// <param name="b">ベクトル</param>
			/// <returns>二つのベクトルの角度差</returns>
			static float CalculateAngle(const Vec3& a, const Vec3& b);

			/// <summary>
			/// 二つのベクトルの回転軸を求める。
			/// </summary>
			/// <param name="a">ベクトル</param>
			/// <param name="b">ベクトル</param>
			/// <returns>二つのベクトルの回転軸</returns>
			static Vec3 CalculateRotationAxis(const Vec3& a, const Vec3& b);

			/// <summary>
			/// 二つのベクトルの差が指定したデグリー角内かどうか
			/// </summary>
			/// <param name="a">ベクトル</param>
			/// <param name="b">ベクトル</param>
			/// <param name="degree">デグリー角</param>
			/// <returns>二つのベクトルの差が指定したデグリー角内ならtrue</returns>
			static bool IsInAngleDegree(const Vec3& a, const Vec3& b, const float degree);

			/// <summary>
			/// 二つのベクトルの差が指定したラジアン角内かどうか
			/// </summary>
			/// <param name="a">ベクトル</param>
			/// <param name="b">ベクトル</param>
			/// <param name="degree">デグリー角</param>
			/// <returns>二つのベクトルの差が指定したラジアン角内ならtrue</returns>
			static bool IsInAngleRadian(const Vec3& a, const Vec3& b, const float radian);

			/// <summary>
			/// 位置補完関数
			/// </summary>
			/// <param name="startPos">開始位置</param>
			/// <param name="endPos">終了位置</param>
			/// <param name="elapsedTime">経過時間</param>
			/// <returns>補完位置</returns>
			static Vec3 Lerp(const Vec3& startPos, const Vec3& endPos, const float& elapsedTime);

			/// <summary>
			/// NaNかどうかの判断
			/// </summary>
			/// <param name="vector">判断したいVector</param>
			/// <returns>NaNならture</returns>
			static bool IsNaN(const Vec3& vector);

			/// <summary>
			/// 大きい方の数値を返す
			/// </summary>
			template<class T>
			static T Max(const T& a, const T& b) {
				return a > b ? a : b;
			}

			/// <summary>
			/// 小さい方の数値を返す
			/// </summary>
			template<class T>
			static T Min(const T& a, const T& b) {
				return a < b ? a : b;
			}

			/// <summary>
			/// min,maxの間の数字を返す。
			/// </summary>
			/// <param name="value">指定ベクトル</param>
			/// <param name="min">最小値</param>
			/// <param name="max">最大値</param>
			/// <returns>min,maxの間の数字</returns>
			template<class T>
			static T Clamp(const T& value, const T& min, const T& max) {
				T reValue = value;

				if (reValue < min) {
					reValue = min;
				}

				if (reValue > max) {
					reValue = max;
				}

				return reValue;
			}

			/// <summary>
			/// ベクトルをClampする
			/// </summary>
			/// <param name="value">指定ベクトル</param>
			/// <param name="min">最小値</param>
			/// <param name="max">最大値</param>
			/// <returns>Clampしたベクトル</returns>
			static Vec3 ClampVec(const Vec3& value, const Vec3& min, const Vec3& max);

			/// <summary>
			/// ベクトルの要素を絶対値にする
			/// </summary>
			/// <param name="value">ベクトル</param>
			/// <returns>絶対値にしたベクトル</returns>
			static Vec3 AbsVec(const Vec3& value);

			/// <summary>
			/// 指定した時間掛けて移動する
			/// </summary>
			/// <param name="current">現在値</param>
			/// <param name="target">目的値</param>
			/// <param name="currentVelocity">現在の速度</param>
			/// <param name="smoothTime">目的地までかかる時間</param>
			/// <returns>変化後の値</returns>
			static float SmoothDamp(const float& current, float target, float& currentVelocity, float smoothTime);

			/// <summary>
			/// 指定した時間掛けて移動する
			/// </summary>
			/// <param name="current">現在値</param>
			/// <param name="target">目的値</param>
			/// <param name="currentVelocity">現在の速度</param>
			/// <param name="smoothTime">目的地までかかる時間</param>
			/// <param name="maxSpeed">最大速度</param>
			/// <param name="deltaTime">計測速度</param>
			/// <returns>変化後の値</returns>
			static float SmoothDamp(const float& current, float target, float& currentVelocity, float smoothTime, const float& maxSpeed, const float& deltaTime);

			/// <summary>
			/// 指定した時間掛けて移動する
			/// </summary>
			/// <param name="current">現在値</param>
			/// <param name="target">目的値</param>
			/// <param name="currentVelocity">現在の速度</param>
			/// <param name="smoothTime">目的地までかかる時間</param>
			/// <returns></returns>
			static Vec3 SmoothDamp(const Vec3& current, Vec3 target, Vec3& currentVelocity, float smoothTime);

			/// <summary>
			/// 指定した時間掛けて移動する
			/// </summary>
			/// <param name="current">現在値</param>
			/// <param name="target">目的値</param>
			/// <param name="currentVelocity">現在の速度</param>
			/// <param name="smoothTime">目的地までかかる時間</param>
			/// <param name="maxSpeed">最大速度</param>
			/// <param name="deltaTime">計測速度</param>
			/// <returns></returns>
			static Vec3 SmoothDamp(const Vec3& current, Vec3 target, Vec3& currentVelocity, float smoothTime, const float& maxSpeed, const float& deltaTime);
		};
	}

	

}