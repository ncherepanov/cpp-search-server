// Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3?
// Напишите ответ здесь: 271

// Закомитьте изменения и отправьте их в свой репозиторий.

/******************************************************************************

Nikolay Cherepanov
41

*******************************************************************************/
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() 
{
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() 
{
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) 
{
    vector<string> words;
    string word;
    for (const char c : text) 
    {
        if (c == ' ') 
        {
            if (!word.empty()) 
            {
                words.push_back(word);
                word.clear();
            }
        }
        else 
        {
            word += c;
        }
    }
    if (!word.empty()) 
    {
        words.push_back(word);
    }
    return words;
}

struct Document 
{
    int id;
    double relevance;
};

class SearchServer 
{
public:

    void SetStopWords(const string& text) 
    {
        if(!text.empty())
        {
            for (const string& word : SplitIntoWords(text)) 
            {
                stop_words_.insert(word);
            }
        }
    }

    void AddDocument(int document_id, const string& document) 
    {
        const vector<string> words = SplitIntoWordsNoStop(document);
        if (!words.empty())
        {
           ++document_count_; 
           for(auto& word : words)
           {
               double num = static_cast<double>(count(words.begin(), words.end(), word));
               docs_id_tf_[word][document_id] = num/words.size();
           }
        }
    }

    vector<Document> FindTopDocuments(const string& raw_query) const 
    {
        const Query query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_words);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) 
             {
                 return lhs.relevance > rhs.relevance;
             });
        
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) 
        {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        
        return matched_documents;
    }

private:

    struct Query
    {
        set <string> plus_words;
        set <string> minus_words;
    };

    map <string, map <int, double>> docs_id_tf_; 
    set <string> stop_words_;
    int document_count_ = 0;
    
    bool IsNotStopWordOrEmpty(const string& word) const
    {
        return !stop_words_.count(word) || stop_words_.empty();
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const 
    {
        vector<string> words;
        
        for (const string& word : SplitIntoWords(text)) 
        {
            if (IsNotStopWordOrEmpty(word)) 
            {
                words.push_back(word);
            }
        }
        
        return words;
    }

    Query ParseQuery(const string& text) const 
    {
        Query query;
        
        for (string& word : SplitIntoWordsNoStop(text)) 
        {
            if (word[0] == '-') 
            {
                word = word.substr(1);
                if (IsNotStopWordOrEmpty(word)) 
                {
                    query.minus_words.insert(word);
                }
            }
            else query.plus_words.insert(word);
        }
        
        if (!query.plus_words.empty())
        {
            for (const string& word : query.minus_words)
            {
                if (query.plus_words.count(word)) 
                {
                    query.plus_words.erase(word);
                }
            }
        }
        
        return query;
    }

    vector<Document> FindAllDocuments(const Query& query_words) const 
    {
        vector<Document> matched_documents;
        map <int, double> temp_docs;
      
        if(!query_words.plus_words.empty())
        {  
            for (const auto& word : query_words.plus_words)    
            {    
                if(docs_id_tf_.count(word))
                {
                    double idf = log((static_cast<double>(document_count_))/docs_id_tf_.at(word).size());
                    for (const auto& [id, tf] : docs_id_tf_.at(word)) 
                    {
                        if(tf) temp_docs[id] += idf*tf;
                    }
                }
           
            }
        }
        
        if(!query_words.minus_words.empty())
        {
            for (const auto& word: query_words.minus_words) 
            {
                if(docs_id_tf_.count(word))
                {
                    for(auto& [id, tf] : docs_id_tf_.at(word))
                    {
                        if (temp_docs.count(id)) 
                        {
                            temp_docs.erase(id);
                        }
                    }
                }
            }
        }
      
        if(!temp_docs.empty())
        {
            for(auto& [id, val] : temp_docs)
            {
                matched_documents.push_back({id, val});
            } 
        }
        
        return matched_documents;
    }
};

SearchServer CreateSearchServer() 
{
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());
    int count = ReadLineWithNumber();
    
    for (int document_id = 0; document_id < count; ++document_id) 
    {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() 
{
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) 
    {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}


/*
// наборы для теста
// пустая строка стоп слов

1
a small curly guinea pig with grey hair has been found
cheburashka with big ears likes oranges


is are was a an in the with near at
3
a colorful parrot with green wings and red tail is lost
a grey hound with black ears is found at the railway station
a white cat with long furry tail is found near the red square
white cat long tail
*/
