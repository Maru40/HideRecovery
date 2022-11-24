/*!
@file UtilityAstar.h
@brief UtilityAstar�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "NavGraphNode.h"
#include "AstarEdge.h"

#include "GraphAstar.h"

#include "Maruyama/Utility/UtilityObstacle.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class GraphAstar;

	//--------------------------------------------------------------------------------------
	///	Astar�ł悭�g���v�Z���܂Ƃ߂��N���X
	/// GraphAstar���]��ɂ��֐��������������߁A�v�Z�n�𕪂��邽�߂ɐ���
	//--------------------------------------------------------------------------------------
	class UtilityAstar
	{
	public:

		/// <summary>
		/// �^�[�Q�b�g�����ԋ߂��̃m�[�h����������B
		/// </summary>
		/// <param name="astar">Astar�̎Q��</param>
		/// <param name="target">�^�[�Q�b�g</param>
		/// <returns>�^�[�Q�b�g�̈�ԋ߂��̃m�[�h</returns>
		static std::shared_ptr<NavGraphNode> SearchNearNode(const GraphAstar& astar, const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// �^�[�Q�b�g�����ԋ߂��̃m�[�h����������B
		/// </summary>
		/// <param name="astar">Astar�̎Q��</param>
		/// <param name="targetPos">�^�[�Q�b�g�̃|�W�V����</param>
		/// <param name="excluteObjs">Ray�̑ΏۊO�ɂ���I�u�W�F�N�g�Q</param>
		/// <returns>�^�[�Q�b�g�����ԋ߂��m�[�h</returns>
		static std::shared_ptr<NavGraphNode> SearchNearNode(const GraphAstar& astar, const Vec3& targetPos);

		/// <summary>
		/// �^�[�Q�b�g�����ԋ߂��̃m�[�h����������B
		/// </summary>
		/// <param name="graph">�O���t�̎Q��</param>
		/// <param name="targetPos">�^�[�Q�b�g�̃|�W�V����</param>
		/// <param name="isObstacleConfirmation">��Q��������s�����ǂ���</param> 
		/// <returns>�^�[�Q�b�g�����ԋ߂��m�[�h</returns>
		static std::shared_ptr<NavGraphNode> SearchNearNode(
			const std::shared_ptr<const GraphAstar::GraphType>& graph, 
			const Vec3& targetPos, 
			const bool isObstacleConfirmation = true
		);

		/// <summary>
		/// �^�[�Q�b�g�����ԋ߂��̃m�[�h����������B
		/// </summary>
		/// <param name="graph">�O���t�̎Q��</param>
		/// <param name="targetPos">�^�[�Q�b�g�̃|�W�V����</param>
		/// <param name="isObstacleConfirmation">��Q��������s�����ǂ���</param> 
		/// <returns>�^�[�Q�b�g�����ԋ߂��m�[�h</returns>
		static std::shared_ptr<NavGraphNode> SearchNearNode(
			const std::shared_ptr<const GraphAstar::GraphType>& graph,
			const Vec3& targetPos,
			const std::vector<std::shared_ptr<GameObject>>& obstacleObjects,
			const std::vector<std::shared_ptr<GameObject>>& excludeObstacleObject,
			const bool isObstacleConfirmation = true
		);

		/// <summary>
		/// �^�[�Q�b�g�����ԋ߂��̃m�[�h����������B
		/// </summary>
		/// <param name="graph">�O���t�̎Q��</param>
		/// <param name="targetPos">�^�[�Q�b�g�̃|�W�V����</param>
		/// <param name="isObstacleConfirmation">��Q��������s�����ǂ���</param> 
		/// <returns>�^�[�Q�b�g�����ԋ߂��m�[�h</returns>
		static std::shared_ptr<NavGraphNode> SearchNearNode(
			const std::shared_ptr<const GraphAstar::GraphType>& graph,
			const std::shared_ptr<GameObject>& targetObject,
			const std::vector<std::shared_ptr<GameObject>>& obstacleObjects,
			const std::vector<std::shared_ptr<GameObject>>& excludeObstacleObject = {},
			const bool isObstacleConfirmation = true
		);

		/// <summary>
		/// �����̃m�[�h����L�т�m�[�h�ň�ԃ^�[�Q�b�g�ɋ߂��m�[�h�̑I��
		/// </summary>
		/// <param name="astar">Astar�̎Q��</param>
		/// <param name="selfObject">�������g</param>
		/// <param name="target">�^�[�Q�b�g</param>
		/// <returns>��ԋ߂��m�[�h</returns>
		static std::shared_ptr<NavGraphNode> SearchMyNodeToTargetNearNode(
			const GraphAstar& astar,
			const std::shared_ptr<GameObject>& selfObject,
			const std::shared_ptr<GameObject>& target
		);

		/// <summary>
		/// �����̃m�[�h����^�[�Q�b�g�����ɂ���m�[�h������
		/// </summary>
		/// <param name="astar">Astar�̎Q��</param>
		/// <param name="startNode">�J�n�m�[�h</param>
		/// <param name="targetPos">�^�[�Q�b�g�̃|�W�V����</param>
		/// <returns>�^�[�Q�b�g�����̃m�[�h</returns>
		static std::shared_ptr<NavGraphNode> CalculateTargetDirectNode(const GraphAstar& astar,
			const std::shared_ptr<NavGraphNode>& startNode,
			const Vec3& targetPos
		);

		/// <summary>
		/// �^�[�Q�b�g���瓦���邽�߂̕����̃m�[�h����������
		/// </summary>
		/// <param name="astar">Astar�̎Q��</param>
		/// <param name="startNode">�J�n�m�[�h</param>
		/// <param name="targetPos">�^�[�Q�b�g�̃|�W�V����</param>
		/// <returns>�^�[�Q�b�g�����̃m�[�h</returns>
		static std::shared_ptr<NavGraphNode> CalculateTargetEscapeDirectNode(const GraphAstar& astar,
			const std::shared_ptr<NavGraphNode>& startNode,
			const Vec3& targetPos
		);

		/// <summary>
		/// ���i��ōł��߂��m�[�h���ǂ���
		/// </summary>
		/// <param name="graph">�O���t�N���X</param>
		/// <param name="toVec">�ړI�̕���</param>
		/// <param name="newNode">�V�K�m�[�h</param>
		/// <param name="compareNode">��r�Ώۂ̃m�[�h</param>
		/// <returns>�ł��߂��Ȃ�true</returns>
		template<class NodeType, class EdgeType>
		static bool IsNearNode(const std::shared_ptr<SparseGraph<NodeType, EdgeType>>& graph,
			Vec3 toVec,
			const std::shared_ptr<NodeType>& newNode,
			const std::shared_ptr<NodeType>& compareNode
		)
		{
			auto nodes = graph->GetNodes();
			for (const auto& node : nodes) {
				if (node == newNode || node == compareNode) { //�����Ȃ珈�������Ȃ�
					continue;
				}

				auto newToVec = node->GetPosition() - newNode->GetPosition();

				const float NearAngle = XMConvertToRadians(10.0f);
				auto newDot = dot(toVec.GetNormalized(), newToVec.GetNormalized());
				auto newRad = (acosf(newDot));

				//�قړ����������A�Z���Ȃ�
				if (newRad < NearAngle && toVec.length() > newToVec.length()) {
					return false;
				}
			}

			return true;
		}

		/// <summary>
		/// �V�K�m�[�h����G�b�W�̐���
		/// </summary>
		/// <param name="graph">�O���t�N���X</param>
		/// <param name="newNode">�V�K�m�[�h</param>
		/// <param name="isRayHit">���C�̊m�F�����邩�ǂ���</param>
		/// <returns>���������G�b�W�Q</returns>
		template<class NodeType, class EdgeType>
		static vector<std::shared_ptr<EdgeType>> CreateAdjacendEdges(
			const std::shared_ptr<GraphAstar::GraphType>& graph,
			const std::shared_ptr<NodeType>& newNode,
			const bool isRayHit = true
		) 
		{
			auto nodes = graph->GetNodes();
			vector<std::shared_ptr<EdgeType>> resultEdge;

			//�������ɋ߂��I�u�W�F�N�g������Ȃ珈�������Ȃ�
			constexpr float NearRange = 2.0f;
			Vec3 positions[] = {
				newNode->GetPosition() + ( Vec3::Forward() * NearRange),
				newNode->GetPosition() + (-Vec3::Forward() * NearRange),
				newNode->GetPosition() + ( Vec3::Right() * NearRange),
				newNode->GetPosition() + (-Vec3::Right() * NearRange),
			};

			for (auto& position : positions) {
				auto objects = maru::Utility::GetStage()->GetGameObjectVec();
				if (maru::UtilityObstacle::IsRayObstacle(newNode->GetPosition(), position, objects)) {
					return resultEdge;
				}
			}

			for (const auto& node : nodes) {
				if (node == newNode) {  //�����Ȃ珈�������Ȃ�
					continue;
				}

				auto toVec = node->GetPosition() - newNode->GetPosition();
				if (!IsNearNode<NodeType, EdgeType>(graph, toVec, newNode, node)) { //�߂��m�[�h�łȂ�������
					continue;
				}

				//��Q���ɓ������Ă��Ȃ�������
				auto objects = maru::Utility::GetStage()->GetGameObjectVec();
				if( !isRayHit ||
					(isRayHit && !maru::UtilityObstacle::IsRayObstacle(newNode->GetPosition(), node->GetPosition(), objects))
				) {
					auto fromEdge = std::make_shared<EdgeType>(newNode->GetIndex(), node->GetIndex());

					graph->AddEdge(fromEdge);
					resultEdge.push_back(fromEdge);
				}
			}

			return resultEdge;
		}

		/// <summary>
		/// node�̃C���f�b�N�X����אڂ���G�b�W�̐�������B
		/// ��������ꍇ�ɒ��ړn���ꂽSparseGraph��Add����B
		/// </summary>
		/// <param name="graph">Graph�̎Q��</param>
		/// <param name="newNode">�V�K�m�[�h</param>
		/// <param name="obstacleObjs">��Q���̃I�u�W�F�N�g�z��</param>
		/// <param name="excluteObjs">��Q������Ȃ��I�u�W�F�N�g�z��</param>
		/// <returns>�������ꂽ�G�b�W�̔z��</returns>
		template<class NodeClass, class EdgeClass>
		static vector<std::shared_ptr<AstarEdge>> CreateAdjacendEdges(std::shared_ptr<GraphAstar::GraphType>& graph, const std::shared_ptr<NavGraphNode>& newNode,
			const vector<shared_ptr<GameObject>>& obstacleObjs, const vector<shared_ptr<GameObject>>& excluteObjs)
		{
			//�g��Ȃ��B
			auto nodes = graph->GetNodes();

			vector<std::shared_ptr<AstarEdge>> reEdges;
			for (auto& node : nodes) {

				//��Q�����Ȃ�������G�b�W��ǉ�����B
				if (!maru::Utility::IsRayObstacle(newNode->GetPosition(), node->GetPosition(), obstacleObjs, excluteObjs)) {
					//�o�����ɃG�b�W�𐶐�
					graph->AddEdge(std::make_shared<AstarEdge>(newNode->GetIndex(), node->GetIndex()));
					graph->AddEdge(std::make_shared<AstarEdge>(node->GetIndex(), newNode->GetIndex()));
					//�ǂ̂悤�ȃG�b�W�𐶐��������Ԃ���悤�ɕʂ̔z��ɓ����B
					reEdges.push_back(std::make_shared<AstarEdge>(newNode->GetIndex(), node->GetIndex()));
					reEdges.push_back(std::make_shared<AstarEdge>(node->GetIndex(), newNode->GetIndex()));
				}
			}

			return reEdges;
		}

	};

}