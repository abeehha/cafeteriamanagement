#include "AYstr.h"

int AYstr::strlength() const {
    if (!Cs) return 0;
    int count = 0;
    while (Cs[count] != '\0') {
        count++;
    }
    return count;
}
int AYstr::strlength(const char* str) {
    if (!str) return 0;
    int count = 0;
    while (str[count] != '\0') {
        count++;
    }
    return count;
}

void AYstr::regrow(int newcap) {
    if (newcap <= cap) {
        return;
    }
    char* newCs = new char[newcap];
    for (int i = 0; i < len; i++) {
        newCs[i] = Cs[i];
    }
    newCs[len] = '\0';
    delete[] Cs;
    Cs = newCs;
    cap = newcap;
}

void AYstr::strcpy(char* dest, const char* src) {
    if (!dest || !src) return;
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void AYstr::inttostr(int num, char* buffer) {
    int i = 0, temp = num;
    if (num == 0) {
        buffer[i++] = '0';
    }
    else {
        while (temp) {
            temp /= 10;
            i++;
        }
        buffer[i] = '\0';
        while (num) {
            buffer[--i] = '0' + (num % 10);
            num /= 10;
        }
    }
}

int AYstr::strtoint(const char* str) const {
    int num = 0;
    int i = 0;

    while (str[i] != '\0') {
        num = num * 10 + (str[i] - '0');
        i++;
    }
    return num;
}

AYstr::AYstr() : len(0), cap(1) {
    Cs = new char[cap];
    Cs[0] = '\0';
}

AYstr::AYstr(const char* p) {
    if (p) {
        len = strlength(p);
        cap = len + 1;
        Cs = new char[cap];
        strcpy(Cs, p);
    }
    else {
        len = 0;
        cap = 1;
        Cs = new char[cap];
        Cs[0] = '\0';
    }
}

AYstr::AYstr(int size, char ch) {
    len = size;
    cap = size + 1;
    Cs = new char[cap];
    for (int i = 0; i < len; i++) {
        Cs[i] = ch;
    }
    Cs[len] = '\0';
}

AYstr::AYstr(int num) {
    char buffer[20];
    inttostr(num, buffer);
    len = strlength(buffer);
    cap = len + 1;
    Cs = new char[cap];
    strcpy(Cs, buffer);
}

AYstr::AYstr(const AYstr& other) {
    len = other.len;
    cap = other.cap;
    Cs = new char[cap];
    strcpy(Cs, other.Cs);
}

AYstr::~AYstr() {
    delete[] Cs;
}

void AYstr::print() const {
    std::cout << Cs << std::endl;
}

void AYstr::replacefirst(char c) {
    Cs[0] = c;
}

AYstr& AYstr::trim() {
    int start = 0;
    int end = len - 1;

    while (start < len && (Cs[start] == ' ' || Cs[start] == '\t' || Cs[start] == '\n')) {
        start++;
    }
    while (end >= 0 && (Cs[end] == ' ' || Cs[end] == '\t' || Cs[end] == '\n')) {
        end--;
    }

    int newlen = end - start + 1;

    char* newCs = new char[newlen + 1];
    for (int i = 0; i < newlen; i++) {
        newCs[i] = Cs[start + i];
    }
    newCs[newlen] = '\0';

    delete[] Cs;
    Cs = newCs;
    len = newlen;
    cap = newlen + 1;

    return *this;
}

AYstr AYstr::trim(const char* str) {
    int start = 0;
    int end = 0;

    while (str[end] != '\0') end++;
    end--;

    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n') {
        start++;
    }
    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')) {
        end--;
    }

    int newlen = end - start + 1;
    char* newCs = new char[newlen + 1];

    for (int i = 0; i < newlen; i++) {
        newCs[i] = str[start + i];
    }
    newCs[newlen] = '\0';

    AYstr result(newCs);
    delete[] newCs;

    return result;
}

bool AYstr::isequal(const AYstr& S) const {
    if (len != S.len) {
        return false;
    }

    for (int i = 0; i < len; i++) {
        if (Cs[i] != S.Cs[i]) {
            return false;
        }
    }
    return true;
}

bool AYstr::isless(const AYstr& S) const {
    int minlen = (len > S.len) ? S.len : len;

    for (int i = 0; i < minlen; i++) {
        if (Cs[i] < S.Cs[i]) {
            return true;
        }
        if (Cs[i] > S.Cs[i]) {
            return false;
        }
    }
    return len < S.len;
}

bool AYstr::isgreater(const AYstr& S) const {
    int minlen = (len > S.len) ? S.len : len;

    for (int i = 0; i < minlen; i++) {
        if (Cs[i] < S.Cs[i]) {
            return false;
        }
        if (Cs[i] > S.Cs[i]) {
            return true;
        }
    }
    return len > S.len;
}

char AYstr::operator[](int i) const {
    if (i >= 0 && i < len) {
        return Cs[i];
    }
    return '\0';
}

char& AYstr::operator[](int i) {
    if (i >= 0 && i < len) {
        return Cs[i];
    }
    static char temp = '\0';
    return temp;
}

AYstr AYstr::concat(const AYstr& s) const {
    int newlen = len + s.len;
    AYstr result;
    result.regrow(newlen + 1);

    for (int i = 0; i < len; i++) {
        result.Cs[i] = Cs[i];
    }

    for (int i = 0; i < s.len; i++) {
        result.Cs[len + i] = s.Cs[i];
    }
    result.Cs[newlen] = '\0';
    result.len = newlen;
    return result;
}

AYstr& AYstr::append(const AYstr& s) {
    regrow(len + s.len + 1);

    for (int i = 0; i < s.len; i++) {
        Cs[len + i] = s.Cs[i];
    }
    len += s.len;
    Cs[len] = '\0';
    return *this;
}

AYstr AYstr::operator+(const AYstr& s) const {
    return this->concat(s);
}

AYstr& AYstr::operator+=(const AYstr& s) {
    return this->append(s);
}


int AYstr::find_last(char ch) const {
    for (int i = len - 1; i >= 0; i--) {
        if (Cs[i] == ch) {
            return i;
        }
    }
    return -1;
}

int AYstr::find_last(const AYstr& substring) const {
    if (substring.len > len) {
        return -1;
    }

    for (int i = len - substring.len; i >= 0; i--) {
        bool match = true;

        for (int j = 0; j < substring.len; j++) {
            if (Cs[i + j] != substring.Cs[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            return i;
        }
    }
    return -1;
}

int* AYstr::find_all(char ch, int& count) const {
    count = 0;

    for (int i = 0; i < len; i++) {
        if (Cs[i] == ch) {
            count++;
        }
    }

    if (count == 0) {
        return nullptr;
    }

    int* indices = new int[count];
    int ind = 0;

    for (int i = 0; i < len; i++) {
        if (Cs[i] == ch) {
            indices[ind++] = i;
        }
    }
    return indices;
}

int* AYstr::find_all(const AYstr& sub, int& count) const {
    count = 0;

    if (sub.len > len) {
        return nullptr;
    }

    for (int i = 0; i <= len - sub.len; i++) {
        bool match = true;

        for (int j = 0; j < sub.len; j++) {
            if (Cs[i + j] != sub.Cs[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            count++;
        }
    }
    if (count == 0) {
        return nullptr;
    }
    int* indices = new int[count];
    int ind = 0;

    for (int i = 0; i <= len - sub.len; i++) {
        bool match = true;

        for (int j = 0; j < sub.len; j++) {
            if (Cs[i + j] != sub.Cs[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            indices[ind++] = i;
        }
    }
    return indices;
}

void AYstr::insert_at(int i, char ch) {
    if (i < 0 || i > len) {
        return;
    }
    regrow(len + 2);

    for (int j = len; j >= i; j--) {
        Cs[j + 1] = Cs[j];
    }
    Cs[i] = ch;
    len++;
    Cs[len] = '\0';
}

void AYstr::insert_at(int i, const AYstr& sub) {
    if (i < 0 || i > len) {
        return;
    }
    regrow(len + sub.len + 1);

    for (int j = len - 1; j >= i; j--) {
        Cs[j + sub.len] = Cs[j];
    }

    for (int j = 0; j < sub.len; j++) {
        Cs[i + j] = sub.Cs[j];
    }
    len += sub.len;
    Cs[len] = '\0';
}

void AYstr::remove_at(int i) {
    if (i < 0 || i >= len) {
        return;
    }

    for (int j = i; j < len - 1; j++) {
        Cs[j] = Cs[j + 1];
    }
    len--;
    Cs[len] = '\0';
}

void AYstr::remove_first(char ch) {
    for (int i = 0; i < len; i++) {
        if (Cs[i] == ch) {
            remove_at(i);
            return;
        }
    }
}

void AYstr::remove_last(char ch) {
    for (int i = len - 1; i >= 0; i--) {
        if (Cs[i] == ch) {
            remove_at(i);
            return;
        }
    }
}

void AYstr::remove_all(char ch) {
    int i = 0;
    while (i < len) {
        if (Cs[i] == ch) {
            remove_at(i);
        }
        else {
            i++;
        }
    }
}

AYstr AYstr::ToUpper() const {
    AYstr result(*this);
    for (int i = 0; i < result.len; i++) {
        if (result.Cs[i] >= 'a' && result.Cs[i] <= 'z') {
            result.Cs[i] -= 32;
        }
    }
    return result;
}

AYstr AYstr::ToLower() const {
    AYstr result(*this);
    for (int i = 0; i < result.len; i++) {
        if (result.Cs[i] >= 'A' && result.Cs[i] <= 'Z') {
            result.Cs[i] += 32;
        }
    }
    return result;
}

void AYstr::ToUpper() {
    for (int i = 0; i < len; i++) {
        if (Cs[i] >= 'a' && Cs[i] <= 'z') {
            Cs[i] -= 32;
        }
    }
}

void AYstr::ToLower() {
    for (int i = 0; i < len; i++) {
        if (Cs[i] >= 'A' && Cs[i] <= 'Z') {
            Cs[i] += 32;
        }
    }
}

void AYstr::clear() {
    delete[] Cs;
    len = 0;
    cap = 1;
    Cs = new char[cap];
    Cs[0] = '\0';
}
double AYstr::strtodouble(const char* str) const {
    double num = 0.0;
    double fraction = 1.0;
    bool decimalFound = false;
    int i = 0;

    bool isNegative = false;
    if (str[0] == '-') {
        isNegative = true;
        i++;
    }

    while (str[i] != '\0') {
        if (str[i] == '.') {
            decimalFound = true;
        }
        else if (str[i] >= '0' && str[i] <= '9') {
            if (decimalFound) {
                fraction *= 0.1;
                num += (str[i] - '0') * fraction;
            }
            else {
                num = num * 10 + (str[i] - '0');
            }
        }
        i++;
    }

    return isNegative ? -num : num;
}
AYstr AYstr::substring(int start, int length) const {
    if (start < 0 || start >= len || length <= 0) {
        throw std::out_of_range("Invalid substring parameters");
    }

    int actualLength = (start + length > len) ? len - start : length;
    AYstr result(actualLength, '\0');

    for (int i = 0; i < actualLength; i++) {
        result.Cs[i] = Cs[start + i];
    }
    result.Cs[actualLength] = '\0';
    result.len = actualLength;

    return result;
}
int AYstr::find_first(char ch, int start_pos) const {
    if (start_pos < 0) start_pos = 0;
    for (int i = start_pos; i < len; i++) {
        if (Cs[i] == ch) {
            return i;
        }
    }
    return -1;
}
int AYstr::find_first(const AYstr& substring, int start_pos) const {
    int sublen = substring.len;
    if (start_pos < 0) start_pos = 0;

    for (int i = start_pos; i <= len - sublen; i++) {
        bool match = true;
        for (int j = 0; j < sublen; j++) {
            if (Cs[i + j] != substring.Cs[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            return i;
        }
    }
    return -1;
}

AYstr& AYstr::operator=(const AYstr& other) {
    if (this != &other) {
        delete[] Cs;
        len = other.len;
        cap = other.cap;
        Cs = new char[cap];
        strcpy(Cs, other.Cs);
    }
    return *this;
}