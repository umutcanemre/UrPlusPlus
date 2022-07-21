#include <iostream>
// #include "gamestate.h"
// #include "board.h"
// #include "gameviewer.h"
// #include "cliview.h"
#include <vector>
#include <memory>
#include <map>
#include "tile.h"
#include "tokenassassin.h" 
#include "tokenbasic.h"
#include "tokenflexible.h"
#include "tokenspeedster.h"
#include "tokensupporter.h"

using namespace std;
class Tile;

int main() {
    // std::vector<std::vector<Token *>> playersTokens;
    std::vector<std::unique_ptr<Token>> playersTokens;
    for (size_t j=0; j<2; j++) {
            playersTokens.emplace_back(make_unique<TokenAssassin>(j,1,1,1,1));
            playersTokens.emplace_back(make_unique<TokenBasic>(j,2,1,1,2));
            playersTokens.emplace_back(make_unique<TokenBasic>(j,3,1,1,3));
            playersTokens.emplace_back(make_unique<TokenBasic>(j,4,1,1,4));
            playersTokens.emplace_back(make_unique<TokenFlexible>(j,5,1,1,5));
            playersTokens.emplace_back(make_unique<TokenSpeedster>(j,6,1,1,6));
            playersTokens.emplace_back(make_unique<TokenSupporter>(j,7,1,1,7));
        // playersTokens.emplace_back(n);
    }
    std::vector<Tile*> x;
    // unique ptrs cannot be emplaced to back

    cout << playersTokens.at(2)->activateCapture() << endl;
    cout << playersTokens.at(0)->activateCapture() << endl;
    cout << playersTokens.at(5)->activateCapture() << endl;
    cout << playersTokens.at(6)->activateCapture() << endl;
    cout << playersTokens.at(4)->activateCapture() << endl;

    cout << endl;


    playersTokens.at(2)->activatePassive(x);
    playersTokens.at(0)->activatePassive(x);
    playersTokens.at(11)->activatePassive(x);
    playersTokens.at(6)->activatePassive(x);
    playersTokens.at(4)->activatePassive(x); // should do nothing
    cout << endl;


    size_t pathProgress = playersTokens.at(2)->getPathProgress();
    pair<size_t,size_t> position = playersTokens.at(2)->getPosition();
    playersTokens.at(2)->isValidMove(3, 2, 2);


    

    cout << playersTokens.at(0)->isValidMove(3, 3, 3) << endl;
    cout << playersTokens.at(5)->isValidMove(4, 3, 5) << endl; // speedster
    cout << playersTokens.at(5)->isValidMove(2, 3, 4) << endl; // speedster
    cout << playersTokens.at(6)->isValidMove(1, 3, 3) << endl;
    cout << playersTokens.at(4)->isValidMove(1, 3, 3) << endl; // flexible
    cout << endl;


    cout << playersTokens.at(12)->isValidMove(3, 2, 2) << endl; // speedster
    cout << playersTokens.at(10)->isValidMove(3, 3, 3) << endl;
    cout << playersTokens.at(11)->isValidMove(4, 3, 5) << endl; // flexible 
    cout << playersTokens.at(13)->isValidMove(2, 3, 4) << endl; 
    cout << playersTokens.at(9)->isValidMove(1, 3, 3) << endl;
    cout << playersTokens.at(8)->isValidMove(1, 3, 3) << endl; 
    cout << endl;


    pathProgress = playersTokens.at(2)->getPathProgress();
    position = playersTokens.at(2)->getPosition();
    std::cout << "Old path progress: " << static_cast<int>(pathProgress) << " (" << static_cast<int>(position.first) << " " << static_cast<int>(position.second) << ") "<< std::endl;
    playersTokens.at(12)->updatePosition(make_pair(2,2), 2);
    pathProgress = playersTokens.at(2)->getPathProgress();
    position = playersTokens.at(2)->getPosition();
    std::cout << "New path progress: " << static_cast<int>(pathProgress) << " (" << static_cast<int>(position.first) << " " << static_cast<int>(position.second) << ") "<< std::endl;


    pathProgress = playersTokens.at(10)->getPathProgress();
    position = playersTokens.at(10)->getPosition();
    std::cout << "old path progress: " << static_cast<int>(pathProgress) << " (" << static_cast<int>(position.first) << " " << static_cast<int>(position.second) << ") "<< std::endl;
    playersTokens.at(10)->updatePosition(make_pair(3, 3), 3);
    playersTokens.at(10)->updatePosition(make_pair(4, 4), 4);
    pathProgress = playersTokens.at(10)->getPathProgress();
    position = playersTokens.at(10)->getPosition();
    std::cout << "New path progress: " << static_cast<int>(pathProgress) << " (" << static_cast<int>(position.first) << " " << static_cast<int>(position.second) << ") "<< std::endl;

    pathProgress = playersTokens.at(11)->getPathProgress();
    position = playersTokens.at(11)->getPosition();
    std::cout << "old path progress: " << static_cast<int>(pathProgress) << " (" << static_cast<int>(position.first) << " " << static_cast<int>(position.second) << ") "<< std::endl;
    playersTokens.at(11)->updatePosition(make_pair(4, 3), 5); // speedster
    pathProgress = playersTokens.at(11)->getPathProgress();
    position = playersTokens.at(11)->getPosition();
    std::cout << "New path progress: " << static_cast<int>(pathProgress) << " (" << static_cast<int>(position.first) << " " << static_cast<int>(position.second) << ") "<< std::endl;

    pathProgress = playersTokens.at(13)->getPathProgress();
    position = playersTokens.at(13)->getPosition();
    std::cout << "old path progress: " << static_cast<int>(pathProgress) << " (" << static_cast<int>(position.first) << " " << static_cast<int>(position.second) << ") "<< std::endl;
    playersTokens.at(13)->updatePosition(make_pair(2, 3), 4); // speedster
    pathProgress = playersTokens.at(13)->getPathProgress();
    position = playersTokens.at(13)->getPosition();
    std::cout << "New path progress: " << static_cast<int>(pathProgress) << " (" << static_cast<int>(position.first) << " " << static_cast<int>(position.second) << ") "<< std::endl;

    pathProgress = playersTokens.at(9)->getPathProgress();
    position = playersTokens.at(9)->getPosition();
    std::cout << "old path progress: " << static_cast<int>(pathProgress) << " (" << static_cast<int>(position.first) << " " << static_cast<int>(position.second) << ") "<< std::endl;
    playersTokens.at(9)->updatePosition(make_pair(1, 3), 3);
    pathProgress = playersTokens.at(9)->getPathProgress();
    position = playersTokens.at(9)->getPosition();
    std::cout << "New path progress: " << static_cast<int>(pathProgress) << " (" << static_cast<int>(position.first) << " " << static_cast<int>(position.second) << ") "<< std::endl;

    pathProgress = playersTokens.at(8)->getPathProgress();
    position = playersTokens.at(8)->getPosition();
    std::cout << "old path progress: " << static_cast<int>(pathProgress) << " (" << static_cast<int>(position.first) << " " << static_cast<int>(position.second) << ") "<< std::endl;
    playersTokens.at(8)->updatePosition(make_pair(1, 3), 3); // flexible
    pathProgress = playersTokens.at(8)->getPathProgress();
    position = playersTokens.at(8)->getPosition();
    std::cout << "New path progress: " << static_cast<int>(pathProgress) << " (" << static_cast<int>(position.first) << " " << static_cast<int>(position.second) << ") "<< std::endl;

    cout << endl;

    cout << playersTokens.at(12)->getIsProtected() << endl;
    playersTokens.at(12)->setIsProtected(true);
    cout << playersTokens.at(12)->getIsProtected() << endl;
    cout << playersTokens.at(0)->getIsProtected() << endl;
    playersTokens.at(0)->setIsProtected(true);
    cout << playersTokens.at(0)->getIsProtected() << endl;
    playersTokens.at(0)->setIsProtected(false);
    cout << playersTokens.at(0)->getIsProtected() << endl;
    cout << endl;

    playersTokens.at(12)->activateManual(); // speedster
    playersTokens.at(10)->activateManual();
    playersTokens.at(11)->activateManual(); // flexible 
    playersTokens.at(13)->activateManual(); 
    playersTokens.at(9)->activateManual();
    playersTokens.at(8)->activateManual(); 
    cout << endl;

}
