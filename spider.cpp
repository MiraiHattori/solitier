#include <array>
#include <map>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>


enum Suite : uint16_t
{
    Diamond = 0,
    Spade = 1,
    Clover = 2,
    Heart = 3,
    None = 4,
};



char suiteToChar(const Suite& suite)
{
    switch(suite) {
    case Suite::Diamond:
        return 'D';
    case Suite::Spade:
        return 'S';
    case Suite::Clover:
        return 'C';
    case Suite::Heart:
        return 'H';
    case Suite::None:
        return '-';
    default:
        assert(false);
        return '-';
    }
}

Suite charToSuite(const char suite_c)
{
    switch (suite_c) {
    case 'D':
        return Suite::Diamond;
    case 'S':
        return Suite::Spade;
    case 'C':
        return Suite::Clover;
    case 'H':
        return Suite::Heart;
    case '-':
        return Suite::None;
    default:
        std::cout << "problematic: " << __LINE__ << " " << suite_c << std::endl;
        assert(false);
        return Suite::None;
    }
}

char numToChar(int16_t num) {
    if (num == 1) {
        return 'A';
    } else if (2 <= num and num <= 9) {
        return std::to_string(num).at(0);
    } else if (num == 10) {
        return '0';
    } else if (num == 11) {
        return 'J';
    } else if (num == 12) {
        return 'Q';
    } else if (num == 13) {
        return 'K';
    } else {
        std::cout << "problematic: " << __LINE__ << " " << num << std::endl;
        assert(false);
        return '-';
    }
}

int16_t charToNum(const char c_num)
{
    if (c_num == 'A') {
        return 1;
    } else if ('1' <= c_num and c_num <= '9') {
        return c_num - '0';
    } else if (c_num == '0') {
        return 10;
    } else if (c_num == 'J') {
        return 11;
    } else if (c_num == 'Q') {
        return 12;
    } else if (c_num == 'K') {
        return 13;
    } else {
        std::cout << "problematic: " << __LINE__ << " " << c_num << std::endl;
        assert(true);
        return -1;
    }
}


constexpr int16_t NUM_NONE = -1;

struct Card
{
    explicit Card(Suite suite_, uint16_t num_) : suite(suite_), num(num_) {}
    explicit constexpr Card() = default;
    Suite suite = Suite::None;
    int16_t num = NUM_NONE;
};

std::string cardToStr(const Card& card) {
    std::string card_str{};
    card_str.push_back(suiteToChar(card.suite));
    card_str.push_back(numToChar(card.num));
    return card_str;
}

Card strToCard(const std::string& str) {
    if (str.size() != 2) {
        assert(true);
    }
    return Card(charToSuite(str[0]), charToNum(str[1]));
}

std::ostream& operator<<(std::ostream& os, const Card& card) {
    os << cardToStr(card);
    return os;
}

struct Board
{
    static constexpr size_t ROW_NUM = 8;
    explicit Board() = default;
    std::array<std::vector<Card>, ROW_NUM> board{};
};


std::ostream& operator<<(std::ostream& os, const Board& board) {
    for (auto it_column = board.board.begin(); it_column != board.board.end(); ++it_column) {
        for (auto it_row = (*it_column).begin(); it_row != (*it_column).end(); ++it_row) {
            os << *it_row;
            os << " ";
        }
        os << "\n";
    }
    return os;
}

// file format
// row is column
// column is row
// split with commas

Board loadFromFile(const std::string& file)
{
    std::ifstream ifs(file);
    if (not ifs.is_open()) {
        std::cerr << "cannot open file " << file << std::endl;
        return Board{};
    }
    std::string buf_line_str;
    std::string buf_card_str;
    Board board;
    while (std::getline(ifs, buf_line_str)) {
        std::istringstream iss(buf_line_str);
        for (size_t i = 0; i < Board::ROW_NUM; i++) {
            std::getline(iss, buf_card_str, ',');
            std::cout << buf_card_str << std::endl;
            board.board[i].emplace_back(strToCard(buf_card_str));
        }
    }
    return board;
}

std::array<std::vector<Card>, Board::ROW_NUM> generateRandomCard() {
    std::array<std::vector<Card>, Board::ROW_NUM> ordered_allcard = std::array<std::vector<Card>, Board::ROW_NUM>();
    for (size_t i = 0; i < Board::ROW_NUM; ++i) {
        ordered_allcard.at(i) = std::vector<Card>(13, Card(Suite::Diamond, 1));
    }
    return ordered_allcard;
}

int main()
{
    Board board;
    board.board = generateRandomCard();
    std::cout << board << std::endl;
    return 0;
}
