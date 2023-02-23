
/*!
@file CellMapImpactManager.cpp
@brief CellMapImpactManager�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "CellMapImpactManager.h"

#include "Maruyama/Thread/ThreadPool.h"
#include "Maruyama/Enemy/ImpactMap/CellMap.h"

namespace basecross {

	namespace maru {

		CellMapImpactController::CellMapImpactController(
			const std::shared_ptr<CellMap>& cellMap,
			const std::uint_fast32_t& thread_count
		) :
			m_cellMap(cellMap),
			m_threadPool(new ThreadPool(thread_count))
		{ }

		CellMapImpactController::Cells CellMapImpactController::ThreadWoker_CalculateImpactCells(
			const std::weak_ptr<GameObject>& requester, 
			const std::shared_ptr<FutureData>& futureData
		) {
			auto result = Cells();

			//�e���x�̌v�Z------
			
					//�����̃Z���̏ꏊ�������B(�ʂŏ�ɊĎ����Ă�f�[�^�������I�ɗp��)
					//�����̎���̉e���x��������B
					//���E�ɍ��킹�ĉe���x�𒲐��B
					//��Q���ɍ��킹�ĉe���x�𒲐��B

			//------------------
			
			futureData->m_isRunning = false;
			return result;
		}

		std::shared_ptr<CellMapImpactController::FutureData> CellMapImpactController::AddTask(const std::weak_ptr<GameObject>& requester) {
			auto futureData = std::make_shared<FutureData>();	//�t���[�`���[�f�[�^�̐����B
			auto future = m_threadPool->Submit(this, &CellMapImpactController::ThreadWoker_CalculateImpactCells, this, requester, futureData);	//�X���b�h�ɃT�u�~�b�g�B
			futureData->MoveFuture(future);		//�t���[�`���[�̐ݒ�B
			return futureData;	
		}

	}
}