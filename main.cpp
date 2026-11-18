#include <iostream>
#include <cstdlib>
#include "bowling_frame.h"

using namespace std;


// Adds together all frame scores for one player
int current_game_total(Bowling_Frame * first_frame)
{
    Bowling_Frame * temp = first_frame;
    int game_total = 0;

    while (temp != nullptr)
    {
        game_total += temp->frame_score();
        temp = temp->next_frame();
    }

    return game_total;
}


int main(int argc, char * argv[])
{
    // Check that the number of players was passed in
    if (argc != 2)
    {
        cout << "Usage: ./a.out <num_players>" << endl;

        // Return 0 so Jenkins continues to the next run
        return 0;
    }


    // Convert command-line argument into an integer
    int num_players = atoi(argv[1]);


    // Make sure the number of players is valid
    if (num_players <= 0)
    {
        cout << "Number of players must be greater than 0." << endl;
        return 0;
    }


    cout << "Player count is: " << num_players << endl;
    cout << endl;


    // =====================================================
    // CREATE ARRAY OF BOWLING_FRAME POINTERS
    // =====================================================

    Bowling_Frame ** players =
        new Bowling_Frame*[num_players];


    // =====================================================
    // CREATE 10 FRAMES FOR EACH PLAYER
    // =====================================================

    for (int i = 0; i < num_players; i++)
    {
        // First frame for this player
        players[i] = new Bowling_Frame(1);

        Bowling_Frame * current = players[i];


        // Create frames 2 through 10
        for (int frame_num = 2;
             frame_num <= 10;
             frame_num++)
        {
            Bowling_Frame * new_frame =
                new Bowling_Frame(frame_num);


            // Link current frame to next frame
            current->next_frame(new_frame);


            // Link new frame back to previous frame
            new_frame->prev_frame(current);


            // Move current forward
            current = new_frame;
        }
    }


    // =====================================================
    // GENERATE SCORES FOR EVERY PLAYER
    // =====================================================

    for (int i = 0; i < num_players; i++)
    {
        Bowling_Frame * current = players[i];


        while (current != nullptr)
        {
            // First roll
            current->update_scores();


            // Second roll
            current->update_scores();


            // Frame 10 can have one extra roll
            // if it contains a strike or spare
            if (current->frame_num() == 10)
            {
                if (current->strike() ||
                    current->spare())
                {
                    current->update_scores();
                }
            }


            current = current->next_frame();
        }
    }


    // =====================================================
    // UPDATE STRIKE AND SPARE BONUS SCORES
    // =====================================================

    // All frames now have their rolls.
    // Go through them again so strike/spare bonuses
    // can use scores from later frames.
    for (int i = 0; i < num_players; i++)
    {
        Bowling_Frame * current = players[i];


        while (current != nullptr)
        {
            current->update_scores();

            current = current->next_frame();
        }
    }


    // =====================================================
    // PRINT EACH PLAYER'S GAME
    // =====================================================

    for (int i = 0; i < num_players; i++)
    {
        cout << "Player " << i + 1 << ":" << endl;
        cout << endl;


        Bowling_Frame * current = players[i];


        while (current != nullptr)
        {
            cout << "Frame "
                 << current->frame_num()
                 << ":" << endl;

            cout << "  Score 1: "
                 << current->score1()
                 << endl;

            cout << "  Score 2: "
                 << current->score2()
                 << endl;

            cout << "  Frame Score: "
                 << current->frame_score()
                 << endl;

            cout << endl;


            current = current->next_frame();
        }


        cout << "Total Game Score for player "
             << i + 1
             << ": "
             << current_game_total(players[i])
             << endl;

        cout << endl;
    }


    // =====================================================
    // DELETE ALL FRAMES
    // =====================================================

    for (int i = 0; i < num_players; i++)
    {
        Bowling_Frame * current = players[i];


        while (current != nullptr)
        {
            Bowling_Frame * next =
                current->next_frame();


            delete current;


            current = next;
        }
    }


    // Delete array of player pointers
    delete [] players;


    return 0;
}
