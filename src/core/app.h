#pragma once

#include "terminal.h"
#include "input.h"
#include "attachment.h"
#include <vector>
#include <span>
#include <memory>

namespace Core
{
	struct CommandLineArgs
	{
		CommandLineArgs(int32_t argc, char** argv) : argc(argc), argv(argv) {  }

		char* operator[](int32_t i) { return argv[i]; }

		int32_t argc;
		char** argv;
	};

	class Application
	{
	public:
		Application();
		~Application();

		void Run();
		void Close();

		void AddAttachment(std::shared_ptr<Attachment> attachment);

		static Application& Get() { return *s_Instance; }

	private:
		void Init();
		void OnEvent(Event e);
		static Application* s_Instance;

		bool m_Running = true;
		Core::Terminal m_Terminal;
		float m_Delta = 0.f;
		std::vector<std::shared_ptr<Attachment>> m_Attachments;

		friend Application* CreateApplication(const CommandLineArgs& args);
	};
	
	Core::Application* CreateApplication(const CommandLineArgs& args);
}
