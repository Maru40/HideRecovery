
/*!
@file RandomPositionGenerator.h
@brief RandomPositionGenerator�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "GeneratorBase.h"
#include "Maruyama/Utility/Random.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �����_���ʒu�����p�����[�^
	//--------------------------------------------------------------------------------------
	struct RandomPositionGenerator_Parametor
	{
		int numCreate;        //������
		Vec3 centerPosition;  //�����ʒu�̒��S
		Vec3 createRangeVec;  //��������͈�

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		RandomPositionGenerator_Parametor();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="numCreate">������</param>
		RandomPositionGenerator_Parametor(const int& numCreate);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="numCreate">������</param>
		/// <param name="centerPosition">�����ʒu�̒��S</param>
		/// <param name="createRangeVec">�����͈͂̃x�N�g��</param>
		RandomPositionGenerator_Parametor(const int& numCreate,
			const Vec3& centerPosition, const Vec3& createRangeVec);
	};

	//--------------------------------------------------------------------------------------
	/// �����_���ʒu�����W�F�l���[�^
	//--------------------------------------------------------------------------------------
	template<class Type>
	class RandomPositionGenerator : public GeneratorBase
	{
	public:
		using Parametor = RandomPositionGenerator_Parametor;

	private:
		Parametor m_param;  //�p�����[�^

		/// <summary>
		/// ��������ꏊ�������_���Ɏ擾
		/// </summary>
		/// <returns>��������ꏊ</returns>
		Vec3 CalculatePosition() {
			auto randomPosition = maru::MyRandom::RandomPosition(m_param.createRangeVec);
			return m_param.centerPosition + randomPosition;
		}

		/// <summary>
		/// �I�u�W�F�N�g�̐���
		/// </summary>
		/// <param name="position">�����ʒu</param>
		/// <returns>���������I�u�W�F�N�g</returns>
		std::shared_ptr<GameObject> CreateObject(const Vec3& position) {
			auto object = GetStage()->AddGameObject<Type>();
			auto objTrans = object->GetComponent<Transform>();
			objTrans->SetPosition(position);
			return object;
		}

		/// <summary>
		/// �p�����[�^�̐��������A�I�u�W�F�N�g�𐶐�����
		/// </summary>
		void CreateObjects() {
			for (int i = 0; i < m_param.numCreate; i++) {
				auto object = CreateObject(CalculatePosition());
				m_objects.push_back(object);
			}
		}

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		RandomPositionGenerator(const std::shared_ptr<GameObject>& objPtr)
			:RandomPositionGenerator(objPtr, Parametor())
		{}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="parametor">�p�����[�^</param>
		RandomPositionGenerator(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor)
			:GeneratorBase(objPtr), m_param(parametor)
		{}

		void OnCreate() override {
			CreateObjects();
		}

	};

}