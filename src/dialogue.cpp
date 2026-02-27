//
// Created by $JingyanZhang on 20/02/2026.
//

#include "dialogue.hpp"

Dialogue::Dialogue(int const version) : current_node_index(0) {
    if (version == 0) {
        build_blue_flow();
    }
    else if (version == 1) {
        build_leo_flow();
    }
    else {
        throw std::out_of_range("only version 0 and 1 are implemented");
    }
}

void Dialogue::build_blue_flow() {
    // dialogue 0
    nodes.push_back({
        "Hello!! I'm Blue",
        {
            {
                "Start", 1
            },
            {
                "Goodbye", 2
            }
        }
    });

    // dialogue 1
    nodes.push_back({
        "As we all know....Today is a very, very, VERY important day! It's someone's birthday!!",
        {
            {
                "Whose????", 3
            },
            {
                "Is it yours?", 4
            },
        }
    });

    //dialogue 2 (bye dialogue)
    nodes.push_back({
        "Fineeeeeee, see you next time!",
        {}
    });

    //dialogue 3 (Main events block)
    nodes.push_back({
        "It's YOUR day!! And I have prepared the most amazing celebration! Now...which one should we start:",
        {
            {
                "have birthday cake with Blue!", 5
            },
            {
                "sing birthday song with Blue!", 9
            },
            {
                "A surprise (do not click this unless you are prepared)", 11
            },
        }
    });

    //dialogue 4 (call back 2)
    nodes.push_back({
        "I wish :(, but no. Today is about someone way cooler!",
        {
            {
                "Let me try one more time", 1
            }
        }
    });

    // dialogue 5 (birthday cake) (cake needs to pop up in the window)
    nodes.push_back({
        "I baked this cake myself. Please ignore the smoke. That's just...decorative atmosphere.",
        {
            {
                "Why is it blue?", 6
            },
            {
                "Why is it moving?", 7
            },
            {
                "Make a wish!!", 8
            }
        },
        DialogueEffect::ShowCake
    });

    // 6
    nodes.push_back({
        "Ehhh..I swear I didn't add something weird...",
        {
            {
                    "Okey???", 5
                },
            {
                "Anyways, make a wish!", 8
            }
        }
    });

    // 7
    nodes.push_back({
        "That means it's fresh!! Probably ;|",
        {
            {
                "Okey???", 5
            },
            {
                "Anyways, make a wish!", 8
            }

        }
    });

    // 8 (make a wish part)
    nodes.push_back({
        "I hope this year gives you more laughter, fewer stress monsters, and at least 300% more happiness!!!",
        {
            {
                "I wanna do more things!!", 3
            },
            {
                "I'm tired, bye bye Blue", 2
            }
        },
        DialogueEffect::ShowCake
    });

    // 9 (sing happy birthday song) (music must play during this dialogue)
    nodes.push_back({
        "HAP-PY BIR-TH-DAAAYYYY TOOOO YOOOOUUUU—",
        {
            {
                "You sing well but please stop.. I wanna do other things!", 3
            },
            {
                "WOWOWOOWOWOWOWOWOOW", 10
            },
            {
                "Bye Blue...", 2
            }
        },
        DialogueEffect::PlaySong
    });

    // 10
    nodes.push_back({
        "You are either very kind...or slightly chaotic $_$. But seriously, But seriously...I hope today is full of happy moments. HAPPY BIRTHDAY!!!",
    {
        {
            "I wanna do more things!!", 3
        },
        {
            "I'm tired, bye bye Blue", 2
        }
    }
    });

    // 11
    nodes.push_back({
        "OHHH so finally you are here.. Are you ready for the surprise??",
{
        {
        "Nahh I misclicked. Let me leave this as secret", 3
        },
        {
            "YESSSS", 12
        }
    }
    });

    // 12 (Pic of Birthday letter display on the window)
    nodes.push_back({
        "Here is your surprise: a gift card from me to you!!!",
        {
            {
                "Thank you Blue, I still wanna do more things!", 3
            },
            {
                "Thank you Blue, bye bye", 2
            }
        },
        DialogueEffect::ShowLetter
    });

}


void Dialogue::build_leo_flow() {
    // todo
}


void Dialogue::choose_option(int option_index) {
    const DialogueNode& current_dialogue = nodes[current_node_index];
    if ((0 <= option_index) && (option_index < current_dialogue.options.size())) {
        current_node_index = current_dialogue.options[option_index].next_dialogue;
    }
}


const DialogueNode& Dialogue::get_current_dialogue() {
    return nodes[current_node_index];
}

