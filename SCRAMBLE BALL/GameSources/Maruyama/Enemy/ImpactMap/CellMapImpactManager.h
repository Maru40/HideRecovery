
/*!
@file CellMapImpactController.h
@brief CellMapImpactController�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Thread/FutureData.h"

#include "Maruyama/Interface/I_Impact.h"

#include "Maruyama/Thread/ThreadPool.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class ThreadPool;

	namespace maru {
	
		class Cell;
		class CellMap;

		//--------------------------------------------------------------------------------------
		///	�Z���}�b�v�̉e���x�R���g���[���[
		//--------------------------------------------------------------------------------------
		class CellMapImpactController : public I_ThreadRequester
		{
		public:
			using Cells = std::vector<std::shared_ptr<Cell>>;
			using FutureData = FutureData<Cells>;

		private:
			mutable std::mutex m_mutex;					//�~���[�e�b�N�X
			std::unique_ptr<ThreadPool> m_threadPool;	//�X���b�h�v�[��

			std::weak_ptr<CellMap> m_cellMap;			//�Z���}�b�v

		public:
			CellMapImpactController(
				const std::shared_ptr<CellMap>& cellMap,
				const std::uint_fast32_t& thread_count = std::thread::hardware_concurrency()
			);

			virtual ~CellMapImpactController() = default;

		private:
			/// <summary>
			/// �e���x�̍X�V
			/// </summary>
			/// <param name="futureData">�t���[�`���[�f�[�^</param>
			/// <returns>�X�V���ꂽ�f�[�^</returns>
			Cells ThreadWoker_CalculateImpactCells(
				const std::weak_ptr<GameObject>& requester, 
				const std::shared_ptr<FutureData>& futureData
			);

		public:

			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �^�X�N�̂��肢
			/// </summary>
			/// <param name="requester">���肢�����l</param>
			std::shared_ptr<FutureData> AddTask(const std::weak_ptr<GameObject>& requester);
		};

	}
}