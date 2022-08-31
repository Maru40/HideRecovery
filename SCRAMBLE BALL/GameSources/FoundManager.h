
/*!
@file FoundManager.h
@brief FoundManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	������ҊǗ�
	//--------------------------------------------------------------------------------------
	class FoundManager : public Component
	{
	public:

		/// <summary>
		/// ������҂̃f�[�^
		/// </summary>
		struct Data
		{
			ex_weak_ptr<GameObject> gameObject;  //�Q�[���I�u�W�F�N�g

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Data();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="gameObject">�Q�[���I�u�W�F�N�g</param>
			Data(const std::shared_ptr<GameObject>& gameObject);
		};

	private:
		Data m_data = Data(); //������҂̃f�[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		FoundManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ������҂̃f�[�^�̎擾
		/// </summary>
		/// <returns>�f�[�^</returns>
		Data GetData() const noexcept { return m_data; }

		/// <summary>
		/// ������҂̃f�[�^�̎Q�Ƃ��擾
		/// </summary>
		/// <returns>������҂̃f�[�^�̎Q��</returns>
		const Data& GetRefData() const{ return m_data; }
	};

}

//endbasecross