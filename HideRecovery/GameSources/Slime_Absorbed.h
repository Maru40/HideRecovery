/*!
@file Slime_Absorbed.h
@brief Slime_Absorbed�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "EnemyStateNodeBase.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// �z�����ꂽ�X�e�[�g�̃p�����[�^
			//--------------------------------------------------------------------------------------
			struct Slime_Absorbed_Parametor 
			{
				std::weak_ptr<GameObject> parent;  //�������z����������B
			};

			//--------------------------------------------------------------------------------------
			/// �z�����ꂽ�X�e�[�g
			//--------------------------------------------------------------------------------------
			class Slime_Absorbed : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = Slime_Absorbed_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr; //�p�����[�^

			public:
				/// <summary>
				/// �z�����ꂽ�X�e�[�g
				/// </summary>
				/// <param name="objPtr">���̃N���X�����L����N���X</param>
				/// <param name="numCreate">�p�����[�^</param>
				Slime_Absorbed(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;

				//--------------------------------------------------------------------------------------
				/// �A�N�Z�b�T
				//--------------------------------------------------------------------------------------

				/// <summary>
				/// �������z������������擾����B
				/// </summary>
				/// <returns>�������z����������</returns>
				std::shared_ptr<GameObject> GetParent() const;
			};

		}
	}
}