
/*!
@file MaruHitHelper.h
@brief MaruHitHelper�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�Փ˃f�[�^
		//--------------------------------------------------------------------------------------
		struct HitData
		{
			ex_weak_ptr<GameObject> selfObject = nullptr; //�������g
			ex_weak_ptr<GameObject> hitObject = nullptr;  //������������
			Vec3 point;    //�q�b�g�ꏊ
			Vec3 normal;   //�@��

			/// <summary>
			/// �������g�̈ʒu���擾
			/// </summary>
			/// <returns>�������g�̈ʒu</returns>
			Vec3 GetSelfPosition();

			/// <summary>
			/// �������g����q�b�g�ꏊ�Ɍ������x�N�g�����擾����
			/// </summary>
			/// <returns>�������g����q�b�g�ꏊ�Ɍ������x�N�g��</returns>
			Vec3 GetSelfToHitVec();

			/// <summary>
			/// �q�b�g�ꏊ���玩�����g�Ɍ������x�N�g����Ԃ�
			/// </summary>
			/// <returns>�q�b�g�ꏊ���玩�����g�Ɍ������x�N�g��</returns>
			Vec3 GetHitToSelfVec();
		};

		//--------------------------------------------------------------------------------------
		///	�Փ˃w���p�[
		//--------------------------------------------------------------------------------------
		class HitHelper
		{

		public:

			/// <summary>
			/// selfPosition���猩��OBB�̍ŋߐړ_�Ƃ���Normal�𓾂�(selfObject��hitObject��nullptr)
			/// </summary>
			/// <param name="selfPosition">�������g�̃|�W�V����</param>
			/// <param name="other">�����OBB</param>
			/// <returns>HitData</returns>
			static HitData ClosestPtPointOBB(const Vec3& selfPosition, const std::shared_ptr<CollisionObb>& other);

			//���ݎg�p�֎~
			static HitData ClosestPtPointOBB(const std::shared_ptr<GameObject>& self, const std::shared_ptr<CollisionObb>& other);
		};
	}

}