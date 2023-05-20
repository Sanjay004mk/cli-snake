#pragma once

namespace Core
{
	class Time
	{
	public:
		static float GetTime() { return s_UpTime; }
		static float GetTimeMilli() { return s_UpTime * 1e3f; }
		static void Reset() { s_UpTime = 0.f; }
		static float GetDeltaTime() { return s_Delta; }

	private:
		static void OnUpdate(float delta)
		{
			s_UpTime += (s_Delta = delta);
		}

		static inline float s_UpTime = 0.f;
		static inline float s_Delta = 0.f;

		friend class Application;
	};
}