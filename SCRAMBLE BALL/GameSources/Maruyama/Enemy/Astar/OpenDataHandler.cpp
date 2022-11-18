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

#include "Watanabe/DebugClass/Debug.h"

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

	bool OpenDataHandler::IsOhterAreaTarget(const std::shared_ptr<NavGraphNode>& startNode, const int targetAreaIndex) {
		//�ڕW�G���A��0�ȏ�A���A�ڕW�m�[�h���ڕW�G���A�ƈႤ�Ƃ��B
		return (targetAreaIndex >= 0 && targetAreaIndex != startNode->GetAreaIndex());
	}

	bool OpenDataHandler::CreateOpenDatas(
		DataPtrList& openDataList,
		DataPtrList& closeDataList,
		const std::shared_ptr<OpenData>& openData,
		const std::shared_ptr<AstarGraph>& graph,
		const std::shared_ptr<NavGraphNode>& startNode,
		const std::shared_ptr<NavGraphNode>& targetNode,
		const int targetAreaIndex
	) {
		const auto& baseNode = openData->node.lock();		//��ƂȂ�m�[�h���擾
		auto edges = graph->GetEdges(baseNode->GetIndex());	//�G�b�W�̎擾
		int baseIndex = baseNode->GetIndex();

		openDataList.pop_front();			//�g�p����I�[�v���f�[�^���폜
		closeDataList.push_back(openData);	//�g�p����I�[�v���f�[�^���N���[�Y���X�g�ɓo�^

		std::vector<std::shared_ptr<OpenData>> otherAreaOpenDatas;

		for (auto& edge : edges) {
			auto node = graph->GetNode(edge->GetTo());	//�m�[�h�̎擾
			if (!node) {
				auto strIndex = std::to_wstring(edge->GetTo());
				Debug::GetInstance()->Log(L"OpenDataHandler::CreateOpenData(): �m�[�h��null�ł��B: " + strIndex);
				continue;
			}

			auto toNodeVec = node->GetPosition() - baseNode->GetPosition();			//�x�[�X�m�[�h����̎��R�X�g���擾
			auto range = toNodeVec.length();
			range += openData->range;
			auto heuristicRange = m_heuristic->CalculateHeuristicRange(node);		//�q���[�X���b�N�����̎擾

			int newIndex = node->GetIndex();
			auto newData = std::make_shared<OpenData>(baseNode, node, range, heuristicRange);	//�V�K�I�[�v���f�[�^�̐���

			bool isAddData = AddOpenData(openDataList, closeDataList, newData);		//�I�[�v���f�[�^�̒ǉ�������B

			//�ڕW�m�[�h�Ȃ�I��点��
			if (node == targetNode) {
				return true;
			}

			//�ڕW�G���A�����G���A�ŁA���A�ڕW�G���A�̃m�[�h�Ȃ�otherAreaOpenDatas�ɓo�^����B
			if (IsOhterAreaTarget(startNode, targetAreaIndex) && node->GetAreaIndex() == targetAreaIndex) {
				otherAreaOpenDatas.push_back(newData);
			}
		}

		//�ʃG���A�̃f�[�^�����݂���Ȃ�A�ʃG���A�Ƃ̋��ڂŒT���I��
		if (!otherAreaOpenDatas.empty()) {	
			std::sort(otherAreaOpenDatas.begin(), otherAreaOpenDatas.end(), &IsSmall_LeftOpenData);
			m_otherAreaNode = otherAreaOpenDatas[0]->node.lock();
			return true;
		}

		return false;
	}

	bool OpenDataHandler::CreateRoute(const DataPtrList& openDataList, const std::shared_ptr<NavGraphNode>& targetNode) {
		int index = 0;
		constexpr int tempMaxIndex = 10000;

		//Debug::GetInstance()->Log(L"���[�g------------------------------------------------------");

		auto tempData = FindSomeOpenData(openDataList, targetNode);
		while (index <= tempMaxIndex) {
			if (tempData->parent.lock() == nullptr) {
				break;
			}

			m_route.push(tempData->node.lock());
			tempData = FindSomeOpenData(openDataList, tempData->parent.lock());
			index++;
		}

		//�f�o�b�O
		//auto tempRoute = m_route;
		//while (!tempRoute.empty()) {
		//	auto data = tempRoute.top();
		//	tempRoute.pop();
		//	//debug----------------------------------------------------------------------------
		//	auto areaIndex = std::to_wstring(data.lock()->GetAreaIndex());
		//	auto nodeIndex = std::to_wstring(data.lock()->GetIndex());

		//	Debug::GetInstance()->Log(L"�G���A�F " + areaIndex + L", �m�[�h�F " + nodeIndex);
		//	//---------------------------------------------------------------------------------
		//}

		//Debug::GetInstance()->Log(L"����������������������������������������������");

		return index <= tempMaxIndex;		//����񐔂𒴂�����A���s
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
		const std::shared_ptr<AstarGraph>& graph,
		const int targetAreaIndex
	) {
		maru::Utility::StackClear(m_route);
		m_otherAreaNode.reset();

		//�I�[�v���f�[�^���X�g�ƃN���[�Y�f�[�^���X�g�𐶐�
		auto openDataList = DataPtrList();
		auto closeDataList = DataPtrList();

		//Debug::GetInstance()->Log(startNode->GetIndex());
		//Debug::GetInstance()->Log(targetNode->GetIndex());

		//�����I�[�v���f�[�^�𐶐�
		m_heuristic->SetTargetNode(targetNode);
		openDataList.push_back(std::make_shared<OpenData>(nullptr, startNode, 0.0f, m_heuristic->CalculateHeuristicRange(startNode)));

		//�I�[�v���f�[�^�����݂�����胋�[�v����B
		while (openDataList.size() != 0) {	
			//�I�[�v���f�[�^�����p�̊�m�[�h�̐����B
			auto baseOpenData = FindSearchBaseOpenData(openDataList);

			//�I�[�v���f�[�^�̐����B�^�[�Q�b�g�m�[�h�ɂ��ǂ蒅������true��Ԃ��B
			if (CreateOpenDatas(openDataList, closeDataList, baseOpenData, graph, startNode, targetNode, targetAreaIndex)) {
				break;
			}
		}

		bool isSearchSuccess = (openDataList.size() != 0);	//�I�[�v���f�[�^�����݂���Ȃ�A��������

		if (!isSearchSuccess) {	//�T�[�`�����s�����珈�����΂��B
			return isSearchSuccess;
		}

		//�N���[�Y�f�[�^���I�[�v���f�[�^�Ɋ܂߂�B
		for (auto closeData : closeDataList) {
			openDataList.push_back(closeData);
		}

		//auto lastTargetNode = m_otherAreaNode.lock() ? 
		//	FindSomeOpenData(openDataList, m_otherAreaNode.lock())->parent.lock() : targetNode;	//�ŏI�I�ȃ^�[�Q�b�g�m�[�h
		auto lastTargetNode = m_otherAreaNode.lock() ? m_otherAreaNode.lock() : targetNode;	//�ŏI�I�ȃ^�[�Q�b�g�m�[�h
		bool isCreateRoute = CreateRoute(openDataList, lastTargetNode);
		if (!isCreateRoute) {
			Debug::GetInstance()->Log(L"OpenDataHandler::SearchAstar() : Astar���[�g�������ł��܂���ł����B");
		}

		return isSearchSuccess;
	}

	std::stack<std::weak_ptr<NavGraphNode>> OpenDataHandler::GetRoute() {
		return m_route;
	}



	//auto iter = m_closeDataMap.find(openData.node.lock()->GetIndex());	//�폜�������C�e���[�^�̎擾
	//m_closeDataMap.erase(iter);											//�N���[�Y�}�b�v����폜

}