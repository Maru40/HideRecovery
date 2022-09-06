/*!
@file ScaleFade.h
@brief ScaleFade
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�傫���t�F�[�h�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct ScaleFade_Parametor
	{
		float speed;                                  //�t�F�[�h�X�s�[�h
		Vec3 direct = Vec3(-1.0f);                    //�t�F�[�h����
		Vec3 targetScale = Vec3(0.0f);                //�ڕW�X�P�[��
		std::function<void()> endFunction = nullptr;  //�t�F�[�h�I�����ɌĂяo���C�x���g
	};

	//--------------------------------------------------------------------------------------
	///	�傫���t�F�[�h�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class ScaleFade : public Component
	{
	public:
		using Parametor = ScaleFade_Parametor;

	private:
		Parametor m_param; //�p�����[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="parametor">�p�����[�^</param>
		ScaleFade(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnUpdate() override;

	private:
		/// <summary>
		/// �傫���X�V
		/// </summary>
		void UpdateScale();

		/// <summary>
		/// �X�P�[���̕ύX���~�߂邩�ǂ���
		/// </summary>
		/// <param name="scale">���݂̃X�P�[��</param>
		/// <param name="targetScale">�ڕW�X�P�[��</param>
		/// <returns>�ڕW�X�P�[����菬�����Ȃ�����true</returns>
		bool IsStopScale(const Vec3& scale, const Vec3& targetScale) const;

	public:
		/// <summary>
		/// �I�����f
		/// </summary>
		/// <returns>�傫���̍X�V���I�����Ă�����true</returns>
		bool IsEnd() const {
			return IsStopScale(transform->GetScale(), m_param.targetScale);
		}
	};

}