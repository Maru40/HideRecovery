
/*!
@file TimeHelper.h
@brief TimeHelper�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�v�����ԃ^�C�v
		//--------------------------------------------------------------------------------------
		enum class DeltaType {
			Normal,  //�ʏ�
			Real,    //���A���^�C��
		};

		//--------------------------------------------------------------------------------------
		///	���ԊǗ��w���p�[
		//--------------------------------------------------------------------------------------
		class TimeHelper 
		{
		public:
			/// <summary>
			/// �o�ߎ��Ԃ̎擾
			/// </summary>
			/// <param name="type">�v�����ԃ^�C�v</param>
			/// <returns>�o�ߎ���</returns>
			static float GetElapsedTime(const DeltaType& type);

		};

	}
}

//endbasecross