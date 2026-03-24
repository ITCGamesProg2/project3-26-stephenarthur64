#include "Boss.h"

Boss::Boss() : NPC(RED, 80.0F), m_canDamage(false)
{
	m_animation.setValues(5, 64, 64, 0.2f);
	m_spriteScale = 2.0f;
}

void Boss::move()
{
}

void Boss::update()
{
	NPC::update();
	if (m_alive)
	{
		m_upgrade.setPosition(m_position);
	}
}

void Boss::damage(int t_amount)
{
	if (m_canDamage)
	{
		GameObject::damage(t_amount);
	}
}

void Boss::setSprite(Texture2D* t_sp)
{
	m_sprite = t_sp;

	m_upgrade.setSprite();
}

void Boss::immuneCheck(Vector2 t_playerPos)
{
	if (m_target == t_playerPos)
	{
		m_canDamage = false;
	}
	else
	{
		m_canDamage = true;
	}
}
