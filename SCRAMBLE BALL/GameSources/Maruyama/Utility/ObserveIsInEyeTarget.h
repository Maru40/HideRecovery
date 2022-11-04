/*!
@file ObserveIsInEyeTarget.h
@brief ObserveIsInEyeTarget�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class EyeSearchRange;

	//--------------------------------------------------------------------------------------
	/// �Ď��Ώۂ����E�͈͂ɂ��邩�ǂ����𔻒f����N���X
	//--------------------------------------------------------------------------------------
	class ObserveIsInEyeTarget
	{
	public:
		using ObserveTargets = std::vector<std::weak_ptr<GameObject>>;
		using ObserveSharedTargets = std::vector<std::shared_ptr<GameObject>>;

	private:
		ObserveTargets m_observeTargets;			//�Ď��Ώ۔z��

		std::weak_ptr<EyeSearchRange> m_eyeRange;	//���E�Ǘ��R���|�[�l���g

	public:
		ObserveIsInEyeTarget(const std::shared_ptr<EyeSearchRange>& eyeRange);

		ObserveIsInEyeTarget(
			const std::shared_ptr<EyeSearchRange>& eyeRange,
			const ObserveSharedTargets& observeTargets
		);

		ObserveIsInEyeTarget(
			const std::shared_ptr<EyeSearchRange>& eyeRange,
			const ObserveTargets& observeTargets
		);

		virtual ~ObserveIsInEyeTarget() = default;

		/// <summary>
		/// ���E���̃^�[�Q�b�g���擾����B
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<GameObject> SearchIsInEyeTarget() const;

		/// <summary>
		/// ���E���̃^�[�Q�b�g��S�Ď擾����B
		/// </summary>
		/// <returns>���E���̃^�[�Q�b�g�S��</returns>
		std::vector<std::shared_ptr<GameObject>> SearchIsInEyeTargets() const;

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �Ď��Ώۂ̒ǉ�
		/// </summary>
		/// <param name="target">�Ď��Ώ�</param>
		void AddObserveTarget(const std::shared_ptr<GameObject>& target) {
			m_observeTargets.push_back(target);
		}

		void SetObserveTargets(const ObserveTargets& targets) { m_observeTargets = targets; }

		void SetObserveTargets(const ObserveSharedTargets& targets);

		_NODISCARD ObserveTargets GetObserveTargets() const noexcept { return m_observeTargets; }

		void ClearObserveTargets() { m_observeTargets.clear(); }

	};

}