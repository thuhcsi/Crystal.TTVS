
#ifndef _CST_ANIMATION_PLAYER_H_
#define _CST_ANIMATION_PLAYER_H_

#include "TalkingHead.h"

///
/// @brief  The player for playing the Talking Head animation
///
class CSTHEAD_API AnimationPlayer
{
public:
    ///
    /// @brief  Playing status
    ///
    enum STATUS
    {
        IDLE=0,
        PLAY=1,
        CALL_PAUSE=2,
        PAUSED=3,
        CALL_STOP=4
    };

public:
    AnimationPlayer(TalkingHead *pHead, void* hWnd);    ///< Constructor
    virtual ~AnimationPlayer();                         ///< Destructor

public:
    bool play();        ///< Begin the animation play
    void pause();       ///< Pause the animation play
    void resume();      ///< Resume the animation play
    void stop();        ///< Stop the animation play
    STATUS getStatus(); ///< Return the playing status

protected:
    static unsigned long _stdcall animateProcedure(void *pParam); ///< The procedure for animation thread

protected:
    void playAnimation();               ///< Main procedure for animation
    void dispatchMessage();             ///< Process the message dispatching
    void updateWindow(void *hWnd);      ///< Update the window displaying
    void synchronize(int initFPS=0);    ///< Synchronize the animation player

protected:
    TalkingHead *m_pTalkingHead;    ///< Handle to the talking head module
    void  *m_hWnd;                  ///< Window handle for updating the animation
    int    m_PlayStatus;            ///< Playing status of the player
};

#endif//_CST_ANIMATION_PLAYER_H_
