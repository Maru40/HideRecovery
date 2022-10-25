/*!
@file OpenDataHandler.cpp
@brief OpenDataHandler�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "OpenDataHandler.h"

#include "Heuristic.h"
#include "NavGraphNode.h"
#include "AstarEdge.h"
#include "AstarGraph.h"

#include "Maruyama/Utility/Utility.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�I�[�v���f�[�^
	//--------------------------------------------------------------------------------------

	OpenData::OpenData()
		:OpenData(nullptr, nullptr, 0, 0)
	{}

	OpenData::OpenData(const std::shared_ptr<NavGraphNode>& parent, const std::shared_ptr<NavGraphNode>& node, const float range, const float heuristic)
		: parent(parent) ,node(node), range(range), heuristic(heuristic), isActive(true)
	{}

	float OpenData::GetSumRange() const {
		return range + heuristic;
	}

	bool OpenData::operator == (const OpenData& data) const {
		return this->node.lock() == data.node.lock();
	}

	//--------------------------------------------------------------------------------------
	///	�I�[�v���f�[�^�������ҁB
	//--------------------------------------------------------------------------------------

	OpenDataHandler::OpenDataHandler() :
		m_heuristic(new Heuristic())
	{}

	/// <summary>
	/// �I�[�v���f�[�^���X�g�̃\�[�g������(���v�l��ŏ����\�[�g��)
	/// </summary>
	/// <param name="left">���t�g�f�[�^</param>
	/// <param name="right">���C�g�f�[�^</param>
	/// <returns>���̍��v�l���������Ȃ�true</returns>
	bool IsSmall_LeftOpenData(const std::shared_ptr<OpenData>& left, const std::shared_ptr<OpenData>& right) {
		return left->GetSumRange() < right->GetSumRange();	//���v�l�����������ɕ��ׂ�B 
	}

	std::shared_ptr<OpenData> OpenDataHandler::FindSearchBaseOpenData(DataPtrList& openDataList) {
		openDataList.sort(&IsSmall_LeftOpenData);

		return openDataList.front();
	}

	bool OpenDataHandler::CreateOpenDatas(
		DataPtrList& openDataList,
		DataPtrList& closeDataList,
		const std::shared_ptr<OpenData>& openData,
		const std::shared_ptr<AstarGraph>& graph,
		const std::shared_ptr<NavGraphNode>& startNode,
		const std::shared_ptr<NavGraphNode>& targetNode
	) {
		int baesIndex = openData->node.lock()->GetIndex();

		const auto& baseNode = openData->node.lock();
		auto edges = graph->GetEdges(baseNode->GetIndex());	//�G�b�W�̎擾

		openDataList.pop_front();			//�g�p����I�[�v���f�[�^���폜
		closeDataList.push_back(openData);	//�g�p����I�[�v���f�[�^���N���[�Y���X�g�ɓo�^

		for (auto& edge : edges) {
			auto node = graph->GetNode(edge->GetTo());	//�m�[�h�̎擾
			int index = node->GetIndex();
			if (node == startNode) {					//�J�n�m�[�h�Ȃ�continue
				continue;
			}

			auto toNodeVec = node->GetPosition() - baseNode->GetPosition();			//�x�[�X�m�[�h����̎��R�X�g���擾
			auto range = toNodeVec.length();
			range += openData->range;
			auto heuristicRange = m_heuristic->CalculateHeuristicRange(node);		//�q���[�X���b�N�����̎擾

			auto newData = std::make_shared<OpenData>(baseNode, node, range, heuristicRange);	//�V�K�I�[�v���f�[�^�̐���

			auto isResult = AddOpenData(openDataList, closeDataList, newData);		//�I�[�v���f�[�^�̒ǉ�������B
			
			//�I�[�v���f�[�^�̒ǉ��Ɏ��s�����炻�̌�̏��������Ȃ�
			if (!isResult) {
				continue;
			}

			//heuristic������Ȃ�������������^�[�Q�b�g�ɂ��ǂ蒅�������߁A�I���B
			constexpr float NearRange = 0.1f;
			if (newData->heuristic < NearRange) {
				return true;
			}
		}

		std::vector<int> debugIndices;
		for (auto data : openDataList) {
			debugIndices.push_back(data->node.lock()->GetIndex());
		}

		return false;
	}

	void OpenDataHandler::CreateRoute(
		const std::shared_ptr<NavGraphNode>& startNode,
		const std::shared_ptr<NavGraphNode>& targetNode,
		const std::shared_ptr<AstarGraph>& graph,
		const DataPtrList& openDataList)
	{
		auto edges = graph->GetEdges(targetNode->GetIndex());

		auto resultData = std::make_shared<OpenData>(nullptr, nullptr, FLT_MAX, FLT_MAX);
		//�m�[�h�̒��ň�ԋ߂������擾
		for (auto& edge : edges) {
			const auto& node = graph->GetNode(edge->GetTo());

			//�����m�[�h�Ȃ�
			if (node == startNode) {
				return; //�����m�[�h�Ȃ珈������߂�B
			}

			auto someOpenData = FindSomeOpenData(openDataList, node);
			//�m�[�h�����݂��Ȃ��A�܂��́A�f�[�^����A�N�e�B�u�Ȃ�A�������΂��B
			if (!someOpenData || !someOpenData->isActive) {
				continue;
			}

			//��ԏ������f�[�^�����o���B
			if (someOpenData->GetSumRange() < resultData->GetSumRange()) {
				resultData = someOpenData;
			}
		}

		if (resultData->node.lock() == nullptr) {	//���U���g��nullptr�Ȃ�
			m_route.pop();	//��ԃg�b�v�ɍ���base�ɂ����m�[�h�������Ă��邩��
			auto beforeNode = m_route.top();
			//auto beforeData = FindSomeOpenData(openDataList, beforeNode);
			
			CreateRoute(startNode, beforeNode, graph, openDataList);
			return;
		}

		//�����m�[�h�Ȃ�
		if (resultData->node.lock() == startNode) {
			return; //��������߂�B
		}

		m_route.push(resultData->node.lock());
		resultData->isActive = false;

		CreateRoute(startNode, resultData->node.lock(), graph, openDataList);
	}

	std::shared_ptr<OpenData> OpenDataHandler::FindSomeOpenData(const DataPtrList& dataList, const std::shared_ptr<NavGraphNode>& node) {
		return FindSomeOpenData(dataList, std::make_shared<OpenData>(nullptr ,node, 0.0f, 0.0f));
	}

	std::shared_ptr<OpenData> OpenDataHandler::FindSomeOpenData(const DataPtrList& dataList, const std::shared_ptr<OpenData>& openData) {
		auto iter = dataList.begin();
		while (iter != dataList.end()) {
			if (*(*iter) == *openData) {
				return *iter;
			}
			iter++;
		}

		return nullptr;
	}

	bool OpenDataHandler::IsRegisterData(const DataPtrList& dataList, const std::shared_ptr<OpenData>& openData) {
		auto iter = dataList.begin();
		while (iter != dataList.end()) {
			if (*(*iter) == *openData) {
				return true;
			}
			iter++;
		}

		return false;
	}

	bool OpenDataHandler::AddOpenData(DataPtrList& openDataList, DataPtrList& closeDataList, const std::shared_ptr<OpenData>& openData) {
		auto someOpenData = FindSomeOpenData(openDataList, openData);	//�I�[�v���f�[�^���X�g����I�[�v���f�[�^�Ɠ����f�[�^��T���B
		auto someCloseData = FindSomeOpenData(closeDataList, openData);	//�N���[�Y�f�[�^���X�g����I�[�v���f�[�^�Ɠ����f�[�^��T���B

		//�I�[�v�����X�g�ɓo�^����Ă��Ȃ��A���A�N���[�Y���X�g�ɓo�^����Ă��Ȃ��B
		if (!someOpenData && !someCloseData) {
			openDataList.push_back(openData);	//�I�[�v���f�[�^�ɒǉ�
			return true;
		}

		//�I�[�v�����X�g�ɓo�^����Ă��āA�V�K�f�[�^�̕������������v�l
		if (someOpenData && !IsSmall_LeftOpenData(someOpenData, openData)) {
			(*someOpenData) = (*openData);		//�I�[�v���f�[�^�̒��g���X�V
			return true;
		}

		//�N���[�Y���X�g�ɓo�^����Ă��āA�V�K�f�[�^�̕������������v�l
		if (someCloseData && !IsSmall_LeftOpenData(someCloseData, openData)) {
			openDataList.push_back(openData);		//�I�[�v���f�[�^�ɓo�^
			closeDataList.remove(someCloseData);	//�N���[�Y�f�[�^����폜
			return true;
		}

		//�ǂ̏����ɂ����Ă͂܂�Ȃ��Ȃ�A�ǉ������Ȃ��B
		return false;
	}

	bool OpenDataHandler::StartSearchAstar(
		const std::shared_ptr<NavGraphNode>& startNode,
		const std::shared_ptr<NavGraphNode>& targetNode,
		const std::shared_ptr<AstarGraph>& graph
	) {
		//�I�[�v���f�[�^���X�g�ƃN���[�Y�f�[�^���X�g�𐶐�
		auto openDataList = DataPtrList();
		auto closeDataList = DataPtrList();

		//�����I�[�v���f�[�^�𐶐�
		m_heuristic->SetTargetNode(targetNode);
		openDataList.push_back(std::make_shared<OpenData>(nullptr, startNode, 0.0f, m_heuristic->CalculateHeuristicRange(startNode)));

		//�I�[�v���f�[�^�����݂�����胋�[�v����B
		while (openDataList.size() != 0) {	
			//�I�[�v���f�[�^�����p�̊�m�[�h�̐����B
			auto baseOpenData = FindSearchBaseOpenData(openDataList);
			int index = baseOpenData->node.lock()->GetIndex();

			if (targetNode == baseOpenData->node.lock()) {	//�ڕW�m�[�h�Ȃ�
				break;
			}

			//�I�[�v���f�[�^�̐����B�^�[�Q�b�g�m�[�h�ɂ��ǂ蒅������true��Ԃ��B
			if (CreateOpenDatas(openDataList, closeDataList, baseOpenData, graph, startNode, targetNode)) {
				break;
			}
		}

		bool isSearchSuccess = (openDataList.size() != 0);	//�I�[�v���f�[�^�����݂���Ȃ�A��������

		//�I�[�v���f�[�^����ŒZ�o�H���擾
		//m_route.push(targetNode);

		if (!isSearchSuccess) {	//�T�[�`�����s�����珈�����΂��B
			return isSearchSuccess;
		}

		//�N���[�Y�f�[�^���I�[�v���f�[�^�ɓ����B
		for (auto closeData : closeDataList) {
			openDataList.push_back(closeData);
		}

		//�f�o�b�O
		std::vector<int> openIndices = {};
		for (auto data : openDataList) {
			openIndices.push_back(data->node.lock()->GetIndex());
		}

		FindSomeOpenData(openDataList, targetNode)->isActive = false;
		//CreateRoute(startNode, targetNode, graph, closeDataList);
		//CreateRoute(startNode, targetNode, graph, openDataList);

		int index = 0;
		auto tempData = FindSomeOpenData(openDataList, targetNode);
		while (index <= 10000) {
			if (tempData->parent.lock() == nullptr) {
				break;
			}

			m_route.push(tempData->node.lock());
			tempData = FindSomeOpenData(openDataList, tempData->parent.lock());
			index++;
		}

		return isSearchSuccess;
	}

	std::stack<std::shared_ptr<NavGraphNode>> OpenDataHandler::GetRoute() {
		return m_route;
	}



	//auto iter = m_closeDataMap.find(openData.node.lock()->GetIndex());	//�폜�������C�e���[�^�̎擾
	//m_closeDataMap.erase(iter);											//�N���[�Y�}�b�v����폜

}