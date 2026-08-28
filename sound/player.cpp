#include "player.hpp"

#include "../Log/log.hpp"

Player::Player()
{
    Log(LogLevel::Debug)<<"Enter media player initialization";
    player=new QMediaPlayer();
    output=new QAudioOutput();
    player->setAudioOutput(output);
    output->setVolume(1.0);
    Log(LogLevel::Debug)<<"Exit media player initialization";
}

void Player::ring()
{
    //loop media for ring
    player->setLoops(QMediaPlayer::Infinite);
    player->play();
}

void Player::stop()
{
    player->stop();
    //reset loop setting
    player->setLoops(QMediaPlayer::Once);
}

//TODO:
//think about load all sound files on init and play from memory
void Player::file(const char* path)
{
	player->setSource(QUrl::fromLocalFile(path));
}

void Player::volume(float volume)
{
	output->setVolume(volume);
}

void Player::playOnce()
{
	player->setLoops(QMediaPlayer::Once);
	player->play();
}

Player* Player::instance()
{
    if(instance_==nullptr)
        instance_=new Player();
    return instance_;
}

Player::~Player()
{
    Log(LogLevel::Debug)<<"Enter media player destructor";
    if(player!=nullptr)
        delete player;
    if(output!=nullptr)
        delete output;
    Log(LogLevel::Debug)<<"Exit media player destructor";
}
