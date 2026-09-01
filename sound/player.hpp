#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QMediaPlayer>
#include <QAudioOutput>

class Player
{
    public:
        Player();

        static Player* instance();
        void ring();
        void stop();
		void playOnce();
		void file(const char* path);
		void volume(float volume);
        const bool playing(){return player->playbackState()==QMediaPlayer::PlayingState;};

        ~Player();

        Player(const Player&)=delete;
        Player operator=(const Player&)=delete;
        Player(Player&&)=delete;
        Player operator=(Player&&)=delete;
    private:
        inline static Player *instance_=nullptr;
        QMediaPlayer *player=nullptr;
        QAudioOutput *output=nullptr;
};

#endif // PLAYER_HPP
