
/*!
@file Mathf.h
@brief Mathf�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	���w�n�N���X
		//--------------------------------------------------------------------------------------
		class Mathf
		{

		public:
			static const float INFINITY_FLOAT; //�ƂĂ��傫��float�l

			/// <summary>
			/// X�����ɓn���ꂽVec3��rad����]���������l��Ԃ�
			/// </summary>
			/// <param name="vec">��]���������x�N�g��</param>
			/// <param name="rad">��]���������p�x</param>
			/// <returns>��]�������x�N�g��</returns>
			static Vec3 Mat4RotationAxisX(const Vec3& vec, const float rad);

			/// <summary>
			/// X������mat4x4��rad����]������B
			/// </summary>
			/// <param name="mat">��]��������mad4x4</param>
			/// <param name="rad">��]���������p�x</param>
			/// <returns>��]������4x4</returns>
			static Mat4x4 Mat4RotationAxisX(const Mat4x4& mat, const float rad);

			/// <summary>
			/// Y�����ɓn���ꂽVec3��rad����]���������l��Ԃ�
			/// </summary>
			/// <param name="vec">��]���������x�N�g��</param>
			/// <param name="rad">��]���������p�x</param>
			/// <returns>��]�������x�N�g��</returns>
			static Vec3 Mat4RotationAxisY(const Vec3& vec, const float rad);

			/// <summary>
			/// Y������mat4x4��rad����]������B
			/// </summary>
			/// <param name="mat">��]��������mad4x4</param>
			/// <param name="rad">��]���������p�x</param>
			/// <returns>��]������4x4</returns>
			static Mat4x4 Mat4RotationAxisY(const Mat4x4& mat, const float rad);

			/// <summary>
			/// Z������Vec3��rad����]������
			/// </summary>
			/// <param name="vec">��]���������x�N�g��</param>
			/// <param name="rad">��]���������p�x</param>
			/// <returns>��]�������x�N�g��</returns>
			static Vec3 Mathf::Mat4RotationAxisZ(const Vec3& vec, const float rad);

			/// <summary>
			/// Z������mat4x4��rad����]������B
			/// </summary>
			/// <param name="mat">��]��������mad4x4</param>
			/// <param name="rad">��]���������p�x</param>
			/// <returns>��]������4x4</returns>
			static Mat4x4 Mat4RotationAxisZ(const Mat4x4& mat, const float rad);

			/// <summary>
			/// �f�O���[�p�����ɓn�����x�N�g������]�����ĕԂ��B
			/// </summary>
			/// <param name="vec">��]�������x�N�g��</param>
			/// <param name="deg">��]�������f�O���[�p</param>
			/// <param name="axis">��]��</param>
			/// <returns>��]�������x�N�g��</returns>
			static Vec3 RotationDegreeVec(const Vec3& vec, const float& deg, const Vec3& axis);

			/// <summary>
			/// ���W�A���p�����ɓn�����x�N�g������]�����ĕԂ��B
			/// </summary>
			/// <param name="vec">��]�������x�N�g��</param>
			/// <param name="rad">��]���������W�A���p</param>
			/// <param name="axis">��]��</param>
			/// <returns>��]�������x�N�g��</returns>
			static Vec3 RotationRadianVec(const Vec3& vec, const float& rad, const Vec3& axis);

			/// <summary>
			/// ��̃x�N�g���̊p�x�������߂�
			/// </summary>
			/// <param name="a">�x�N�g��</param>
			/// <param name="b">�x�N�g��</param>
			/// <returns>��̃x�N�g���̊p�x��</returns>
			static float CalculateAngle(const Vec3& a, const Vec3& b);

			/// <summary>
			/// ��̃x�N�g���̉�]�������߂�B
			/// </summary>
			/// <param name="a">�x�N�g��</param>
			/// <param name="b">�x�N�g��</param>
			/// <returns>��̃x�N�g���̉�]��</returns>
			static Vec3 CalculateRotationAxis(const Vec3& a, const Vec3& b);

			/// <summary>
			/// ��̃x�N�g���̍����w�肵���f�O���[�p�����ǂ���
			/// </summary>
			/// <param name="a">�x�N�g��</param>
			/// <param name="b">�x�N�g��</param>
			/// <param name="degree">�f�O���[�p</param>
			/// <returns>��̃x�N�g���̍����w�肵���f�O���[�p���Ȃ�true</returns>
			static bool IsInAngleDegree(const Vec3& a, const Vec3& b, const float degree);

			/// <summary>
			/// ��̃x�N�g���̍����w�肵�����W�A���p�����ǂ���
			/// </summary>
			/// <param name="a">�x�N�g��</param>
			/// <param name="b">�x�N�g��</param>
			/// <param name="degree">�f�O���[�p</param>
			/// <returns>��̃x�N�g���̍����w�肵�����W�A���p���Ȃ�true</returns>
			static bool IsInAngleRadian(const Vec3& a, const Vec3& b, const float radian);

			/// <summary>
			/// �ʒu�⊮�֐�
			/// </summary>
			/// <param name="startPos">�J�n�ʒu</param>
			/// <param name="endPos">�I���ʒu</param>
			/// <param name="elapsedTime">�o�ߎ���</param>
			/// <returns>�⊮�ʒu</returns>
			static Vec3 Lerp(const Vec3& startPos, const Vec3& endPos, const float& elapsedTime);

			/// <summary>
			/// NaN���ǂ����̔��f
			/// </summary>
			/// <param name="vector">���f������Vector</param>
			/// <returns>NaN�Ȃ�ture</returns>
			static bool IsNaN(const Vec3& vector);

			/// <summary>
			/// �傫�����̐��l��Ԃ�
			/// </summary>
			template<class T>
			static T Max(const T& a, const T& b) {
				return a > b ? a : b;
			}

			/// <summary>
			/// ���������̐��l��Ԃ�
			/// </summary>
			template<class T>
			static T Min(const T& a, const T& b) {
				return a < b ? a : b;
			}

			/// <summary>
			/// min,max�̊Ԃ̐�����Ԃ��B
			/// </summary>
			/// <param name="value">�w��x�N�g��</param>
			/// <param name="min">�ŏ��l</param>
			/// <param name="max">�ő�l</param>
			/// <returns>min,max�̊Ԃ̐���</returns>
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
			/// �x�N�g����Clamp����
			/// </summary>
			/// <param name="value">�w��x�N�g��</param>
			/// <param name="min">�ŏ��l</param>
			/// <param name="max">�ő�l</param>
			/// <returns>Clamp�����x�N�g��</returns>
			static Vec3 ClampVec(const Vec3& value, const Vec3& min, const Vec3& max);

			/// <summary>
			/// �x�N�g���̗v�f���Βl�ɂ���
			/// </summary>
			/// <param name="value">�x�N�g��</param>
			/// <returns>��Βl�ɂ����x�N�g��</returns>
			static Vec3 AbsVec(const Vec3& value);

			/// <summary>
			/// �w�肵�����Ԋ|���Ĉړ�����
			/// </summary>
			/// <param name="current">���ݒl</param>
			/// <param name="target">�ړI�l</param>
			/// <param name="currentVelocity">���݂̑��x</param>
			/// <param name="smoothTime">�ړI�n�܂ł����鎞��</param>
			/// <returns>�ω���̒l</returns>
			static float SmoothDamp(const float& current, float target, float& currentVelocity, float smoothTime);

			/// <summary>
			/// �w�肵�����Ԋ|���Ĉړ�����
			/// </summary>
			/// <param name="current">���ݒl</param>
			/// <param name="target">�ړI�l</param>
			/// <param name="currentVelocity">���݂̑��x</param>
			/// <param name="smoothTime">�ړI�n�܂ł����鎞��</param>
			/// <param name="maxSpeed">�ő呬�x</param>
			/// <param name="deltaTime">�v�����x</param>
			/// <returns>�ω���̒l</returns>
			static float SmoothDamp(const float& current, float target, float& currentVelocity, float smoothTime, const float& maxSpeed, const float& deltaTime);

			/// <summary>
			/// �w�肵�����Ԋ|���Ĉړ�����
			/// </summary>
			/// <param name="current">���ݒl</param>
			/// <param name="target">�ړI�l</param>
			/// <param name="currentVelocity">���݂̑��x</param>
			/// <param name="smoothTime">�ړI�n�܂ł����鎞��</param>
			/// <returns></returns>
			static Vec3 SmoothDamp(const Vec3& current, Vec3 target, Vec3& currentVelocity, float smoothTime);

			/// <summary>
			/// �w�肵�����Ԋ|���Ĉړ�����
			/// </summary>
			/// <param name="current">���ݒl</param>
			/// <param name="target">�ړI�l</param>
			/// <param name="currentVelocity">���݂̑��x</param>
			/// <param name="smoothTime">�ړI�n�܂ł����鎞��</param>
			/// <param name="maxSpeed">�ő呬�x</param>
			/// <param name="deltaTime">�v�����x</param>
			/// <returns></returns>
			static Vec3 SmoothDamp(const Vec3& current, Vec3 target, Vec3& currentVelocity, float smoothTime, const float& maxSpeed, const float& deltaTime);
		};
	}

	

}