#ifndef AUDIO_H
#define AUDIO_H

#include <stdlib.h>

#include <vorbis/vorbisfile.h>

#if RETRO_PLATFORM != RETRO_VITA && RETRO_PLATFORM != RETRO_OSX
#include "SDL.h"
#endif

#if RETRO_USING_SDL2

extern SDL_AudioDeviceID audioDevice;
extern SDL_AudioStream *ogv_stream;

#define LockAudioDevice()   do { if (audioDevice != 0) SDL_LockAudioDevice(audioDevice); } while (0)
#define UnlockAudioDevice() do { if (audioDevice != 0) SDL_UnlockAudioDevice(audioDevice); } while (0)

#elif RETRO_USING_SDL1

#define LockAudioDevice()   SDL_LockAudio()
#define UnlockAudioDevice() SDL_UnlockAudio()

#else

#define LockAudioDevice()   ;
#define UnlockAudioDevice() ;

#endif

#define TRACK_COUNT   (0x10)
#define SFX_COUNT     (0x100)
#define CHANNEL_COUNT (0x4)
#define SFXDATA_COUNT (0x400000)

#define MAX_VOLUME (100)

#define STREAMFILE_COUNT (2)

#define MIX_BUFFER_SAMPLES (256)

struct TrackInfo {
    char fileName[0x40];
    bool trackLoop;
    uint loopPoint;
};

struct StreamInfo {
    OggVorbis_File vorbisFile;
    int vorbBitstream;
#if RETRO_USING_SDL1
    SDL_AudioSpec spec;
#endif
#if RETRO_USING_SDL2
    SDL_AudioStream *stream;
#endif
    Sint16 buffer[MIX_BUFFER_SAMPLES];
    bool trackLoop;
    uint loopPoint;
    bool loaded;
};

struct SFXInfo {
    char name[0x40];
    Sint16 *buffer;
    size_t length;
    bool loaded;
};

struct ChannelInfo {
    size_t sampleLength;
    Sint16 *samplePtr;
    int sfxID;
    byte loopSFX;
    sbyte pan;
};

struct StreamFile {
    byte *buffer;
    int fileSize;
    int filePos;
};

enum MusicStatuses {
    MUSIC_STOPPED = 0,
    MUSIC_PLAYING = 1,
    MUSIC_PAUSED  = 2,
    MUSIC_LOADING = 3,
    MUSIC_READY   = 4,
};

extern int globalSFXCount;
extern int stageSFXCount;

extern int masterVolume;
extern int trackID;
extern int sfxVolume;
extern int bgmVolume;
extern bool audioEnabled;

extern int nextChannelPos;
extern bool musicEnabled;
extern int musicStatus;
extern TrackInfo musicTracks[TRACK_COUNT];
extern SFXInfo sfxList[SFX_COUNT];

extern ChannelInfo sfxChannels[CHANNEL_COUNT];

extern int currentStreamIndex;
extern StreamFile streamFile[STREAMFILE_COUNT];
extern StreamInfo streamInfo[STREAMFILE_COUNT];
extern StreamFile *streamFilePtr;
extern StreamInfo *streamInfoPtr;

#if RETRO_USING_SDL1 || RETRO_USING_SDL2
extern SDL_AudioSpec audioDeviceFormat;
#endif

int InitAudioPlayback();
void LoadGlobalSfx();

#if RETRO_USING_SDL1 || RETRO_USING_SDL2
void ProcessMusicStream(void *data, Sint16 *stream, int len);
void ProcessAudioPlayback(void *data, Uint8 *stream, int len);
void ProcessAudioMixing(Sint32 *dst, const Sint16 *src, int len, int volume, sbyte pan);

inline void FreeMusInfo(int slot)
{
    if (streamInfoPtr == &streamInfo[slot]) {
        streamInfoPtr = NULL;
        streamFilePtr = NULL;
    }

#if RETRO_USING_SDL2
    if (streamInfo[slot].stream)
        SDL_FreeAudioStream(streamInfo[slot].stream);
#endif

    if (streamInfo[slot].loaded)
        ov_clear(&streamInfo[slot].vorbisFile);

#if RETRO_USING_SDL2
    streamInfo[slot].stream = nullptr;
#endif

    if (streamFile[slot].buffer)
        free(streamFile[slot].buffer);

    streamFile[slot].buffer = NULL;
    streamFile[slot].fileSize = 0;
    streamFile[slot].filePos = 0;
    streamInfo[slot].loaded = false;
}

inline void FreeMusInfo()
{
    FreeMusInfo(currentStreamIndex);
}
#else
void ProcessMusicStream() {}
void ProcessAudioPlayback() {}
void ProcessAudioMixing() {}

inline void FreeMusInfo() { ov_clear(&streamInfo[currentStreamIndex].vorbisFile); }
#endif

#if RETRO_USE_MOD_LOADER
extern char globalSfxNames[SFX_COUNT][0x40];
extern char stageSfxNames[SFX_COUNT][0x40];
void SetSfxName(const char *sfxName, int sfxID, bool global);
#endif

void LoadMusic();
void SetMusicTrack(char *filePath, byte trackID, bool loop, uint loopPoint);
bool PlayMusic(int track);
inline void StopMusic()
{
    LockAudioDevice();
    musicStatus = MUSIC_STOPPED;

#if RETRO_USING_SDL1 || RETRO_USING_SDL2
    for (int slot = 0; slot < STREAMFILE_COUNT; ++slot)
        FreeMusInfo(slot);
#else
    FreeMusInfo();
#endif

    UnlockAudioDevice();
}

void LoadSfx(char *filePath, byte sfxID);
void PlaySfx(int sfx, bool loop);
inline void StopSfx(int sfx)
{
    if (sfx < 0 || sfx >= SFX_COUNT)
        return;

    LockAudioDevice();

    for (int i = 0; i < CHANNEL_COUNT; ++i) {
        if (sfxChannels[i].sfxID == sfx) {
            MEM_ZERO(sfxChannels[i]);
            sfxChannels[i].sfxID = -1;
        }
    }

    UnlockAudioDevice();
}
void SetSfxAttributes(int sfx, int loopCount, sbyte pan);

inline void SetMusicVolume(int volume)
{
    if (volume < 0)
        volume = 0;
    if (volume > MAX_VOLUME)
        volume = MAX_VOLUME;
    masterVolume = volume;
}

inline bool PauseSound()
{
    if (musicStatus == MUSIC_PLAYING) {
        musicStatus = MUSIC_PAUSED;
        return true;
    }
    return false;
}

inline void ResumeSound()
{
    if (musicStatus == MUSIC_PAUSED)
        musicStatus = MUSIC_PLAYING;
}

inline void StopAllSfxUnlocked()
{
    for (int i = 0; i < CHANNEL_COUNT; ++i) {
        MEM_ZERO(sfxChannels[i]);
        sfxChannels[i].sfxID = -1;
    }
}

inline void StopAllSfx()
{
    LockAudioDevice();
    StopAllSfxUnlocked();
    UnlockAudioDevice();
}

inline void ReleaseGlobalSfxUnlocked()
{
    StopAllSfxUnlocked();

    for (int i = globalSFXCount - 1; i >= 0; --i) {
        if (sfxList[i].loaded) {
            StrCopy(sfxList[i].name, "");
            free(sfxList[i].buffer);
            sfxList[i].buffer = nullptr;
            sfxList[i].length = 0;
            sfxList[i].loaded = false;
        }
    }

    globalSFXCount = 0;
}

inline void ReleaseGlobalSfx()
{
    LockAudioDevice();

    ReleaseGlobalSfxUnlocked();

    UnlockAudioDevice();
}

inline void ReleaseStageSfxUnlocked()
{
    for (int i = stageSFXCount + globalSFXCount - 1; i >= globalSFXCount; --i) {
        if (sfxList[i].loaded) {
            StrCopy(sfxList[i].name, "");
            free(sfxList[i].buffer);
            sfxList[i].buffer = nullptr;
            sfxList[i].length = 0;
            sfxList[i].loaded = false;
        }
    }
    stageSFXCount = 0;
}

inline void ReleaseStageSfx()
{
    LockAudioDevice();

    StopAllSfxUnlocked();
    ReleaseStageSfxUnlocked();

    UnlockAudioDevice();
}

inline void ReleaseAudioDevice()
{
#if RETRO_USING_SDL2
    if (audioDevice != 0)
        SDL_PauseAudioDevice(audioDevice, 1);
#elif RETRO_USING_SDL1
    SDL_PauseAudio(1);
#endif

    StopMusic();

    LockAudioDevice();

    StopAllSfxUnlocked();
    ReleaseStageSfxUnlocked();
    ReleaseGlobalSfxUnlocked();

    UnlockAudioDevice();

#if RETRO_USING_SDL2
    if (audioDevice != 0) {
        SDL_CloseAudioDevice(audioDevice);
        audioDevice = 0;
    }

    if (ogv_stream) {
        SDL_FreeAudioStream(ogv_stream);
        ogv_stream = nullptr;
    }
#endif
}

#endif // !AUDIO_H