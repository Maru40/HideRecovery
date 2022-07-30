/*!
@file WaterWepon.h
@brief WaterWepon�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �����W�F�b�g����
	//--------------------------------------------------------------------------------------
	class WaterWepon : public WeponBase
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		WaterWepon(const std::shared_ptr<GameObject>& objPtr);

	protected:
		virtual void HoldShotUpdate() override;
		virtual void HoldShotEnd() override;

		/// <summary>
		/// ��΂������̎擾
		/// </summary>
		/// <returns>��΂�����</returns>
		Vec3 GetDirect() const;
	};

}