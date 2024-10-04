#pragma once
#include <iostream>

namespace GSM
{
	class BaseLevel
	{
	public:
		virtual ~BaseLevel() {
		};
		//Init
		virtual void Init() = 0;
		//Update
		virtual void Update() = 0;
		//Exit
		virtual void Exit() = 0;
	};
}