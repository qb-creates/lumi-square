#include "poweronstate.h"
#include "ledmatrix.h"

PowerOnState::PowerOnState()
    : GameBaseState(GameState::PowerOn),
      m_animationTransitionTime(64),
      m_nextStateDelayTime(200),
      m_animationTimer(0),
      m_nextStateDelayTimer(0),
      m_reverseAnimation(false),
      m_animationComplete(false),
      m_currentLedIndex(0),
      m_buttonIndexList{0, 1, 2, 3, 7, 11, 15, 14, 13, 12, 8, 4, 5, 6, 10, 9},
      m_ledColorList{
          Colors::pink,
          Colors::azure,
          Colors::cyan,
          Colors::purple,
          Colors::aquamarine,
          Colors::red,
          Colors::orange,
          Colors::yellow,
          Colors::pink,
          Colors::azure,
          Colors::cyan,
          Colors::purple,
          Colors::aquamarine,
          Colors::red,
          Colors::orange,
          Colors::yellow}
{
}

void PowerOnState::enterState(GameState previousState)
{
    GameBaseState::enterState(previousState);
    
    for (int i = 0; i < 16; ++i)
    {
        LEDMatrix::ledOff(m_buttonIndexList[i], m_ledColorList[i], 1);
    }

    AudioSource::Instance().playNoteSequence(POWER_UP_AUDIO_CLIP);
}

void PowerOnState::exitState() {}

void PowerOnState::updateState()
{
    if (!m_animationComplete)
    {
        m_animationTimer += DeviceUtility::DELTA_TIME;

        if (m_animationTimer >= m_animationTransitionTime)
        {
            m_animationTimer = 0;

            m_reverseAnimation ? LEDMatrix::ledOff(m_buttonIndexList[m_currentLedIndex]) : LEDMatrix::ledOn(m_buttonIndexList[m_currentLedIndex]);

            ++m_currentLedIndex;

            if (m_currentLedIndex == 16 && !m_reverseAnimation)
            {
                m_reverseAnimation = true;
                m_currentLedIndex = 0;
            }
            else if (m_currentLedIndex == 16 && m_reverseAnimation)
            {
                m_animationComplete = true;
            }
        }
    }
    else
    {
        m_nextStateDelayTimer += DeviceUtility::DELTA_TIME;

        if (m_nextStateDelayTimer >= m_nextStateDelayTime)
        {
            nextState = GameState::Menu;
        }

        return;
    }
}

void PowerOnState::onButtonPressed(int8_t buttonIndex)
{
}
