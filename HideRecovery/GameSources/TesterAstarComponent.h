/*!
@file TesterAstarComponent.h
@brief TesterAstarComponent
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "I_Impact.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class GameTimer;

	namespace maru {
		class ImpactMap;
	}

	namespace Tester {

		//--------------------------------------------------------------------------------------
		///	AI�̉e���҃p�����[�^
		//--------------------------------------------------------------------------------------
		struct AIImpacter_Parametor {
			float occupancyUpdateIntervalTime;	//��L�l�X�V�C���^�[�o������(�}���`�X���b�h�ɂ���܂ł̉��}���u)

			AIImpacter_Parametor();
		};

		//--------------------------------------------------------------------------------------
		///	AI�̉e���҃R���|�\�l���g
		//--------------------------------------------------------------------------------------
		class TesterAstarComponent : public Component, public maru::I_Impacter
		{
		public:
			using Parametor = AIImpacter_Parametor;

		private:
			Parametor m_param;								//�p�����[�^

			std::unique_ptr<GameTimer> m_occupancyTimer;	//��L�l�X�V�^�C�}�[

		public:
			TesterAstarComponent(const std::shared_ptr<GameObject>& objPtr);

			void OnUpdate() override;

		private:
			void UpdateEyeRangeImpactMap();

		public:
			//--------------------------------------------------------------------------------------
			///	�C���^�[�t�F�[�X�̎���
			//--------------------------------------------------------------------------------------

			std::shared_ptr<GameObject> GetImpacterObject() const noexcept override;

			std::shared_ptr<EyeSearchRange> GetEyeSearchRange() const override;

			std::shared_ptr<maru::ImpactMap> GetImpactMap() const override ;
		};

	}
}