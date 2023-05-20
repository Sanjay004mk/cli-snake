#include "app.h"

#include <chrono>
#include "time.h"

#include "input.h"
#include "view.h"

namespace Core
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
		Init();
	}


	Application::~Application()
	{
		s_Instance = nullptr;
	}

	void Application::Run()
	{
		std::chrono::steady_clock::time_point last, now = std::chrono::high_resolution_clock::now();
		while (m_Running)
		{
			Time::OnUpdate(m_Delta);

			last = now;

			m_Terminal.OnUpdate(m_Delta);

			Core::View& view = m_Terminal.PrepareView();

			for (auto& attachment : m_Attachments)
				attachment->OnUpdate(view, m_Delta);

			m_Terminal.FlushView();

			Core::Input::PollEvents(m_Delta);

			now = std::chrono::high_resolution_clock::now();
			m_Delta = std::min(0.33f, std::chrono::duration_cast<std::chrono::microseconds>(now - last).count() * 1e-6f);
			
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::AddAttachment(std::shared_ptr<Attachment> attachment)
	{
		m_Attachments.push_back(attachment);
	}

	void Application::Init()
	{
		Core::Input::Init();
		Core::Input::SetEventCallbackFn([this](Core::Event e) { this->OnEvent(e); });
	}

	void Application::OnEvent(Event e)
	{
		// default behavior
		// falls on the attachment to decide when the application closes ( using Application::Close() )
		if (e == Core::Event::Escape && m_Attachments.empty())
			m_Running = false;

		for (auto& attachment : m_Attachments)
			if (attachment->OnEvent(e))
				return;
	}
}