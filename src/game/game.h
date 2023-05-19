#include "../core/attachment.h"

namespace Game
{
	class GameManager : public Core::Attachment
	{
	public:
		virtual void OnUpdate(Core::View& view, float delta) override;
		virtual bool OnEvent(Core::Event e) override;

	private:
	};
}