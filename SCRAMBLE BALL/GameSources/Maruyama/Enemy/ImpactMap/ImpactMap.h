
/*!
@file ImpactMap.h
@brief ImpactMap�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Patch/CommonStructs/RectData.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class GraphAstar;
	class AstarNodeDraw;
	class AstarEdgeDraw;
	class NavGraphNode;
	class I_Impacter;

	struct EyeSearchRangeParametor;

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	Factory_�e���}�b�v�p�p�����[�^
		//--------------------------------------------------------------------------------------
		struct Factory_ImpactMap_Parametor 
		{
			//--------------------------------------------------------------------------------------
			///	�G���A�����̃m�[�h���\����
			//--------------------------------------------------------------------------------------
			struct AreaNodeCount
			{
				int width;	//���̃m�[�h��
				int depth;	//�c�̃m�[�h��

				AreaNodeCount();

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="width">���̃m�[�h��</param>
				/// <param name="depth">�c�̃m�[�h��</param>
				AreaNodeCount(const int width, const int depth);
			};

			//--------------------------------------------------------------------------------------
			///	�����o�[�ϐ�
			//--------------------------------------------------------------------------------------

			Rect rect;						//�͈͂̎l�p�`�f�[�^
			float intervalRange;			//�E�F�C�|�C���g��u���Ԋu
			AreaNodeCount areaNodeCount;	//�G���A�̋�؂�̃m�[�h��
			float createHeight;				//��������ꏊ�̍���

			Factory_ImpactMap_Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="rect">��������l�p�`�͈�</param>
			/// <param name="intervalRange">�E�F�C�|�C���g��u���Ԋu</param>
			Factory_ImpactMap_Parametor(const Rect& rect, const float intervalRange);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="rect">��������l�p�`�͈�</param>
			/// <param name="intervalRange">�E�F�C�|�C���g��u���Ԋu</param>
			/// <param name="areaRangeCount">�G���A��؂�̃m�[�h��</param>
			Factory_ImpactMap_Parametor(const Rect& rect, const float intervalRange, const AreaNodeCount areaNodeCount);
		};

		//--------------------------------------------------------------------------------------
		///	Factory_�e���}�b�v
		//--------------------------------------------------------------------------------------
		class Factory_ImpactMap
		{
		public:
			using Parametor = Factory_ImpactMap_Parametor;

		private:
			Parametor m_param; //�p�����[�^

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="parametor">�p�����[�^</param>
			Factory_ImpactMap(const Parametor& parametor);

		private:

			/// <summary>
			/// �G���A�C���f�b�N�X�̌v�Z
			/// </summary>
			/// <param name="widthCount">���̃J�E���g</param>
			/// <param name="depthCount">�c�̃J�E���g</param>
			/// <returns>�G���A�C���f�b�N�X</returns>
			int CalculateAreaIndex(const int widthCount, const int depthCount, const int numLoopDepth);
			
			/// <summary>
			/// �m�[�h��S�Đ���
			/// </summary>
			/// <param name="astar">�m�[�h�𐶐�����GraphAstar�̃|�C���^</param>
			void CreateNodes(const std::shared_ptr<GraphAstar>& astar);

			/// <summary>
			/// �G�b�W�̐���
			/// </summary>
			/// <param name="astar">�G�b�W�𐶐�����GraphAstar�̃|�C���^</param>
			void CreateEdges(const std::shared_ptr<GraphAstar>& astar);

			/// <summary>
			/// �G���A�G�b�W�̐���(��Č�����)
			/// </summary>
			void CreateAreaEdges(
				const std::shared_ptr<GraphAstar>& astar,
				const int widthCount,
				const int depthCount,
				const int numLoopWidth,
				const int numLoopDepth
			);

		public:
			/// <summary>
			/// �e���}�b�v�p�̃O���t�𐶐�
			/// </summary>
			/// <returns>�e���}�b�v�p�̃O���t</returns>
			std::shared_ptr<GraphAstar> CreateGraphAstar();

			/// <summary>
			/// �e���}�b�v�p�̃O���t�𐶐�
			/// </summary>
			/// <param name="parametor">�����p�p�����[�^</param>
			/// <returns>���������O���t</returns>
			std::shared_ptr<GraphAstar> CreateGraphAstar(const Parametor& parametor);

			/// <summary>
			/// �e���}�b�v�Ƀm�[�h��ǉ�����B
			/// </summary>
			/// <param name="astar">�e���}�b�v�̃O���t�f�[�^</param>
			/// <param name="parametor">�����p�̃p�����[�^</param>
			void AddNodes(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor);

			/// <summary>
			/// ���݂̃m�[�h�ɍ��킹�ĉe���}�b�v�̃G�b�W�𐶐�����B
			/// </summary>
			/// <param name="astar">�e���}�b�v�̃O���t�f�[�^</param>
			/// <param name="parametor">�����p�̃p�����[�^</param>
			void AddEdges(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor);

			/// <summary>
			/// ���ŃG���A�m�[�h����������B
			/// </summary>
			/// <param name="astar">�e���}�b�v�̃O���t�f�[�^</param>
			/// <param name="parametor">�����p�̃p�����[�^</param>
			void AddAreaNodes(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor);

			/// <summary>
			/// ���ŃG���A�G�b�W����������B
			/// </summary>
			/// <param name="astar"></param>
			/// <param name="parametor"></param>
			void AddAreaEdges(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor);

			/// <summary>
			/// �G�b�W�ƃm�[�h�̗����𐶐�����B
			/// </summary>
			/// <param name="astar">�e���}�b�v�̃O���t�f�[�^</param>
			/// <param name="parametor">�����p�p�����[�^</param>
			void AddNodesEdges(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor);

		};

		//--------------------------------------------------------------------------------------
		///	Factory_�e���}�b�v_�t���b�h�t�B���A���S���Y��(���ݍ쐬��) 
		//--------------------------------------------------------------------------------------
		class Factory_ImpactMap_FloodFill
		{
			struct Parametor {
				float intervalRange = 5.0f;  //�m�[�h�̊Ԋu����
				//int numLoopCount = 3;        //�������[�v��
			};

			Parametor m_param;

		public:
			void AddImpactMap(const std::shared_ptr<GraphAstar>& astar, const Parametor& parametor);
		};


		//--------------------------------------------------------------------------------------
		///	�e���}�b�v�{��
		//--------------------------------------------------------------------------------------
		class ImpactMap
		{
		public:
			using Factory_Parametor = Factory_ImpactMap_Parametor;
			using GraphAstarMap = std::unordered_map<int, std::shared_ptr<GraphAstar>>;
			

		private:
			Factory_Parametor m_param = Factory_Parametor();	//�p�����[�^

			std::weak_ptr<Stage> m_stage;						//�����̏�������X�e�[�W�B
			//std::shared_ptr<GraphAstar> m_areaGraphAstar;		//�G���A�Ԃ̃O���t�Ǘ��N���X
			//std::shared_ptr<GraphAstar> m_inAreaGraphAstar;		//�G���A���ׂ̍����O���t�Ǘ��N���X

			std::shared_ptr<GraphAstar> m_baseAstar;			//Astar�Ǘ�������O���t��n���B(��Ԑe�̃O���t)
			//GraphAstarMap m_childAstarMap;						//Astar�̎q�N���X���Ǘ�����N���X�B

			//GraphAstarMap m_astarMap;	//�O���t�̃}�b�v(0���G���A�A1���G���A���A2������ɍׂ����������ꍇ�ȂǂɎg���B)

			//�f�o�b�N�p----------------------------------------------------------------

			ex_weak_ptr<AstarNodeDraw> m_nodeDraw = nullptr;	//�m�[�h�̌�����
			ex_weak_ptr<AstarEdgeDraw> m_edgeDraw = nullptr;	//�G�b�W�̌�����

			//--------------------------------------------------------------------------

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="stage">��������������X�e�[�W�̃|�C���^</param>
			ImpactMap(const std::shared_ptr<Stage>& stage);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="stage">��������������X�e�[�W�̃|�C���^</param>
			/// <param name="parametor">�p�����[�^</param>
			ImpactMap(const std::shared_ptr<Stage>& stage, const Factory_Parametor& parametor);

			/// <summary>
			/// �f�X�g���N�^
			/// </summary>
			~ImpactMap();

		public:

			/// <summary>
			/// �e���}�b�v�f�[�^���쐬����B
			/// </summary>
			void CreateImpactData();

			/// <summary>
			/// �e���}�b�v�f�[�^���쐬����B
			/// </summary>
			/// <param name="parametor">�e���}�b�v�����p�̃p�����[�^</param>
			void CreateImpactData(const Factory_Parametor& parametor);

			/// <summary>
			/// �m�[�h�̕����ǉ�����
			/// </summary>
			/// <param name="parametor">�e���}�b�v�����p�̃p�����[�^</param>
			void AddNodes(const Factory_Parametor& parametor);

			/// <summary>
			/// �G�b�W�̕����ǉ�����
			/// </summary>
			/// <param name="parametor">�e���}�b�v�����p�̃p�����[�^</param>
			void AddEdges(const Factory_Parametor& parametor);

			/// <summary>
			/// �G���A�m�[�h�̐���(���Ŏ����A��čl����)
			/// </summary>
			/// <param name="parametor">�e���}�b�v�����p�̃p�����[�^</param>
			void AddAreaNodes(const Factory_Parametor& parametor);

			/// <summary>
			/// �G���A�G�b�W�̐���(���Ŏ����A��čl����)
			/// </summary>
			/// <param name="parametor">�e���}�b�v�����p�̃p�����[�^</param>
			void AddAreaEdges(const Factory_Parametor& parametor);

			/// <summary>
			/// �m�[�h�ƃG�b�W��ǉ���������B
			/// </summary>
			/// <param name="parametor">�e���}�b�v�����p�̃p�����[�^</param>
			void AddImpactData(const Factory_Parametor& parametor);

			/// <summary>
			/// �m�[�h�̃N���A
			/// </summary>
			void ClearNodes();

			/// <summary>
			/// �G�b�W�̃N���A
			/// </summary>
			void ClearEdges();

			/// <summary>
			/// �����������[�g��S�Ď擾����B
			/// </summary>
			/// <param name="selfPosition">�������g�̃|�W�V����</param>
			/// <param name="targetPosition">�ړI�n</param>
			/// <returns>���������|�W�V�����̔z��</returns>
			std::vector<Vec3> GetRoutePositons(const Vec3& selfPosition, const Vec3& targetPosition);

			/// <summary>
			/// �����������[�g��S�Ď擾����B
			/// </summary>
			/// <param name="selfPosition">�������g�̃|�W�V����</param>
			/// <param name="targetPosition">�ړI�n</param>
			/// <param name="areaIndex">�G���A�C���f�b�N�X</param>
			/// <returns>�����������[�g�̃|�W�V����</returns>
			std::vector<Vec3> GetRoutePositions(
				const Vec3& selfPosition, 
				const Vec3& targetPosition, 
				const int areaIndex,
				const int targetAreaIndex
			);

			std::vector<Vec3> GetRoutePositions(
				const std::shared_ptr<NavGraphNode>& selfNode,
				const Vec3& targetPosition,
				const int areaIndex,
				const int targetAreaIndex
			);

			std::vector<Vec3> ImpactMap::GetRoutePositions(
				const std::shared_ptr<NavGraphNode>& selfNode,
				const std::shared_ptr<NavGraphNode>& targetNode,
				const int areaIndex,
				const int targetAreaIndex
			);

			/// <summary>
			/// �����̎��E���̃m�[�h���擾
			/// </summary>
			/// <param name="selfPosition">�������g�̈ʒu</param>
			/// <param name="impacter">�e����^�����</param>
			/// <returns></returns>
			std::vector<std::shared_ptr<NavGraphNode>> GetEyeRangeNodes(const Vec3& selfPosition, const std::shared_ptr<I_Impacter>& impacter);

			//------------------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//------------------------------------------------------------------------------------------------

			/// <summary>
			/// �����p�̃p�����[�^���Z�b�g
			/// </summary>
			/// <param name="parametor">�����p�̃p�����[�^</param>
			void SetFactoryParametor(const Factory_Parametor& parametor) noexcept;

			/// <summary>
			/// �����p�̃p�����[�^�̎擾
			/// </summary>
			/// <returns></returns>
			Factory_Parametor GetFactoryParametor() const noexcept;

			/// <summary>
			/// Astar�p�̃O���t���擾(�����I�ɍ폜)
			/// </summary>
			/// <returns></returns>
			std::shared_ptr<GraphAstar> GetGraphAstar() const noexcept;

			/// <summary>
			/// �X�e�[�W�̎擾
			/// </summary>
			/// <returns></returns>
			std::shared_ptr<Stage> GetStage() const noexcept { return m_stage.lock(); }


		private:
			//�f�o�b�O�n----------------------------------------------------------------------------------------

		public:
			//�\���̐؂�ւ��Ȃ�
			void DebugInput(const std::function<bool()>& isInput);

			//�f�o�b�O�p��Draw�𐶐�
			void CreateDebugDraw(const bool isDraw = true);

			//�f�o�b�ODraw�̕\���؂�ւ�
			void SetIsDebugDraw(const bool isDraw);

			//�^�[�Q�b�g�ɂ����m�[�h�̐F��ύX����B
			void ChangeTargetNodeColor(const int& nodeIndex, const Col4& color);

		};

	}
}