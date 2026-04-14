#include "Pickup.h"

Pickup::Pickup() : m_radius(25.0f), m_alive(true), m_frame(0)
{
}

void Pickup::draw()
{
	if (m_alive)
	{
		DrawCircle(m_position.x, m_position.y, m_radius, YELLOW);
		DrawTexturePro(m_sprite, { 32.0f * m_frame, 0, 32, 32 }, { m_position.x - (m_radius), m_position.y - (m_radius), m_radius * 3.0f, m_radius * 3.0f}, {16, 16}, 0.0f, WHITE);
	}
}

void Pickup::drawPopup()
{
	m_popup.draw();
}

void Pickup::setPosition(Vector2 t_pos)
{
	m_position = t_pos;
}

void Pickup::setAbility(TimeAbilities t_a)
{
	m_ability = t_a;

	switch (m_ability)
	{
	case REWIND:
		m_frame = 0;
		m_popup.setTutorial("rewind");
		break;
	case SKIP:
		m_frame = 1;
		m_popup.setTutorial("skip");
		break;
	case STOP:
		m_frame = 2;
		m_popup.setTutorial("stop");
	default:
		break;
	}
}

void Pickup::deactivate()
{
	m_alive = false;
	PlaySound(AssetManager::getSound("powerup"));
	m_popup.spawn({ 600.0f, 200.0f }, { 50.0f, 50.0f });
}
