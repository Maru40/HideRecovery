
/*!
@file StateNodeBase.h
@brief StateNodeBase
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

    //--------------------------------------------------------------------------------------
    ///	�X�e�[�g�m�[�h�̃C���^�[�t�F�[�X
    //--------------------------------------------------------------------------------------
    class I_StateNode
    {
    public:
        /// <summary>
        /// �J�n���ɌĂяo������
        /// </summary>
        virtual void OnStart() = 0;

        /// <summary>
        /// ���t���[���Ăяo������(�O���Ăяo��)
        /// </summary>
        /// <returns>����������������true</returns>
        virtual bool OnUpdate() = 0;

        /// <summary>
        /// �I�����ɌĂяo������
        /// </summary>
        virtual void OnExit() = 0;
    };

    //--------------------------------------------------------------------------------------
    ///	�O���錾
    //--------------------------------------------------------------------------------------
    template<class NodeType>
    class StateNodeBase;

    //--------------------------------------------------------------------------------------
    ///	�X�e�[�g�m�[�h�̊��N���X
    //--------------------------------------------------------------------------------------
	template<class NodeType>
	class StateNodeBase : public I_StateNode
	{
        std::weak_ptr<NodeType> m_owner;  //���L�҂̃|�C���^

	public:
 
        /// <summary>
        /// �R���X�g���N�^
        /// </summary>
        /// <param name="owner">���L��</param>
        StateNodeBase(const std::shared_ptr<NodeType>& owner)
            :m_owner(owner)
        {}

        virtual ~StateNodeBase() {}

    protected:

        /// <summary>
        /// ���L�҂̎擾
        /// </summary>
        /// <returns>���L��</returns>
        std::shared_ptr<NodeType> GetOwner() const {
            auto owner = m_owner.lock();
            return owner ? owner : nullptr;
        }

	};

}

//endbasecross