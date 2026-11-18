#include <iostream>
#include <cstdlib>
#include "bowling_frame.h"

using namespace std;


// Adds the score from every frame in one player's game
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
    // ==========================================
    // CHECK COMMAND-LINE ARGUMENT
    // ==========================================

    if (argc != 2)
    {
        cout << "Usage: ./a.out <num_players>" << endl;
        return 1;
    }

    int num_players = atoi(argv[1]);

    if (num_players <= 0)
    {
        cout << "Number of players must be greater than 0." << endl;
        return 1;
    }

    cout << "Player count is: " << num_players << endl;


    // ==========================================
    // CREATE ARRAY OF PLAYER FRAME POINTERS
    // ==========================================

    Bowling_Frame ** players =
        new Bowling_Frame*[num_players];


    // ==========================================
    // CREATE 10 FRAMES FOR EACH PLAYER
    // ==========================================

    for (int i = 0; i < num_players; i++)
    {
        // First frame for this player
        players[i] = new Bowling_Frame(1);

        Bowling_Frame * current = players[i];

        // Create frames 2 through 10
        for (int frame_num = 2; frame_num <= 10; frame_num++)
        {
            Bowling_Frame * new_frame =
                new Bowling_Frame(frame_num);

            // Link current frame forward
            current->next_frame(new_frame);

            // Link new frame backward
            new_frame->prev_frame(current);

            // Move forward
            current = new_frame;
        }
    }


    // ==========================================
    // POPULATE SCORES
    // ==========================================

    for (int i = 0; i < num_players; i++)
    {
        Bowling_Frame * current = players[i];

        while (current != nullptr)
        {
            // First roll
            current->update_scores();

            // Second roll
            current->update_scores();

            // Frame 10 may need a third roll
            if (current->frame_num() == 10)
            {
                if (current->strike() || current->spare())
                {
                    current->update_scores();
                }
            }

            current = current->next_frame();
        }
    }


    // ==========================================
    // RECALCULATE STRIKE/SPARE BONUSES
    // ==========================================

    // Earlier frames need the later frame scores
    // before their bonus can be calculated.
    for (int i = 0; i < num_players; i++)
    {
        Bowling_Frame * current = players[i];

        while (current != nullptr)
        {
            current->update_scores();
            current = current->next_frame();
        }
    }


    // ==========================================
    // PRINT EACH PLAYER'S GAME
    // ==========================================

    for (int i = 0; i < num_players; i++)
    {
        Bowling_Frame * current = players[i];

        while (current != nullptr)
        {
            cout << "Frame "
                 << current->frame_num()
                 << ":" << endl;

            cout << " Score 1: "
                 << current->score1()
                 << endl;

            cout << " Score 2: "
                 << current->score2()
                 << endl;

            current = current->next_frame();
        }

        cout << endl;

        cout << "Total Game Score for player "
             << i + 1
             << ": "
             << current_game_total(players[i])
             << endl;

        cout << endl;
    }


    // ==========================================
    // DELETE ALL FRAMES
    // ==========================================

    for (int i = 0; i < num_players; i++)
    {
        Bowling_Frame * current = players[i];

        while (current != nullptr)
        {
            Bowling_Frame * next = current->next_frame();

            delete current;

            current = next;
        }
    }

    delete [] players;


    return 0;
}
