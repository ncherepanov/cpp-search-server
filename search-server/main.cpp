<<<<<<< HEAD

/******************************************************************************

Nikolay Cherepanov
43

*******************************************************************************/

=======
﻿
>>>>>>> c51da619f63dc5df7983b0d10eb6499178382341
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
<<<<<<< HEAD
    Document() = default;

    Document(int id, double relevance, int rating)
        : id(id)
        , relevance(relevance)
        , rating(rating) {
    }

    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};

template <typename StringContainer>
set<string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    set<string> non_empty_strings;
    for (const string& str : strings) {
        if (!str.empty()) {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}

=======
    int id;
    double relevance;
    int rating;
};

>>>>>>> c51da619f63dc5df7983b0d10eb6499178382341
enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
public:
<<<<<<< HEAD
    SearchServer() = default;

    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words)
        : stop_words_(MakeUniqueNonEmptyStrings(stop_words)) {
        for(const auto& word : stop_words_){
            if (!IsValidWord(word)){
                throw invalid_argument("In stop words are especial simbols!"s);
            }   
        }
    }
    
    explicit SearchServer(const string& stop_words) {
        for (const string& word : SplitIntoWords(stop_words)) {
            if (!IsValidWord(word)){
                throw invalid_argument("In stop words are especial simbols!"s);
           }
            stop_words_.insert(word);
        }
    }
    
    void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings){
        if ((documents_.find(document_id) != documents_.end()) || (document_id < 0)) {
            throw invalid_argument("Wrong id!"s);
        }
        const vector<string> words = SplitIntoWordsNoStop(document);
        for (auto& word : words) {
            if (!IsValidWord(word)) {
            return throw invalid_argument("Founded special simbols!");
            }
        }
=======
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document, DocumentStatus status,
                     const vector<int>& ratings) {
        const vector<string> words = SplitIntoWordsNoStop(document);
>>>>>>> c51da619f63dc5df7983b0d10eb6499178382341
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
<<<<<<< HEAD
        documents_index_.push_back(document_id);
    }

    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const {
        if (raw_query.empty()) {
            throw invalid_argument("Empty query"s);
        }
        for (const string& word : SplitIntoWords(raw_query)) {
            if (!IsValidWord(word) || !IsValidMinusWord(word)) {
                throw invalid_argument("The query includes special simbols or minuses"s);
            }
        }
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, document_predicate);
=======
    }

    template <typename KeyMapper>
    vector<Document> FindTopDocuments(const string& raw_query, KeyMapper key_mapper) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, key_mapper);

>>>>>>> c51da619f63dc5df7983b0d10eb6499178382341
        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
                     return lhs.rating > rhs.rating;
                 } else {
                     return lhs.relevance > rhs.relevance;
                 }
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }
    
    
    vector<Document> FindTopDocuments(const string& raw_query) const{
        return FindTopDocuments(raw_query, 
                                [](int document_id, DocumentStatus status, int rating) 
                                { return status == DocumentStatus::ACTUAL;});
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query,
                                                        int document_id) const {
        const Query query = ParseQuery(raw_query);
        vector<string> matched_words;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }
        return {matched_words, documents_.at(document_id).status};
    }

    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        return FindTopDocuments(
            raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
                return document_status == status;
            });
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
    }

    int GetDocumentCount() const {
        return documents_.size();
    }
    
    int GetDocumentId(int index) const {
        if ((index < 0) || (index >= static_cast<int>(documents_.size()))) {
            throw out_of_range("Wrong id"s);
        }
        return documents_index_[index];
    }

    tuple <vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const {
        if (raw_query.empty()) {
            throw invalid_argument("Empty query"s);
        }
        for (const string& word : SplitIntoWords(raw_query)) {
            if (!IsValidWord(word) || !IsValidMinusWord(word)) {
                throw invalid_argument("The query includes special simbols or minuses"s);
            }
        }
        const Query query = ParseQuery(raw_query);
        vector<string> matched_words;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }
        return tuple {matched_words, documents_.at(document_id).status};
    }

private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };
    set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;
    vector<int> documents_index_;

<<<<<<< HEAD
=======
    set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;

>>>>>>> c51da619f63dc5df7983b0d10eb6499178382341
    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = 0;
        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    }
<<<<<<< HEAD
    
    static bool IsValidWord(const string& word) {
        // A valid word must not contain special characters
        return none_of(word.begin(), word.end(), [](char c) {
            return c >= '\0' && c < ' ';
        });
    }    

    static bool IsValidMinusWord(const string& word) {
        if (word.empty() || (word == "-"s)) {
            return false;
        }
        char lit_left = 0;
        for(char lit : word){
            if((lit == '-') && (lit_left == '-')) { return false; }
            else { lit_left = lit; };
        }
        /*if (word[0] == '-' && word[1] == '-') {
            return false;
        }*/
        return true;
    }
=======
>>>>>>> c51da619f63dc5df7983b0d10eb6499178382341

    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {text, is_minus, IsStopWord(text)};
    }

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }

    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

<<<<<<< HEAD
    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query,
                                      DocumentPredicate document_predicate) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
=======
    template <typename KeyMapper>
    vector<Document> FindAllDocuments(const Query& query, KeyMapper key_mapper) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            
>>>>>>> c51da619f63dc5df7983b0d10eb6499178382341
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto &[document_id, term_freq] : word_to_document_freqs_.at(word)) {
<<<<<<< HEAD
                const auto& document_data = documents_.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
=======
                if constexpr (is_same_v<KeyMapper, DocumentStatus>){
                    if (documents_.at(document_id).status == key_mapper){
                        document_to_relevance[document_id] += term_freq * inverse_document_freq;    
                    }
                }
                else{
                    if (key_mapper(document_id, documents_.at(document_id).status, documents_.at(document_id).rating)) {
                        document_to_relevance[document_id] += term_freq * inverse_document_freq;
                    }
>>>>>>> c51da619f63dc5df7983b0d10eb6499178382341
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto &[document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto &[document_id, relevance] : document_to_relevance) {
            matched_documents.push_back(
                {document_id, relevance, documents_.at(document_id).rating});
        }
        return matched_documents;
    }
};

<<<<<<< HEAD
// ==================== для примера =========================//
=======


>>>>>>> c51da619f63dc5df7983b0d10eb6499178382341

void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
<<<<<<< HEAD
         << "rating = "s << document.rating << " }"s << endl;
}

int main() {
    try {
        //SearchServer search_server("и в на"s);
        SearchServer search_server("и \x12в на"s);
        search_server.AddDocument(1, "пушистый кот #пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
        //search_server.AddDocument(1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2});
        search_server.AddDocument(-1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2});
        //search_server.AddDocument(3, "большой пёс скво\x12рец"s, DocumentStatus::ACTUAL, {1, 3, 2});
        //search_server.FindTopDocuments("--пушистый"s);
    }
    catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }
    catch(const out_of_range& e){
        cout << e.what() << endl;
    }
}

=======
         << "rating = "s << document.rating
         << " }"s << endl;
}
int main() {
    SearchServer search_server;
    search_server.SetStopWords("и в на"s);
    search_server.AddDocument(0, "белый кот и модный ошейник"s,        DocumentStatus::ACTUAL, {8, -3});
    search_server.AddDocument(1, "пушистый кот пушистый хвост"s,       DocumentStatus::ACTUAL, {7, 2, 7});
    search_server.AddDocument(2, "ухоженный пёс выразительные глаза"s, DocumentStatus::ACTUAL, {5, -12, 2, 1});
    search_server.AddDocument(3, "ухоженный скворец евгений"s,         DocumentStatus::BANNED, {9});
    cout << "ACTUAL by default:"s << endl;
    for (const Document& document : search_server.FindTopDocuments("пушистый ухоженный кот"s)) {
        PrintDocument(document);
    }
    cout << "BANNED:"s << endl;
    for (const Document& document : search_server.FindTopDocuments("пушистый ухоженный кот"s, DocumentStatus::BANNED)) {
        PrintDocument(document);
    }
    cout << "Even ids:"s << endl;
    for (const Document& document : search_server.FindTopDocuments("пушистый ухоженный кот"s, [](int document_id, DocumentStatus status, int rating) { return document_id % 2 == 0; })) {
        PrintDocument(document);
    }
    return 0;
}

>>>>>>> c51da619f63dc5df7983b0d10eb6499178382341
