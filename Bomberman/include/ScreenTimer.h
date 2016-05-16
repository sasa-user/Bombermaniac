#ifndef SCREENTIMER_H
#define SCREENTIMER_H

/**
 *  \file ScreenTimer.h
 *
 *  Defines class ScreenTimer which is used with SDL.h to calculate elapsed time
 */

/** \class ScreenTimer
 *  \brief class for measuring time
 *  \details This is an utility class for measuring time which can be used for
 *           properly timing events. This class is singleton which means there
 *           can only be one instance of this class.
 */
class ScreenTimer
{
    public:

        /** \brief Method to be used instead of constructor
         *
         * Returns pointer to object ScreenTimer which is of singleton class so
         * it will always return pointer to the same object.
         * \return pointer to singleton ScreenTimer
         */
        static ScreenTimer* Instance();

        /**
         *  \brief Gives time elapsed in milliseconds
         *
         *  Value returned is unsigned long which represents time in
         *  milliseconds since last time the timer was reset or since it was
         *  created.
         *
         *  \return number of milliseconds since last reset
         */
        unsigned long GetTimeElapsed() const;
        /** \brief Pauses the timer
         *
         * Stops measuring the time. Each consecutive call of the
         * GetTimeElapsed() will always give the same time until Unpause() is
         * called. Also IsPaused() will now return true;
         * \return void
         */
        void Pause();
        /** \brief Unpauses the timer
         *
         * Resumes measuring the time if it was paused. Timer will continue to
         * measure the normally in either case.
         * \return void
         */
        void Unpause();
        /** \brief Tells if the timer is paused
         *
         * Return true if timer is currently not measuring time that passes.
         * \return true if the timer is paused and not measuring time.
         */
        bool IsPaused() const;


    protected:

        /**
         *  \brief Make new ScreenTimer and set it to 0.
         *
         *  Basic constructor which makes new ScreenTimer object and sets it's
         *  value to 0.
         */
        ScreenTimer();

    private:
        unsigned long       m_past_ticks;
        unsigned long       m_paused_ticks;
        bool                m_paused;

        static ScreenTimer* s_instance;

        void ResetTimer();
        void DecreaseTimer(unsigned long interval);

};

#endif // SCREENTIMER_H
