#include "player.hpp"

#include "../Log/log.hpp"

Player::Player()
{
    Log(LogLevel::Debug)<<"Enter media player initialization";
    player=new QMediaPlayer();
    output=new QAudioOutput();
    player->setAudioOutput(output);
    //TODO: replace this and other hardcoded settings with user defined settings
    player->setSource(QUrl::fromLocalFile(""));
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
