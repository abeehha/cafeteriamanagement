#ifndef AYSTR_H
#define AYSTR_H

#include <iostream>

class AYstr {
    int len;
    int cap;
    char* Cs;
public:
    int strlength() const ;
    int strlength(const char* str);
    void regrow(int newcap);
    void strcpy(char* dest, const char* src);
    void inttostr(int num, char* buffer);
    int strtoint(const char* str) const;
    double strtodouble(const char* str) const;
    const char* c_str() const { return Cs; }

    AYstr();
    AYstr(const char* p);
    AYstr(int size, char ch);
    AYstr(int num);
    AYstr(const AYstr& other);
    ~AYstr();


    void print() const;
    void replacefirst(char c);
    AYstr& trim();
    static AYstr trim(const char* str);
    bool isequal(const AYstr& S) const;
    bool isless(const AYstr& S) const;
    bool isgreater(const AYstr& S) const;
    AYstr substring(int start, int length) const;
    AYstr ToUpper() const;
    AYstr ToLower() const;
    void ToUpper();
    void ToLower();
    void clear();

    char operator[](int i) const;
    char& operator[](int i);
    AYstr operator+(const AYstr& s) const;
    AYstr& operator+=(const AYstr& s);
    AYstr& operator=(const AYstr& other);

    int find_first(char ch, int start_pos = 0) const;
    int find_first(const AYstr& substring, int start_pos = 0) const;
    int find_last(char ch) const;
    int find_last(const AYstr& substring) const;
    int* find_all(char ch, int& count) const;
    int* find_all(const AYstr& sub, int& count) const;

    AYstr concat(const AYstr& s) const;
    AYstr& append(const AYstr& s);
    void insert_at(int i, char ch);
    void insert_at(int i, const AYstr& sub);
    void remove_at(int i);
    void remove_first(char ch);
    void remove_last(char ch);
    void remove_all(char ch);
   
};

#endif
