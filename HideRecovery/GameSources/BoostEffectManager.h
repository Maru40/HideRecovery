/*!
@file BoostEffectManager.h
@brief BoostEffectManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class BoostEffectObject;

	//--------------------------------------------------------------------------------------
	/// �u�[�X�g�G�t�F�N�g�Ǘ�
	//--------------------------------------------------------------------------------------
	class BoostEffectManager : public Component
	{
	private:
		ex_weak_ptr<BoostEffectObject> m_effectObject;  //�G�t�F�N�g�I�u�W�F�N�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stage">���̃N���X����������X�e�[�W</param>
		BoostEffectManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// ��������ꏊ���v�Z���Ď擾
		/// </summary>
		/// <returns>��������ꏊ</returns>
		Vec3 CalculatePosition();

	public:
		/// <summary>
		/// �G�t�F�N�g�̐���
		/// </summary>
		void CreateEffect();

	};

}