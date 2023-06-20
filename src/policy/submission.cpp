#include <cstdlib>
#include <iostream>
#include <queue>

#include "../config.hpp"
#include "../state/state.hpp"
#include "../policy/alphabeta.hpp"

/**
 * @brief AlphaBeta
 *
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move
 */

Move AlphaBeta::get_move(T_Node* root){
    Move dymmy_move(Point(-1, -1), Point(-1, -1));;

    T_Node* ptr = nullptr;
    for (auto iter = root->child_vec.begin(); iter != root->child_vec.end(); ++iter){
        ptr = *iter;
        if ((*iter)->value == root->value)
            break;
    }
    if (ptr != nullptr)
        return ptr->p_move;
    else
        return dymmy_move;
}

bool AlphaBeta::build_tree(int depth, int player, State *state, T_Node *root){
    root->depth  = depth;
    root->id     = ++node_cnt;
    root->player = player;
    root->state  = state;
    if (depth == MAX_DEPTH)
        root->value = state->evaluate();
    else
        root->value = 0;

    // continue to process child nodes, if depth < MAX_DEPTH
    if (depth < MAX_DEPTH){
        std::vector<Move> legal_moves = state->legal_actions;
        for (auto iter = legal_moves.begin(); iter != legal_moves.end(); ++iter){
            // get child state of state by iter(next move)
            State *child_state = state->next_state(*iter);
            //child_state->get_legal_actions();

            // new child_root & push it into root->child_vec
            T_Node *child_root = new (T_Node);
            child_root->parent = root;
            child_root->p_id   = root->id;
            child_root->p_move = *iter;
            root->child_vec.push_back(child_root); // vector for pointing all children node

            build_tree(depth+1, 1-player, child_state, child_root);
        }
    }
    return true;
}

int AlphaBeta::alphabeta_tree(T_Node *root, int depth, int maximizingPlayer, int alpha, int beta){
    if (root->depth >= MAX_DEPTH)
        return root->value;

    int best_value = 0;
    if (maximizingPlayer){
        best_value = INT_MIN;
        for (auto iter = root->child_vec.begin(); iter != root->child_vec.end(); ++iter){
            best_value = max(best_value, alphabeta_tree(*iter, depth+1, 0, alpha, beta));
            alpha = max(alpha, best_value);
            if (beta <= alpha){
                //std::cout << "alpha break:" << alpha << " " << beta << std::endl;
                break;
            }

        }
        root->value = best_value;
        root->alpha = alpha;
        root->beta  = beta;
        return best_value;
    }else{ /* minimizing player */
        best_value = INT_MAX;
        for (auto iter = root->child_vec.begin(); iter != root->child_vec.end(); ++iter){
            best_value = min(best_value, alphabeta_tree(*iter, depth+1, 1, alpha, beta));
            beta = min(beta, best_value);
            if (beta <= alpha){
                //std::cout << "beta break" << alpha << " " << beta << std::endl;
                break;
            }
        }
        root->value = best_value;
        root->alpha = alpha;
        root->beta  = beta;
        return best_value;
    }
}

void AlphaBeta::node_output(T_Node *root){
    cout << "depth=" << root->depth << " ";
    cout << "id=" << root->id << "\t";
    cout << "p_id=" << root->p_id << "\t";
    cout << "player=" << root->player << "\t";
    cout << "val=" << root->value << "\t";
    cout << "child_size=" << root->child_vec.size() << "\t";
    cout << endl;
    if (root->depth <= MAX_DEPTH-3){
        std::string ss = (root->state)->encode_state();
        //cout << ss << endl;
    }
}

void AlphaBeta::DFS_output(T_Node *root){
    node_output(root);
    if (root->depth < MAX_DEPTH){
        for (auto iter = root->child_vec.begin(); iter != root->child_vec.end(); ++iter)
            DFS_output(*iter);
    }
}

void AlphaBeta::BFS_output(T_Node *t_root){
    queue<T_Node *> q;
    q.push(t_root);
    T_Node *root = nullptr;
    while(!q.empty()){
        root = q.front();
        q.pop();
        if(root->visited == false){
            node_output(root);
            for (auto iter = root->child_vec.begin(); iter != root->child_vec.end(); ++iter)
                q.push(*iter);
            root->visited = true;
        }
    }
}

