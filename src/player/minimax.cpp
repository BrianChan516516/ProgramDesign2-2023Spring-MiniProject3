#include <iostream>
#include <fstream>


#include "../config.hpp"
#include "../state/state.hpp"
#include "../policy/minimax.hpp"


State* state_root;

/**
 * @brief Read the board from the file
 *
 * @param fin
 */
void read_board(std::ifstream& fin) {
    Board board;
    int player;
    fin >> player;
    //std::cout << "player=" << player << std::endl;
    for (int pl=0; pl<2; pl++) {
        for (int i=0; i<BOARD_H; i++) {
            for (int j=0; j<BOARD_W; j++) {
                int c; fin >> c;
                //std::cout << c << " ";
                board.board[pl][i][j] = c;
            }
            //std::cout << std::endl;
        }
    }
    state_root = new State(board, player);
    state_root->get_legal_actions();
}

/**
 * @brief Main function for player
 *
 * @param argv
 * @return int
 */

int main(int, char** argv) {
    
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    
    // build state object
    read_board(fin);
    
    // build minimax object mm
    MiniMax mm;
    mm.t_root =  new(T_Node);

    // build_tree(int depth, int player, State *state, T_Node *root, T_Node *parent);
    mm.build_tree(1, state_root->player, state_root, mm.t_root);

    // int minimax_tree(T_Node *root, int depth, int maximizingPlayer);
    mm.minimax_tree(mm.t_root, 1, 1);

    //cout << "BFS_output Begin" << endl;
    //mm.BFS_output(mm.t_root);
    //cout << "BFS_output END"<< endl;

    //write_valid_spot(fout);
    auto move = mm.get_move(mm.t_root);
    fout << move.first.first << " "  << move.first.second << " "
         << move.second.first << " " << move.second.second << std::endl;
    fout.flush();

    fin.close();
    fout.close();

    //cout << "final move:" << " "
    //     << move.first.first << " "  << move.first.second << " "
    //     << move.second.first << " " << move.second.second << std::endl;

    return 0;
}
