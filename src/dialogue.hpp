//
// Created by $JingyanZhang on 20/02/2026.
//

#ifndef DIALOGUE_HPP
#define DIALOGUE_HPP

#pragma once
#include <string>
#include <vector>

enum class DialogueEffect {
    None,
    ShowCake,
    PlaySong,
    ShowLetter
};

struct Option {
    std::string text;
    int next_dialogue;
};

struct DialogueNode {
    std::string character_text;
    std::vector<Option> options;
    DialogueEffect effect = DialogueEffect::None;
};

struct Dialogue {
    public:
        explicit Dialogue(int);
        const DialogueNode& get_current_dialogue();
        void choose_option(int);
        void build_blue_flow();
        void build_leo_flow();
        int current_node_index;

    private:
        std::vector<DialogueNode> nodes;
};

#endif //DIALOGUE_HPP
