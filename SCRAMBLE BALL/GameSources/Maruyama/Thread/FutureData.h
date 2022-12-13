/*!
@file FutureData.h
@brief FutureData�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include <future>

namespace basecross {

	template<class R>
	struct FutureData {
		mutable std::atomic<bool> m_isRunning;	//�X�V�����ǂ���
		std::future<R> m_future;				//�t���[�`���[�f�[�^

		FutureData() :
			m_isRunning(true)
		{}

		/// <summary>
		/// �t���[�`���[�f�[�^�̈����n���B
		/// </summary>
		/// <param name="other">�����n���f�[�^</param>
		void MoveFuture(std::future<R>& other) {
			m_future = std::move(other);
		}
	};

}