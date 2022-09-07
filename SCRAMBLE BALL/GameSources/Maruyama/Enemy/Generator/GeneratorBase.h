
/*!
@file GeneratorBase.h
@brief GeneratorBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �W�F�l���[�^�̊��N���X
	//--------------------------------------------------------------------------------------
	class GeneratorBase : public Component
	{
	protected:
		std::vector<ex_weak_ptr<GameObject>> m_objects; //���������I�u�W�F�N�g�̔z��

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		GeneratorBase(const std::shared_ptr<GameObject>& objPtr);

		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ���������I�u�W�F�N�g���̎擾
		/// </summary>
		/// <returns></returns>
		virtual int GetNumObject() const;

		/// <summary>
		/// ���������I�u�W�F�N�g�̔z����擾
		/// </summary>
		/// <returns></returns>
		std::vector<ex_weak_ptr<GameObject>> GetObjects() const;

		/// <summary>
		/// �I�u�W�F�N�g�̒ǉ�
		/// </summary>
		/// <param name="object"></param>
		void AddObject(const std::shared_ptr<GameObject>& object) {
			m_objects.push_back(object);
		}
	};

}

//endbasecross