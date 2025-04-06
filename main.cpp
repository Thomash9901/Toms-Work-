#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include "chessBoard.h"
#include <string>
#include <random>
#include <sstream>
#include <chrono>
#include "MoveScore.h"
#include "OpeningBook.h"

static std::vector<std::string> moveList;
static float loggerScrollOffset = 0.f;


void drawMoveLogger(sf::RenderWindow& window, sf::Font& font, const std::vector<std::string>& moves, float& scrollOffset)
{
    float panelX = 800.f;
    float panelWidth = 400.f;
    float panelHeight = 800.f;

    sf::Color bgColor (0x2f, 0x2f, 0x2f);
    sf::Color boxColor (0x44, 0x44, 0x44);
    sf::Color textColor (0xee, 0xee, 0xee);
    sf::Color borderColor (0xaa, 0xaa, 0xaa);
    sf::Color buttonColor (0x50, 0x50, 0x50);


    sf::RectangleShape panelBg(sf::Vector2f(panelWidth, panelHeight));
    panelBg.setPosition(panelX, 0.f);
    panelBg.setFillColor(bgColor);
    window.draw(panelBg);

    float sideMargin = 10.f;
    sf::Text title("Move Logger", font, 24);
    title.setFillColor(textColor);
    title.setStyle(sf::Text::Bold);
    title.setPosition(panelX + sideMargin, 8.f);
    window.draw(title);

    sf::RectangleShape separator(sf::Vector2f(panelWidth - sideMargin * 2, 2.f));
    separator.setPosition(panelX + sideMargin, 40.f);
    separator.setFillColor(borderColor);
    window.draw(separator);

    float bottomPadding = 15.f;
    float buttonHeight = 34.f;
    float buttonWidth = 70.f;
    float buttonSpacing = 10.f;

    float buttonY = panelHeight - buttonHeight - bottomPadding;
    float firstButtonX = panelX + sideMargin;

    sf::RectangleShape undoButton(sf::Vector2f(buttonWidth, buttonHeight));
    undoButton.setPosition(firstButtonX, buttonY);
    undoButton.setFillColor(buttonColor);
    undoButton.setOutlineColor(borderColor);
    undoButton.setOutlineThickness(1.f);
    window.draw(undoButton);

    sf::Text undoText("Undo", font, 16);
    undoText.setFillColor(textColor);
    undoText.setPosition(
            firstButtonX + (buttonWidth - undoText.getLocalBounds().width) / 2.f,
            buttonY + (buttonHeight - undoText.getLocalBounds().height) / 2.f - 3.f
    );
    window.draw(undoText);

    float redoButtonX = firstButtonX + buttonWidth + buttonSpacing;
    sf::RectangleShape redoButton(sf::Vector2f(buttonWidth, buttonHeight));
    redoButton.setPosition(redoButtonX, buttonY);
    redoButton.setFillColor(buttonColor);
    redoButton.setOutlineColor(borderColor);
    redoButton.setOutlineThickness(1.f);
    window.draw(redoButton);

    sf::Text redoText("Redo", font, 16);
    redoText.setFillColor(textColor);
    redoText.setPosition(
            redoButtonX + (buttonWidth - redoText.getLocalBounds().width) / 2.f,
            buttonY + (buttonHeight - redoText.getLocalBounds().height) / 2.f - 3.f
    );
    window.draw(redoText);

    float resignButtonX = redoButtonX + buttonWidth + buttonSpacing;
    sf::RectangleShape resignButton(sf::Vector2f(buttonWidth + 20.f, buttonHeight));
    resignButton.setPosition(resignButtonX, buttonY);
    resignButton.setFillColor(buttonColor);
    resignButton.setOutlineColor(borderColor);
    resignButton.setOutlineThickness(1.f);
    window.draw(resignButton);

    sf::Text resignText("Resign", font, 16);
    resignText.setFillColor(textColor);
    resignText.setPosition(
            resignButtonX + (resignButton.getSize().x - resignText.getLocalBounds().width) / 2.f,
            buttonY + (buttonHeight - resignText.getLocalBounds().height) / 2.f - 3.f
    );
    window.draw(resignText);

    float timeControlAreaHeight = 50.f;
    float timeControlAreaY = buttonY - timeControlAreaHeight - 10.f;

    sf::Text timeLabel("Time Controls", font, 18);
    timeLabel.setFillColor(textColor);
    timeLabel.setPosition(panelX + sideMargin, timeControlAreaY);
    window.draw(timeLabel);
    float timeButtonWidth = 80.f;
    float timeButtonHeight = 28.f;
    float timeButtonSpacing = 8.f;

    float timeButtonY = timeControlAreaY + 22.f;
    float firstTimeButtonX = panelX + sideMargin;

    for (int i = 0; i < 3; ++i)
    {
        sf::RectangleShape timeButton(sf::Vector2f(timeButtonWidth, timeButtonHeight));
        timeButton.setFillColor(buttonColor);
        timeButton.setOutlineColor(borderColor);
        timeButton.setOutlineThickness(1.f);

        float xPos = firstTimeButtonX + (timeButtonWidth + timeButtonSpacing) * i;
        timeButton.setPosition(xPos, timeButtonY);
        window.draw(timeButton);

        std::string label;
        if (i == 0)
        {
            label = "5 Min";
        }
        else if (i == 1)
        {
            label = "10 Min";
        }
        else {label = "15 Min";}
        sf::Text timeBtnText(label, font, 14);
        timeBtnText.setFillColor(textColor);
        float textX = xPos + (timeButtonWidth - timeBtnText.getLocalBounds().width) / 2.f;
        float textY = timeButtonY + (timeButtonHeight - timeBtnText.getLocalBounds().height) / 2.f - 3.f;
        timeBtnText.setPosition(textX, textY);
        window.draw(timeBtnText);
    }

    float movesBoxX = panelX;
    float movesBoxY = 42.f;
    float movesBoxWidth = panelWidth;
    float movesBoxHeight = (timeControlAreaY - 10.f) - movesBoxY;

    sf::RectangleShape movesBox(sf::Vector2f(movesBoxWidth, movesBoxHeight));
    movesBox.setPosition(movesBoxX, movesBoxY);
    movesBox.setFillColor(boxColor);
    movesBox.setOutlineThickness(1.f);
    movesBox.setOutlineColor(borderColor);
    window.draw(movesBox);

    float textLeftMargin = 6.f;
    float textTopMargin = 6.f;
    float lineSpacing = 24.f;

    float colNumberWidth = 30.f;
    float colWhiteWidth  = 50.f;
    float gapBetweenCols = 8.f;

    float colNumberX = movesBoxX + textLeftMargin;
    float colWhiteX  = colNumberX + colNumberWidth + gapBetweenCols;
    float colBlackX  = colWhiteX + colWhiteWidth + gapBetweenCols;

    float textY = movesBoxY + textTopMargin + scrollOffset;
    int moveNumber = 1;

    for (std::size_t i = 0; i < moves.size(); i += 2)
    {
        std::string whiteMoveFull = moves[i];
        std::string whiteMoveTo;
        if (whiteMoveFull.size() >= 4)
        {
            whiteMoveTo = whiteMoveFull.substr(2);
        }
        else
        {
            whiteMoveTo = whiteMoveFull;
        }

        std::string blackMoveTo;
        if (i + 1 < moves.size())
        {
            std::string blackMoveFull = moves[i + 1];
            if (blackMoveFull.size() >= 4)
            {
                blackMoveTo = blackMoveFull.substr(2);
            }
            else
            {
                blackMoveTo = blackMoveFull;
            }
        }
        sf::Text numText(std::to_string(moveNumber) + ".", font, 18);
        numText.setFillColor(textColor);
        numText.setPosition(colNumberX, textY);

        sf::Text whiteText(whiteMoveTo, font, 18);
        whiteText.setFillColor(textColor);
        whiteText.setPosition(colWhiteX, textY);

        sf::Text blackText(blackMoveTo, font, 18);
        blackText.setFillColor(textColor);
        blackText.setPosition(colBlackX, textY);

        float lineTop = textY;
        float lineBottom = textY + lineSpacing;
        float boxBottom = movesBoxY + movesBoxHeight;
        if (lineBottom >= movesBoxY && lineTop <= boxBottom)
        {
            window.draw(numText);
            window.draw(whiteText);
            if (!blackMoveTo.empty())
            {
                window.draw(blackText);
            }
        }
        textY = textY + lineSpacing;
        moveNumber++;
    }

    float totalLinesHeight = (moveNumber - 1) * lineSpacing + textTopMargin;
    float maxVisibleHeight = movesBoxHeight - textTopMargin;
    float maxScrollOffset = -(totalLinesHeight - maxVisibleHeight);
    if (maxScrollOffset > 0)
    {
        maxScrollOffset = 0;
    }
    if (scrollOffset > 0)
    {
        scrollOffset = 0;
    }
    else if (scrollOffset < maxScrollOffset)
    {
        scrollOffset = maxScrollOffset;
    }

    if (totalLinesHeight > maxVisibleHeight)
    {
        float scrollbarWidth = 8.f;
        float scrollbarX = movesBoxX + movesBoxWidth - scrollbarWidth;
        float scrollbarY = movesBoxY;
        float scrollbarHeight = movesBoxHeight;

        float visibleRatio = maxVisibleHeight / totalLinesHeight;
        float thumbHeight = visibleRatio * scrollbarHeight;
        float scrollableHeight = scrollbarHeight - thumbHeight;

        float offsetRange = -maxScrollOffset;
        float offsetNormalized = (scrollOffset - maxScrollOffset) / offsetRange;
        float thumbY = scrollbarY + scrollableHeight * offsetNormalized;

        sf::RectangleShape scrollbarBg(sf::Vector2f(scrollbarWidth, scrollbarHeight));
        scrollbarBg.setPosition(scrollbarX, scrollbarY);
        scrollbarBg.setFillColor(sf::Color(70,70,70));
        window.draw(scrollbarBg);

        sf::RectangleShape scrollbarThumb(sf::Vector2f(scrollbarWidth, thumbHeight));
        scrollbarThumb.setPosition(scrollbarX, thumbY);
        scrollbarThumb.setFillColor(sf::Color(180,180,180));
        window.draw(scrollbarThumb);
    }
}


bool handleMove(sf::RenderWindow& window, ChessBoard& board, bool& whitesTurn, int& fromCol, int& fromRow, const std::map<char, sf::Texture>& pieceTextures, sf::RectangleShape squares[8][8], sf::Font& loggerFont) {
    sf::RectangleShape highlightSquare;
    highlightSquare.setSize(sf::Vector2f(100, 100));
    highlightSquare.setFillColor(sf::Color::Transparent);
    highlightSquare.setOutlineThickness(2);
    highlightSquare.setOutlineColor(sf::Color::Red);
    sf::Event event;
    bool gameOver = false;
    bool isGameRunning = window.isOpen();
    while (isGameRunning == true)
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (gameOver == true)
            {
                board.initialiseBoard();
                whitesTurn = true;
                fromCol = -1;
                fromRow = -1;
                gameOver = false;
                board.printBoard();
            }
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                float scrollAmount = 15.f;
                if (event.mouseWheelScroll.delta > 0)
                {
                    loggerScrollOffset += scrollAmount;
                }
                else
                {
                    loggerScrollOffset -= scrollAmount;
                }
            }
            if ((gameOver == false) && (whitesTurn == true) && event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                int col = mousePos.x / 100;
                int displayRow = mousePos.y / 100;
                int row = 7 - displayRow;
                if (fromCol == -1)
                {
                    char pieceSymbol = board.getPieceSymbolAt(row, col);
                    if (pieceSymbol != '.')
                    {
                        fromCol = col;
                        fromRow = row;
                        highlightSquare.setPosition(col * 100, displayRow * 100);
                    }
                }
                else
                {
                    ChessBoard previousBoard = board;
                    int fromIndex = fromRow * 8 + fromCol;
                    int toIndex = row * 8 + col;

                    if (board.makeMove(fromIndex, toIndex, whitesTurn) == true)
                    {
                        std::stringstream ss;
                        char fromFile = 'a' + fromCol;
                        char toFile = 'a' + col;
                        int fromRank = fromRow + 1;
                        int toRank = row + 1;
                        ss << fromFile << fromRank << toFile << toRank;
                        moveList.push_back(ss.str());
                        if (!board.isKingInCheck(whitesTurn))
                        {
                            whitesTurn = !whitesTurn;
                            board.printBoard();
                            bool isCheckMate = board.isCheckmate(whitesTurn);
                            bool isStaleMate = board.isStaleMate(whitesTurn);
                            if (isCheckMate || isStaleMate)
                            {
                                gameOver = true;
                            }
                            for (int i = 0; i < 8; ++i)
                            {
                                for (int j = 0; j < 8; ++j)
                                {
                                    window.draw(squares[i][j]);
                                }
                            }
                            for (int row = 0; row < 8; ++row)
                            {
                                for (int col = 0; col < 8; ++col)
                                {
                                    char pieceSymbol = board.getPieceSymbolAt(row, col);
                                    if (pieceSymbol != '.')
                                    {
                                        sf::Sprite pieceSprite;
                                        pieceSprite.setTexture(pieceTextures.at(pieceSymbol));
                                        sf::FloatRect spriteBounds = pieceSprite.getLocalBounds();
                                        pieceSprite.setOrigin(spriteBounds.width / 2, spriteBounds.height / 2);
                                        pieceSprite.setPosition(col * 100 + 43, (7 - row) * 100 + 43);
                                        window.draw(pieceSprite);
                                    }
                                }
                            }
                            drawMoveLogger(window, loggerFont, moveList, loggerScrollOffset);
                            window.display();
                        } else
                        {
                            std::cout << "Move leaves king in check. Try again.\n";
                            board = previousBoard;
                        }
                    }
                    fromCol = -1;
                }
            }
            // AI stuff
            if (!gameOver && !whitesTurn)
            {
                std::cout << "ai is thinking of move";
                MoveScore bestMove;
                bool aiMoveMade = false;

                std::string fullFEN = board.toFEN(false);
                std::istringstream iss(fullFEN);
                std::string piecePlacement, activeColor, castling, enPassant;
                iss >> piecePlacement >> activeColor >> castling >> enPassant;
                std::string lookupKey = piecePlacement + " " + activeColor + " " + castling + " " + enPassant;

                std::vector<std::pair<int, int>> candidates;
                bool needsTransform = false;

                if (aggregatedOpeningBook.find(lookupKey) != aggregatedOpeningBook.end())
                {
                    candidates = aggregatedOpeningBook[lookupKey];
                    needsTransform = false;
                }
                else if (originalOpeningBook.find(lookupKey) != originalOpeningBook.end())
                {
                    candidates = originalOpeningBook[lookupKey];
                    needsTransform = true;
                }

                if (!candidates.empty())
                {
                    std::random_shuffle(candidates.begin(), candidates.end());
                    for (auto candidate : candidates)
                    {
                        MoveScore candidateMove;
                        candidateMove.fromIndex = candidate.first;
                        candidateMove.toIndex = candidate.second;
                        if (needsTransform)
                        {
                            candidateMove.fromIndex = ChessBoard::transformIndexForBlack(candidateMove.fromIndex);
                            candidateMove.toIndex = ChessBoard::transformIndexForBlack(candidateMove.toIndex);
                        }
                        if (board.makeMove(candidateMove.fromIndex, candidateMove.toIndex, false))
                        {
                            bestMove = candidateMove;
                            aiMoveMade = true;
                            break;
                        }
                    }
                    if (!aiMoveMade)
                    {
                        std::cout << "No valid opening book move found, falling back to minimax.";
                    }
                }
                else
                {
                    std::cout << "No opening book moves found for current board state, falling back to minimax.";
                }
                // Fallback to minimax if no valid opening book move was found.
                if (!aiMoveMade)
                {
                    int maxDepth = 7;
                    int currentDepth = 1;
                    auto startTime = std::chrono::steady_clock::now();
                    int timeLimitMillis = 1000;
                    for (currentDepth = 1; currentDepth <= maxDepth; currentDepth++)
                    {
                        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::steady_clock::now() - startTime).count();
                        if (elapsed > timeLimitMillis)
                        {
                            break;
                        }
                        MoveScore currentBest = minimaxCalculation(board, currentDepth, false, 99999, -99999);
                        if (currentBest.fromIndex != -1 && currentBest.toIndex != -1)
                        {
                            bestMove = currentBest;
                        }
                        std::cout << "Depth " << currentDepth << " best score: " << currentBest.score << "\n";
                    }
                    if (bestMove.fromIndex != -1 && bestMove.toIndex != -1)
                    {
                        if (board.makeMove(bestMove.fromIndex, bestMove.toIndex, false))
                        {
                            int aiFromRow = bestMove.fromIndex / 8;
                            int aiFromCol = bestMove.fromIndex % 8;
                            int aiToRow = bestMove.toIndex / 8;
                            int aiToCol = bestMove.toIndex % 8;
                            std::stringstream ssAI;
                            char fromFile = 'a' + aiFromCol;
                            char toFile = 'a' + aiToCol;
                            int fromRank = aiFromRow + 1;
                            int toRank = aiToRow + 1;
                            ssAI << fromFile << fromRank << toFile << toRank;
                            moveList.push_back(ssAI.str());
                            whitesTurn = !whitesTurn;
                            board.printBoard();
                            if (board.isCheckmate(whitesTurn) || board.isStaleMate(whitesTurn))
                            {
                                gameOver = true;
                            }
                            aiMoveMade = true;
                        }
                        else
                        {
                            std::cerr << "AI generated an invalid move somehow" << std::endl;
                        }
                        fromCol = -1;
                        fromRow = -1;
                    }
                    else
                    {
                        std::cout << "No legal moves for Black. Game over.\n";
                        gameOver = true;
                    }
                } else
                {

                    int aiFromRow = bestMove.fromIndex / 8;
                    int aiFromCol = bestMove.fromIndex % 8;
                    int aiToRow = bestMove.toIndex / 8;
                    int aiToCol = bestMove.toIndex % 8;
                    std::stringstream ssAI;
                    char fromFile = 'a' + aiFromCol;
                    char toFile = 'a' + aiToCol;
                    int fromRank = aiFromRow + 1;
                    int toRank = aiToRow + 1;
                    ssAI << fromFile << fromRank << toFile << toRank;
                    moveList.push_back(ssAI.str());
                    whitesTurn = !whitesTurn;
                    board.printBoard();
                    if (board.isCheckmate(whitesTurn) || board.isStaleMate(whitesTurn))
                    {
                        gameOver = true;
                    }
                }
            }
        }

        window.clear();
        for (int i = 0; i < 8; ++i)
        {
            for (int j = 0; j < 8; ++j)
            {
                window.draw(squares[i][j]);
            }
        }
        if (fromCol != -1 && fromRow != -1)
        {
            window.draw(highlightSquare);
        }
        for (int row = 0; row < 8; ++row)
        {
            for (int col = 0; col < 8; ++col)
            {
                char pieceSymbol = board.getPieceSymbolAt(row, col);
                if (pieceSymbol != '.')
                {
                    sf::Sprite pieceSprite;
                    pieceSprite.setTexture(pieceTextures.at(pieceSymbol));
                    // This code changes it so the pieces are now central instead of in the top left of their square
                    sf::FloatRect spriteBounds = pieceSprite.getLocalBounds();
                    pieceSprite.setOrigin(spriteBounds.width / 2, spriteBounds.height / 2);
                    pieceSprite.setPosition(col * 100 + 43, (7 - row) * 100 + 43);
                    window.draw(pieceSprite);
                }
            }
        }
        drawMoveLogger(window, loggerFont, moveList, loggerScrollOffset);
        window.display();
    }
    return true;
}


void createBoard(sf::RenderWindow& window, std::map<char, sf::Texture>& pieceTextures, sf::RectangleShape squares[8][8])
{
    window.create(sf::VideoMode(1200, 800), "Chessboard");
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            squares[i][j].setSize(sf::Vector2f(100, 100));
            squares[i][j].setPosition(i * 100, (7 - j) * 100);
            int total = j + i;
            if ((total) % 2 == 0)
            {
                squares[i][j].setFillColor(sf::Color(120, 140, 80));
            }
            else
            {
                squares[i][j].setFillColor(sf::Color(230, 230, 200));
            }
        }
    }

    sf::Texture whitePawnTexture, blackPawnTexture, whiteRookTexture, blackRookTexture, whiteKnightTexture, blackKnightTexture, whiteBishopTexture, blackBishopTexture,
            whiteQueenTexture, blackQueenTexture, whiteKingTexture, blackKingTexture;

    if (!whitePawnTexture.loadFromFile("assets/pawn_white.png") || !blackPawnTexture.loadFromFile("assets/pawn_black.png") ||
        !whiteRookTexture.loadFromFile("assets/rook_white.png") || !blackRookTexture.loadFromFile("assets/rook_black.png") ||
        !whiteKnightTexture.loadFromFile("assets/knight_white.png") || !blackKnightTexture.loadFromFile("assets/knight_black.png") ||
        !whiteBishopTexture.loadFromFile("assets/bishop_white.png") || !blackBishopTexture.loadFromFile("assets/bishop_black.png") ||
        !whiteQueenTexture.loadFromFile("assets/queen_white.png") || !blackQueenTexture.loadFromFile("assets/queen_black.png") ||
        !whiteKingTexture.loadFromFile("assets/king_white.png") || !blackKingTexture.loadFromFile("assets/king_black.png")) {
        std::cout << "pieces are not loading";
    }

    pieceTextures = {
            {'P', whitePawnTexture}, {'p', blackPawnTexture}, {'R', whiteRookTexture}, {'r', blackRookTexture},
            {'N', whiteKnightTexture}, {'n', blackKnightTexture}, {'B', whiteBishopTexture}, {'b', blackBishopTexture},
            {'Q', whiteQueenTexture}, {'q', blackQueenTexture}, {'K', whiteKingTexture}, {'k', blackKingTexture}
    };
}



int main() {
    sf::Font loggerFont;
    loggerFont.loadFromFile("C:\\Users\\tomha\\CLionProjects\\newProjectTest - new version\\arial.ttf");
    std::vector<std::string> files = { "C:\\Users\\tomha\\CLionProjects\\newProjectTest - new version\\dist\\a.tsv"};
    loadOpeningBookFromFiles(files);
    loadAggregatedOpeningBook("C:\\Users\\tomha\\CLionProjects\\newProjectTest - new version\\dist\\aggregated_opening_book.tsv");

    sf::RenderWindow window;
    sf::RectangleShape squares[8][8];
    std::map<char, sf::Texture> pieceTextures;
    ChessBoard board;
    board.initialiseBoard();
    board.printBoard();
    board.bitBoardSquares();
    createBoard(window, pieceTextures, squares);
    bool whitesTurn = true;
    int fromCol = -1;
    int fromRow = -1;
    handleMove(window, board, whitesTurn, fromCol, fromRow, pieceTextures, squares, loggerFont);
    return 0;
}