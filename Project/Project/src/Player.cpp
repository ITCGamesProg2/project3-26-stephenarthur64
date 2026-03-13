#include "Player.h"
#include <iostream>

Player::Player(Color t_c, float t_r) : GameObject(t_c, t_r), MAX_SPEED(6.0f), MIN_SPEED(0.01f), m_light(m_position), m_heavy(m_position), m_special(m_position), 
										DEFAULT_FRICTION(0.94f), m_comboCurrent(0), COMBO_MAX(3), COMBO_MAX_COOLDOWN(2.0f), m_comboCountdown(0.0f), m_momentum(100.0f),
										m_maxMomentum(100.0f)
{
	m_speed = 0.4f;
	m_baseRadius = t_r;
	m_maxHealth = 10;
	m_health = m_maxHealth;
	m_maxIFrames = 0.3f;
	m_friction = DEFAULT_FRICTION;
	CollisionCheck::setPlayerReference(this);

	m_combo.push_back(&m_light);
	m_combo.push_back(&m_light);
	m_combo.push_back(&m_heavy);
	/*m_combo.push_back(&m_special);*/

	m_spriteScale = 1.0f;

	Timeline::setSpriteInfo(m_spriteScale, &m_sprite);

	m_abilities[TimeAbilities::REWIND] = true;
	m_abilities[TimeAbilities::STOP] = true;
	m_abilities[TimeAbilities::SKIP] = true;
}

void Player::update()
{
	GameObject::update();

	if (m_comboCountdown < COMBO_MAX_COOLDOWN)
	{
		m_comboCountdown += GetFrameTime();
	}

	if (m_comboCurrent == COMBO_MAX)
	{
		m_combo.back()->execute(GetMousePosition());
		m_comboCurrent = 0;
	}
	
	if (m_special.isRunning())
	{
		decreaseMomentum();
		m_radius = m_special.getRadius();
	}
	if (!m_special.isRunning())
	{
		m_friction = DEFAULT_FRICTION;
		m_radius = m_baseRadius;
	}

	if (m_light.isCollided())
	{
		addMomentum(5.0f);
	}
	if (m_heavy.isCollided())
	{
		addMomentum(10.0f);
	}
	if (m_special.isCollided())
	{
		addMomentum(5.0f);
	}

	move();

	m_light.setStart(m_position);
	m_light.process();

	m_heavy.setStart(m_position);
	m_heavy.process();

	m_special.setStart(m_position);
	m_special.process();
}

void Player::move()
{
	m_position += m_velocity;
	m_velocity *= m_friction;
	if (m_velocity.x > 0)
	{
		m_animation.flipSprite(1);
	}
	else
	{
		m_animation.flipSprite(-1);
	}
}

void Player::draw()
{
	GameObject::draw();

	m_light.draw();
	m_heavy.draw();
	m_special.draw();
}

void Player::collision(int t_damage, Vector2 t_pos)
{
	applyKnockback(t_pos);

	if (t_damage > 0)
	{
		damage(t_damage);
	}
}

void Player::damage(int t_amount)
{
	GameObject::damage(t_amount);
	std::cout << m_health << "\n";

	if (m_invincibilityFrames <= 0.0f)
	{
		m_momentum -= t_amount * 2;
		if (m_momentum < 0)
		{
			m_momentum = 0;
		}
		m_invincibilityFrames = m_maxIFrames;
	}
}

void Player::respawn()
{
	GameObject::respawn();
}

void Player::addForce(Vector2 t_direction)
{
	m_velocity += t_direction * m_speed;
	if (m_velocity.x > MAX_SPEED)
	{
		m_velocity.x = MAX_SPEED;
	}
	if (m_velocity.y > MAX_SPEED)
	{
		m_velocity.y = MAX_SPEED;
	}
}

void Player::addMomentum(float t_amount)
{
	if (m_momentum + t_amount <= m_maxMomentum)
	{
		m_momentum += t_amount;
	}
	else
	{
		m_momentum = m_maxMomentum;
	}
}

void Player::decreaseMomentum(float t_amount)
{
	m_momentum -= t_amount;
}

void Player::setMousePosition(Camera2D& t_cam)
{
	m_mouse = GetMousePosition();
	m_mouse = GetScreenToWorld2D(m_mouse, t_cam);
}

Attack* Player::getAttack(AttackTypes t_type)
{
	switch (t_type)
	{
	case LIGHT:
		return &m_light;
		break;
	case HEAVY:
		return &m_heavy;
		break;
	case SPECIAL:
		return &m_special;
		break;
	default:
		break;
	}
}

bool Player::rewind()
{
	if (m_momentum > 0.0f)
	{
		m_newTime = Timeline::rewind();

		if (m_newTime.position.x != 0.0f)
		{
			m_velocity = m_newTime.velocity;
			m_position = m_newTime.position;
			m_health = m_newTime.health;
			m_momentum -= 1.0f;
			return true;
		}
	}
	return false;
}

Time Player::generateTime()
{
	m_newTime.position = m_position;
	m_newTime.spriteRect = m_animation.getFrame();
	m_newTime.velocity = m_velocity;
	m_newTime.health = m_health;

	return m_newTime;
}

void Player::useAttack(AttackTypes t_attack)
{
	switch (t_attack)
	{
	case LIGHT:
		if (m_light.canAttack())
		{
			comboCheck(t_attack);
		}
		m_light.execute(m_mouse);
		break;
	case HEAVY:
		if (m_heavy.canAttack())
		{
			comboCheck(t_attack);
		}
		m_heavy.execute(m_mouse);
		break;
	case SPECIAL:
		m_comboCurrent = 0;
		m_special.execute(m_mouse);
		m_friction = 0.99f;
		break;
	default:
		break;
	}
}

void Player::comboCheck(AttackTypes t_attack)
{
	if (m_comboCurrent < m_combo.size() - 1 && m_comboCountdown < COMBO_MAX_COOLDOWN)
	{
		if (m_combo.at(m_comboCurrent)->getType() == t_attack)
		{
			m_comboCurrent++;
			m_comboCountdown = 0.0f;
		}
		else
		{
			m_comboCurrent = 0;
		}
	}
	else
	{
		m_comboCountdown = 0.0f;
		m_comboCurrent = 0;
	}
}

void Player::addToCombo(Attack* t_attack)
{
	m_combo.push_back(t_attack);
}

void Player::newAbility(TimeAbilities t_a)
{
	m_abilities[t_a] = true;
}

bool Player::canUse(TimeAbilities t_a)
{
	return m_abilities[t_a];
}
