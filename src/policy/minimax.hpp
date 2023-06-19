#pragma once
#include <limits.h>
using namespace std;

#include "../config.hpp"
#include "../state/state.hpp"
#include "../policy/minimax.hpp"

/**
 * @brief Policy class for minimax policy,
 * your policy class should have get_move method
 */

class T_Node{
    public:
    int depth       = 1;        // starting from root (root=0)
    int id          = 1;        // id of total
    int p_id        = 1;        // id of parent
    int player      = 0;        // 0:white, 1:black
    int value       = 0;        // return by State::evaluate()
    //int alpha       = INT_MIN;  // alpha value
    //int beta        = INT_MAX;  // beta value
    bool visited    = false;    //for BFS
    Move p_move = Move(Point(-1, -1), Point(-1, -1)); // move record from parent
    T_Node* parent  = nullptr;  // point to parent
    State *state    = nullptr;
    std::vector<T_Node*> child_vec; // vector for porting all children node
};

class MiniMax{
public:
    //static Move get_move(State *state, int depth);
    static Move get_move(T_Node* root);

    bool build_tree(int depth, int player, State *state, T_Node *root);
    int minimax_tree(T_Node *root, int depth, int maximizingPlayer);
    int alphabeta_tree(T_Node *root, int depth, int maximizingPlayer,
                       int alpha, int beta);
    void node_output(T_Node *root);
    void DFS_output(T_Node *root);
    void BFS_output(T_Node *root);

    T_Node *t_root = nullptr;
    int node_cnt = 0;
};



