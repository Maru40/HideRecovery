/*!
@file HeightDestroy.h
@brief HeightDestroy�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	namespace maru {
		template<class T>
		class Action;
	}

	//--------------------------------------------------------------------------------------
	/// ���̍����Ŏ��S������o���R���|�[�l���g�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct HeightDestroy_Parametor
	{
		float height;

		HeightDestroy_Parametor();
	};

	//--------------------------------------------------------------------------------------
	/// ���̍����Ŏ��S������o���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class HeightDestroy : public Component
	{
	public:
		using Parametor = HeightDestroy_Parametor;

	private:
		Parametor m_param;

		std::unique_ptr<maru::Action<void()>> m_action;

	public:
		HeightDestroy(const std::shared_ptr<GameObject>& objPtr);

		void OnUpdate() override;

	private:
		void ObserveHeight();

		void Invoke();

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		void AddDestroyAction(const std::function<void()>& func);

		bool IsDestroyHeight() const;

		void SetHeight(const float height) noexcept { m_param.height = height; }

		float GetHeight() const noexcept { return m_param.height; }
	};

}