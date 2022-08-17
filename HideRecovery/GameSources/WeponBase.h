
/*!
@file WeponBase.h
@brief WeponBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �E�F�|���̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct WeponBase_Parametor {
		Vec3 bulletInstanceOffset;	//�o���b�g�𐶐�����ʒu�̃I�t�Z�b�g

		WeponBase_Parametor();
	};

	//--------------------------------------------------------------------------------------
	/// �E�F�|���̊��N���X
	//--------------------------------------------------------------------------------------
	template<class BulletType>
	class WeponBase : public Component
	{
	public:
		using Parametor = WeponBase_Parametor;

	private:
		Parametor m_param;

	public:
		WeponBase(const std::shared_ptr<GameObject>& objPtr):
			Component(objPtr)
		{}

		/// <summary>
		/// �e������
		/// </summary>
		/// <param name="direct">������</param>
		virtual void Shot(const Vec3& direct) = 0;

		/// <summary>
		/// �e�̐���
		/// </summary>
		template<class... Ts>
		std::shared_ptr<BulletType> InstantiateBullet(const Vec3& position, const Quat& quat, Ts... params) {
			auto bulletObject = GetStage()->Instantiate<BulletType>(position, quat, params...);
			//auto bullet = bulletObject->AddComponent<BulletType>();

			return bulletObject;
		}

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �p�����[�^�̐ݒ�
		/// </summary>
		/// <param name="parametor">�p�����[�^</param>
		void SetParametor(const Parametor& parametor) noexcept { m_param = parametor; }

		/// <summary>
		/// �p�����[�^�̎擾
		/// </summary>
		/// <returns>�p�����[�^</returns>
		Parametor GetParametor() const noexcept { return m_param; }

		/// <summary>
		/// �o���b�g�𐶐�����ʒu�̃I�t�Z�b�g�̐ݒ�
		/// </summary>
		/// <param name="offset">�o���b�g�𐶐�����ʒu�̃I�t�Z�b�g</param>
		void SetBulletInstanceOffset(const Vec3& offset) noexcept { m_param.bulletInstanceOffset = offset; }

		/// <summary>
		/// �o���b�g�𐶐�����ʒu�̃I�t�Z�b�g�̎擾
		/// </summary>
		/// <returns>�o���b�g�𐶐�����ʒu�̃I�t�Z�b�g</returns>
		Vec3 GetBulletInstanceOffset() const noexcept { return m_param.bulletInstanceOffset; }

	};

}