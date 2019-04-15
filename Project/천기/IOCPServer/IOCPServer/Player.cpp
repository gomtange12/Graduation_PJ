#include "pch.h"
#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}
void Player::Initialize() {
	m_join = false;
	m_connected = false;
	m_x=4;
	m_y=4;
}