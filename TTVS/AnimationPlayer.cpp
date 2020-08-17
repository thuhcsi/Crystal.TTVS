
#include <Windows.h>
#ifndef WINCE
#include <process.h>
#endif
#include <time.h>
#include "TalkingHead.h"
#include "AnimationPlayer.h"

AnimationPlayer::AnimationPlayer(TalkingHead *pHead, void* hWnd)
{
    m_pTalkingHead = pHead;
    m_hWnd         = hWnd;
    m_PlayStatus   = IDLE;
}

AnimationPlayer::~AnimationPlayer()
{
    m_pTalkingHead = NULL;
    m_hWnd         = NULL;
    m_PlayStatus   = IDLE;
}

bool AnimationPlayer::play()
{
    if (m_PlayStatus != IDLE)
        return false;

    // create a thread to play
    m_PlayStatus = PLAY;
    DWORD threadID;
    CreateThread(NULL, 0, animateProcedure, this, 0, &threadID);
    return true;
}

void AnimationPlayer::pause()
{
    // set pause flag
    if (m_PlayStatus & PLAY)
        m_PlayStatus |= CALL_PAUSE;
}

void AnimationPlayer::resume()
{
    // clear pause flag
    m_PlayStatus &= ~CALL_PAUSE;
}

void AnimationPlayer::stop()
{
    if (m_PlayStatus != IDLE)
    {
        // clear pause flag
        // set call for stop flag
        m_PlayStatus |= CALL_STOP;
        m_PlayStatus &= ~CALL_PAUSE;
    }
}

AnimationPlayer::STATUS AnimationPlayer::getStatus()
{
    return (STATUS)(m_PlayStatus & ~CALL_STOP);
}

unsigned long _stdcall AnimationPlayer::animateProcedure(void *pParam)
{
    AnimationPlayer *pPlayer = (AnimationPlayer*)pParam;
    if (pPlayer)
        pPlayer->playAnimation();
    return 0;
}

void AnimationPlayer::playAnimation()
{
    // record start time
    synchronize(m_pTalkingHead->getFPS());

    // start loop
    m_pTalkingHead->rewind();
    while (!m_pTalkingHead->isEnd())
    {
        // call for stop?
        if (m_PlayStatus & CALL_STOP)
            break;

        // update the talking head with current frame
        m_pTalkingHead->update();

        // display the new head
        updateWindow(m_hWnd);

        // process message
        dispatchMessage();

        // synchronize playback
        synchronize(0);

        // call for pause
        while (m_PlayStatus & CALL_PAUSE)
        {
            synchronize(0);
        }

        // advance to next frame
        m_pTalkingHead->next();
    }

    // call for stop?
    if (m_PlayStatus & CALL_STOP)
    {
        // reset the animation, (mouth to close status)
        m_pTalkingHead->rewind();
        m_pTalkingHead->update();
        updateWindow(m_hWnd);
        dispatchMessage();
    }

    // reset status
    m_PlayStatus = IDLE;
}

void AnimationPlayer::updateWindow(void *hWnd)
{
    SendMessage((HWND)hWnd, WM_PAINT, 0, 0);
}

void AnimationPlayer::dispatchMessage()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void AnimationPlayer::synchronize(int initFPS)
{
    static DWORD lastTime  = 0;
    static DWORD frameTime = 1;

    // bStart means it is beginning playback now, record information
    if (initFPS != 0)
    {
        lastTime  = GetTickCount();
        frameTime = 1000 / initFPS;
    }

    // synchronize. if fast, slow down
    while (GetTickCount() < lastTime + frameTime)
    {
        dispatchMessage();
        Sleep(frameTime / 4);
    }

    // record the new time stamp
    lastTime += frameTime;
}
