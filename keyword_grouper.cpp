//
//  keyword_grouper.cpp
//  Keyword Grouper
//
//  Created by Jad Beydoun on 3/3/19.
//

#include <ctype.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <sstream>

using namespace std;

struct Item {
    string name;
    double value;
    
    Item() {
        name = "";
        value = 0;
    }
    
    Item(string name, double value) {
        this->name = name;
        this->value = value;
    }
};

struct CompareItems {
    bool operator()(const Item &left, const Item &right) const {
        return (left.value > right.value);
    }
};

void group_words(vector<Item> &items, string &item_list, string keyword, string category);
string to_title_case(string word);
string trim_zeroes(string number);

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);

    vector<Item> items;
    vector<string> words;
    string item_list = "";
    string name_output = "";
    string value_output = "";

    words.reserve(argc - 1);

    for (char** word = argv + 1; *word != argv[argc]; ++word) {
        words.push_back(*word);
    }

    vector<string>::iterator words_iter = words.begin();
    vector<string>::iterator end_iter = words.end();

    while (words_iter != end_iter) {
        string keyword = *words_iter++;
        string new_category = *words_iter++;
        items.clear();
        group_words(items, item_list, keyword, new_category);
        cin.ignore();
    }

    u_int count = 0;
    for (Item iter : items) {
        string item_name = string(iter.name);
        double item_value = iter.value;
        item_name = item_name.substr(0, item_name.find_last_of(' '));
        name_output += item_name += '\n';
        value_output += trim_zeroes(to_string(item_value));

        if (++count != items.size()) value_output += '\n';
    }

    cout << item_list << '\n';
    cout << "Output for copying into spreadsheets:\n\n";
    cout << name_output << '\n';
    cout << value_output << std::flush;

    return 0;
}

/**
 * Requires: Some form of user input/input redirection.
 *
 * Modifies: items, item_list
 *
 * Effects: Creates a new Item category using the input category and searches the user input for 
 * strings that match the input keyword, while grouping up the values of entries that contain the
 * keyword. Sorts the list in increasing order by item/entry values and adds the new sorted and
 * grouped categories to item_list for output/reuse in a next iteration.
 */
void group_words(vector<Item> &items, string &item_list, string keyword, string category) {
    string input;
    string name;
    istringstream iss(item_list);
    istream& inputStream = (item_list.empty()) ? cin : iss;

    keyword = to_title_case(keyword);
    category = to_title_case(category);
    items.push_back(Item(category += ' ', 0));

    while (getline(inputStream, input)) {
        list<string> entry;
        stringstream ss(input);
        string entry_name;
        string word;
        double entry_value;
        bool match = false;

        while (ss >> word) entry.push_back(to_title_case(word));
        entry_value = stod(entry.back());
        entry.pop_back();

        while (!entry.empty()) {
            if (entry.front().compare(keyword) == 0) match = true;
            entry_name += (entry.front() + ' ');
            entry.pop_front();
        }

        if (match) {
            items.front().value += entry_value;
        }
        else {
            items.push_back(Item(entry_name, entry_value));
        }
    }

    sort(items.begin(), items.end(), CompareItems());

    item_list = "";
    for (Item iter : items) {
        item_list += string(iter.name + trim_zeroes(to_string(iter.value)) + '\n');
    }

    return;
}


/**
 * Requires: The input "word" is at least one character in length.
 *
 * Modifies: word
 *
 * Effects: Converts the word to follow title case by first casting every character to lowercase and
 * then casting the first letter to uppercase.
 */
string to_title_case(string word) {
    for (char letter : word) {
        letter = (char) tolower(letter);
    }
    word.front() = (char) toupper(word.front());

    return word;
}

/**
* Requires: Input number can be represented as a nonzero value.

* Modifies: number

* Effects: Removes trailing zeroes from a number string and removes the decimal point if it can be 
* represented as an integer.
*
* Ex: 0.00100 -> 0.001, 1.000 -> 1
*/
string trim_zeroes(string number) {
    number.erase(number.find_last_not_of('0') + 1);
    if (number.back() == '.') number.erase(number.find('.'));
    
    return number;
}
