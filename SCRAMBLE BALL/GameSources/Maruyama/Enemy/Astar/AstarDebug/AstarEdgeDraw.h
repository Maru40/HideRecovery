/*!
@file AstarEdgeDraw.h
@brief AstarEdgeDraw
�S���ҁF�ێR �T��
*/

#pragma once

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class GraphAstar;
	class AstarEdge;
	class ArrowObject;

	//--------------------------------------------------------------------------------------
	/// �G�b�W�f�[�^
	//--------------------------------------------------------------------------------------
	struct EdgeData {
		ex_weak_ptr<AstarEdge> astarEdge = nullptr;    //�Ή������G�b�W
		ex_weak_ptr<GameObject> drawObject = nullptr;  //���������G�b�W�I�u�W�F�N�g

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="astarEdge">�Ή������G�b�W</param>
		/// <param name="drawObject">���������G�b�W�I�u�W�F�N�g</param>
		EdgeData(const std::shared_ptr<AstarEdge>& astarEdge, const std::shared_ptr<GameObject>& drawObject);
	};

	//--------------------------------------------------------------------------------------
	/// Astar�G�b�W�\��
	//--------------------------------------------------------------------------------------
	class AstarEdgeDraw : public Component {

		std::weak_ptr<const GraphAstar::GraphType> m_astar;   //GraphAstar�̃|�C���^
		std::vector<EdgeData> m_edgeDatas; //���������G�b�W�f�[�^

		/// <summary>
		/// �G�b�W�𐶐�����ʒu���v�Z���ĕԂ�
		/// </summary>
		/// <param name="startPosition">�m�[�h�̊J�n�ʒu</param>
		/// <param name="endPosition">�m�[�h�̏I���ʒu</param>
		/// <returns>�G�b�W�𐶐�����ʒu</returns>
		Vec3 CalculateCreatePosition(const Vec3& startPosition, const Vec3& endPosition) const;

		/// <summary>
		/// �G�b�W�̑傫�����v�Z���ĕԂ�
		/// </summary>
		/// <param name="startPosition">�m�[�h�̊J�n�ʒu</param>
		/// <param name="endPosition">�m�[�h�̏I���ʒu</param>
		/// <returns>�G�b�W��傫��</returns>
		Vec3 CalculateCreateScale(const Vec3& startPosition, const Vec3& endPosition) const;

		/// <summary>
		/// �n�����I�u�W�F�N�g�̈ʒu�A�����A�傫�����Z�b�e�B���O����B
		/// </summary>
		/// <param name="object">�ݒ肵�����I�u�W�F�N�g</param>
		/// <param name="startPosition">�m�[�h�̊J�n�ʒu</param>
		/// <param name="endPosition">�m�[�h�̏I���ʒu</param>
		/// <returns>�G�b�W��傫��</returns>
		void SettingDrawPositionRotateScale(const std::shared_ptr<GameObject>& object, const Vec3& startPosition, const Vec3& endPosition);

		/// <summary>
		/// �G�b�W�f�[�^�̐���
		/// </summary>
		/// <param name="object">�����������G�b�W</param>
		/// <param name="startPosition">�m�[�h�̊J�n�ʒu</param>
		/// <param name="endPosition">�m�[�h�̏I���ʒu</param>
		/// <returns>�G�b�W�f�[�^</returns>
		EdgeData CreateEdgeData(const std::shared_ptr<AstarEdge>& edge, const Vec3& startPosition, const Vec3& endPosition);

		/// <summary>
		/// ArrowObject�̐���
		/// </summary>
		/// <param name="position">�����������ꏊ</param>
		/// <param name="quat">��������������</param>
		/// <returns>��������ArrowObject�̃|�C���^</returns>
		std::shared_ptr<ArrowObject> InstantiateArrowObject(const Vec3& position, const Quat& quat);

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="astar">Astar�O���t</param>
		AstarEdgeDraw(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<const GraphAstar::GraphType>& astar);

		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

	private:
		/// <summary>
		/// �ʒu�̍X�V
		/// </summary>
		void UpdatePosition();

		/// <summary>
		/// �����Ŏw�肵���G�b�W�ƈꏏ�̃I�u�W�F�N�g���擾����B
		/// </summary>
		/// <param name="edge">���������G�b�W</param>
		std::shared_ptr<GameObject> FindEdgeDrawObject(const std::shared_ptr<AstarEdge>& edge);

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �f�o�b�O�\�������邩�ǂ����ݒ�
		/// </summary>
		/// <param name="isDraw">�f�o�b�O�\�����邩�ǂ���</param>
		void SetDebugDraw(const bool isDraw);

		/// <summary>
		/// �f�o�b�O�\����Ԃ̎擾
		/// </summary>
		/// <returns>�f�o�b�O�\�����</returns>
		bool GetDebugDraw() const;

	};

}

//endbasecross