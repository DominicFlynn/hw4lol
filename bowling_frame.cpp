#include "bowling_frame.h"
#include <random>

std::random_device dev;
std::mt19937 rng(dev());

Bowling_Frame::Bowling_Frame(int fnum)
{
    score1_ = 0;
    score2_ = 0;
    score3_ = 0;

    strike_ = false;
    spare_ = false;

    roll_one_ = false;
    roll_two_ = false;
    roll_three_ = false;

    total_score_ = 0;
    frame_num_ = fnum;
    frame_score_ = 0;

    next_frame_ = nullptr;
    prev_frame_ = nullptr;
}


Bowling_Frame::~Bowling_Frame()
{
    // Frames will be deleted in main.
    // Do not free next_frame_ or prev_frame_ here.
}


Bowling_Frame* Bowling_Frame::next_frame()
{
    return next_frame_;
}

void Bowling_Frame::next_frame(Bowling_Frame * new_frame)
{
    next_frame_ = new_frame;
}


Bowling_Frame* Bowling_Frame::prev_frame()
{
    return prev_frame_;
}

void Bowling_Frame::prev_frame(Bowling_Frame * new_frame)
{
    prev_frame_ = new_frame;
}


int Bowling_Frame::score1()
{
    return score1_;
}

void Bowling_Frame::score1(int score)
{
    score1_ = score;
}


int Bowling_Frame::score2()
{
    return score2_;
}

void Bowling_Frame::score2(int score)
{
    score2_ = score;
}


bool Bowling_Frame::strike()
{
    return strike_;
}

void Bowling_Frame::strike(bool value)
{
    strike_ = value;
}


bool Bowling_Frame::spare()
{
    return spare_;
}

void Bowling_Frame::spare(bool value)
{
    spare_ = value;
}


int Bowling_Frame::frame_num()
{
    return frame_num_;
}


int Bowling_Frame::frame_score()
{
    return frame_score_;
}


void Bowling_Frame::update_scores()
{
    // =====================================================
    // FRAMES 1 THROUGH 9
    // =====================================================

    if (frame_num_ < 10)
    {
        // First roll
        if (!roll_one_)
        {
            std::uniform_int_distribution<int> dist10(0, 10);

            score1_ = dist10(rng);
            roll_one_ = true;

            if (score1_ == 10)
            {
                strike_ = true;
                roll_two_ = true;
            }
        }

        // Second roll
        else if (!roll_two_)
        {
            std::uniform_int_distribution<int> distX(
                0, 10 - score1_
            );

            score2_ = distX(rng);
            roll_two_ = true;

            if (score1_ + score2_ == 10)
            {
                spare_ = true;
            }
        }
    }


    // =====================================================
    // FRAME 10
    // =====================================================

    else if (frame_num_ == 10)
    {
        // First roll
        if (!roll_one_)
        {
            std::uniform_int_distribution<int> dist10(0, 10);

            score1_ = dist10(rng);
            roll_one_ = true;

            if (score1_ == 10)
            {
                strike_ = true;
            }
        }

        // Second roll
        else if (!roll_two_)
        {
            if (score1_ == 10)
            {
                // Pins reset after a strike
                std::uniform_int_distribution<int> dist10(0, 10);

                score2_ = dist10(rng);
            }
            else
            {
                std::uniform_int_distribution<int> distX(
                    0, 10 - score1_
                );

                score2_ = distX(rng);
            }

            roll_two_ = true;

            if (score1_ != 10 &&
                score1_ + score2_ == 10)
            {
                spare_ = true;
            }
        }

        // Third roll is only allowed after a
        // strike or spare in frame 10
        else if (!roll_three_ && (strike_ || spare_))
        {
            if (strike_)
            {
                // If roll 2 was also a strike,
                // pins reset again
                if (score2_ == 10)
                {
                    std::uniform_int_distribution<int> dist10(
                        0, 10
                    );

                    score3_ = dist10(rng);
                }
                else
                {
                    std::uniform_int_distribution<int> distX(
                        0, 10 - score2_
                    );

                    score3_ = distX(rng);
                }
            }
            else
            {
                // Spare means pins reset
                std::uniform_int_distribution<int> dist10(
                    0, 10
                );

                score3_ = dist10(rng);
            }

            roll_three_ = true;
        }
    }


    // =====================================================
    // CALCULATE FRAME SCORE
    // =====================================================

    // Frame 10 includes its bonus roll directly
    if (frame_num_ == 10)
    {
        frame_score_ = score1_ + score2_ + score3_;
        return;
    }


    // Normal open frame
    frame_score_ = score1_ + score2_;


    // =====================================================
    // SPARE BONUS
    // =====================================================

    if (spare_ && next_frame_ != nullptr)
    {
        frame_score_ = 10 + next_frame_->score1_;
    }


    // =====================================================
    // STRIKE BONUS
    // =====================================================

    if (strike_ && next_frame_ != nullptr)
    {
        frame_score_ = 10;

        // If next frame is also a strike
        if (next_frame_->strike_)
        {
            frame_score_ += next_frame_->score1_;

            // Frame 9 followed by a strike in frame 10
            if (next_frame_->frame_num_ == 10)
            {
                frame_score_ += next_frame_->score2_;
            }

            // Consecutive strikes before frame 10
            else if (next_frame_->next_frame_ != nullptr)
            {
                frame_score_ +=
                    next_frame_->next_frame_->score1_;
            }
        }

        // Next frame is not a strike
        else
        {
            frame_score_ += next_frame_->score1_;
            frame_score_ += next_frame_->score2_;
        }
    }
}
