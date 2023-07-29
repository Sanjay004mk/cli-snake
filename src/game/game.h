#include "../core/attachment.h"
#include "level.h"

namespace Game
{
	class GameManager : public Core::Attachment
	{
	public:
		GameManager();
		~GameManager();

		virtual void OnUpdate(Core::View& view, float delta) override;
		virtual bool OnEvent(Core::Event e) override;

		std::unique_ptr<Level> level;
	private:
	};
}