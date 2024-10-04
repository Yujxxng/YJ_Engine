#pragma once

#include "BaseLevel.h"
namespace Levels
{
	class Test : public GSM::BaseLevel
	{
	public:
		virtual ~Test() override {};

		void Init() override;
		void Update() override;
		void Exit() override;
	};
}
