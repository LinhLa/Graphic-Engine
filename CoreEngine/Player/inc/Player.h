#pragma once
#include "IPlayerComponent.h"
#include "Focus.h"
#include "Node2DImage.h"
#include "NodeText2D.h"
#include "EmptyNode.h"

#define PLAY_BUTTON				"play_button"
#define PLAY_BUTTON_BACKGROUND	"playbutton_background"
#define PLAYER_BACKGROUND		"player_background"
#define SLIDER					"slider"
#define SCRUBBING_BAR			"scrubbing_bar"
#define ELAPSED_TIME			"elapsed_time"
#define SPEAKER_SYMBOL			"speaker"
#define FULL_SIZE_SYMBOL		"fullsize"

#define HOURS					"hh"
#define MINUTES					"mm"
#define SECONDS					"ss"
#define MSECONS					"ms"

#define LAND_SCAPE				"landscape"
#define BACK_GROUND 			"background"

SDL_Rect toSDLRect(glm::vec3 transform, glm::vec2 size);

class Player final: public creator<Player>, public std::enable_shared_from_this<Player>, public Component<IPlayerComponent>
{
private:
	const std::string VIDEO_PLAYER = "VIDEO_PLAYER";
	const std::string FADE_IN_PROGRESS = "FADE_IN_PROGRESS";

	FocusPtr			m_pFocus = nullptr;
	EmptyNodePtr		m_pPlayer = nullptr;

	void AcquireResource();
	void PrepareAnimation();
	void PrepareKeyCommand();
	void PrepareKeyMap();
protected:
	Player();
public:
	~Player();
	friend class creator<Player>;

	void setVisible(bool visible);

	EmptyNodePtr	getPlayer();

	void init(VoidType&&);
	void onKeyInputEvent(SDL_Event&& arg);

	FocusPtr getFocus();
};
typedef std::shared_ptr<Player> PlayerPtr;