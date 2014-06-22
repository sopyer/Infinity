#ifndef __VIDEOPLAYBACKENGINE_H_INCLUDED__
#	define __VIDEOPLAYBACKENGINE_H_INCLUDED__

struct media_player_data_t;

typedef struct media_player_data_t* media_player_t;

void mediaInit();
void mediaShutdown();


media_player_t mediaCreatePlayer(const char* source);
void           mediaDestroyPlayer(media_player_t mediaPlayer);

void mediaStartPlayback(media_player_t mediaPlayer);
void mediaStopPlayback(media_player_t mediaPlayer);
void mediaPausePlayback(media_player_t mediaPlayer);
void mediaSeekPosition(media_player_t mediaPlayer, float time);
void mediaPlayerUpdate(media_player_t mediaPlayer);
void mediaPlayerRender(media_player_t mediaPlayer);
void mediaPlayerPrepareRender(media_player_t player);

#endif