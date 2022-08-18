/*!
@file Goar.h
@brief Goar�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Watanabe/StageObject/PlayerSpawnPointObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	struct CollisionPair;

	//--------------------------------------------------------------------------------------
	/// �S�[���Ǘ��N���X�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct Goar_Parametor {
		Team team;

		Goar_Parametor(const Team& team);
	};

	//--------------------------------------------------------------------------------------
	/// �S�[���Ǘ��N���X
	//--------------------------------------------------------------------------------------
	class Goar : public Component
	{
	public:
		using Parametor = Goar_Parametor;

	private:
		Parametor m_param;

	public:
		Goar(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// �S�[������
		/// </summary>
		/// <param name="pair">�S�[���������l</param>
		void SuccessGoar(const CollisionPair& pair);

		/// <summary>
		/// �����蔻����Ƃ邩�ǂ���
		/// </summary>
		/// <param name="pair">������������̃f�[�^</param>
		/// <returns>�����蔻����Ƃ�Ȃ�true</returns>
		bool IsCollision(const CollisionPair& pair) const;

	public:
		void OnCollisionEnter(const CollisionPair& pair) override;

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �`�[���^�C�v�̐ݒ�
		/// </summary>
		/// <param name="team">�`�[���^�C�v</param>
		void SetTeam(const Team& team) noexcept { m_param.team = team; }

		/// <summary>
		/// �`�[���^�C�v�̎擾
		/// </summary>
		/// <returns>�`�[���^�C�v</returns>
		Team GetTeam() const noexcept { return m_param.team; }

	};

}