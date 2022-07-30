/*!
@file PressRefrectionManager.h
@brief PressRefrectionManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	struct CollisionPair;
	struct PressData;

	//--------------------------------------------------------------------------------------
	/// �ׂ����ˊǗ�
	//--------------------------------------------------------------------------------------
	class PressRefrectionManager : public Component
	{
	public:
		/// <summary>
		/// �p�����[�^
		/// </summary>
		struct Parametor
		{
			Vec3 velocity;           //���x
			float deselerationPower; //�������x

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="velocity">���x</param>
			/// <param name="deselerationPower">�������x</param>
			Parametor(const Vec3& velocity, const float& deselerationPower);
		};

	private:
		Parametor m_param;   //�p�����[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		PressRefrectionManager(const std::shared_ptr<GameObject>& objPtr);

	private:
		/// <summary>
		/// ���˂���x�N�g����Ԃ��B
		/// </summary>
		/// <param name="data">�ׂ��f�[�^</param>
		/// <returns>���˂����x�N�g��</returns>
		Vec3 CalcuReflectionVec(const PressData& data);

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ���t���N�V�����J�n
		/// </summary>
		/// <param name="data">�����Ԃ����Ƃ��ɕK�v�ȃf�[�^</param>
		/// <param name="deselerationPower">������</param>
		void StartReflection(const PressData& data, const float& deselerationPower = 1.0f);
	};

}

//endbasecross