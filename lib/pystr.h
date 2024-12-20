#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <vector>

#ifdef SHOW_DEST
    #define destprint(string, ptr) printf(string, ptr)
#else
    #define destprint(string, ptr)
#endif

#ifdef SHOW_INIT
    #define initprint(string) printf(string)
#else
    #define initprint(string)
#endif

#define c_lower(_char) if (is_letter(_char)) { _char |=  (1 << 5); }

#define c_upper(_char) if (is_letter(_char)) { _char &= ~(1 << 5); }

#define rund(line)\
            printf("%s\n", #line);\
            line;\

#define _range(number, var_name) (int var_name = 0; var_name<number; var_name++)
#define _irange(number, var_name) (int var_name = number-1; var_name>=0; var_name--)

#define str_assert(expression, prompt) \
        ((expression) ? (void)0 : \
        (fprintf(stderr, "ERROR: %s\n", prompt), exit(-1)))

#define $str(_string, _method, _args) str_to_string(string_to_str(_string)._method(_args))

int floor(float x) {
    return int(x);
}

int ceiling(float x) {
    return int(x) + ((x - int(x) >= 0.5) ? 1 : 0);
}

bool is_letter(char &letter) {
    return (letter >= 65 and letter <= 90) or (letter >= 97 and letter <= 122);
}

bool is_number(char &letter) {
    return (letter >= 48 and letter <= 57);
}

int next_powt(int len){
    int temp = 1;
    while (temp <= len) {
        temp *= 2;
    }
    return temp;
}

int strsrc(char* haystack, char* needle) {
    char* result = strstr(haystack, needle);
    if (result != NULL) {
        return int(result - haystack);
    } else {
        return -1;
    }

}

class str {
    public:
    char* data;
    int length;
    int capacity;
    str& self = *this;

    // PRIVATES:

    int __proc_idx__(int idx) {
        if (idx == -1) {
            return length-1;
        }
        str_assert(idx >= 0 and idx < length, "string index out of bounds");
        return idx;
    }

    char& at(int idx) {
        return data[__proc_idx__(idx)];
    }

    str& __overwrite_self__(const str& other) {
        capacity = next_powt(other.length);
        char* old_data = data;
        data = (char*) malloc(capacity*sizeof(char));
        strcpy(data, other.data);
        length = other.length;
        free(old_data);
        return self;
    }

    str& __add__(str& other) {
        return __insert__(length, other.c_str());
    }

    str& __insert__(size_t _pos, const char* _new) {
        char* old_data = data;
        int new_len = strlen(_new);
        capacity = next_powt(length + strlen(_new));
        data = (char*) malloc(capacity);
        int inserted = 0;
        for _range(length+1, idx) {
            if(idx == _pos) {
                for _range(new_len, idy) {
                    data[idx+idy] = _new[idy];
                    inserted++;
                }
            }
            data[idx+inserted] = old_data[idx];
        }
        length += new_len;
        data[length] = 0;
        free(old_data);
        return self;
    }

    str& __remove__(size_t _pos, size_t _len) {
        str_assert(_len > 0, "length of remover must be more than 0");
        str_assert(_len+_pos <= length, "removing out of range");
        for _range(length-_pos-_len+1, idx) {
            if (_pos+idx+_len <= length) {
                at(_pos+idx) = data[_pos+idx+_len];
            }
        }
        length -= _len;
        return self;
    }

    // CONSTRUCTORS:

    str() { 
        initprint("<class: str> default initializer\n");
        length = 0; capacity = 1; data = (char*)malloc(1);
        *data = 0;
        initprint( "<class: str> done initializing\n");
    }
    
    str(const str& other) {
        initprint("<class: str> other initialized\n");
        length = 0; capacity = 1; data = (char*)malloc(1);
        *data = 0;
        __overwrite_self__(other);
        initprint( "<class: str> done initializing\n");
    }

    str(const char chr) {
        initprint("<class: str> char initialized\n");
        data = (char*)malloc(2);
        data[0] = chr;
        data[1] = 0;
        length = 1;
        capacity = 2;
        initprint( "<class: str> done initializing\n");
    }
    
    str(const char* _string) {
        initprint("<class: str> char* initialized\n");
        int _string_length = strlen(_string);
        length = _string_length;
        capacity = next_powt(length);
        data = (char*) malloc(sizeof(char) * capacity);
        _string_length++;
        for _range(_string_length, idx) {
            data[idx] = _string[idx];
        }
        initprint( "<class: str> done initializing\n");
    }

    // METHODS

    char& operator[] (int idx) { return at(idx); }

    str operator[] (std::initializer_list<int> list) {
        str_assert(list.size() >= 2, "expecting 2 arguments (start, end) but got less");
        str_assert(list.size() <= 2, "expecting 2 arguments (start, end) but got more");
        return substr(*list.begin(), *(list.end()-1));
    }

    const char* encode() { return data; }
    const char* c_str() { return data; }
    const char* _() { return data; }
    bool empty() { return !length; }
    void clear() { __remove__(0, length); }

    str replace(const char* _old, const char* _new, int _count = -1) {
        str temp = self;
        int old_len = strlen(_old);
        int new_len = strlen(_new);
        char* result;
        int offset = 0;
        while ((result = strstr(temp.data + offset, _old)) != NULL) {
            temp.__remove__(result-temp.data, old_len);
            offset = result - temp.data + new_len;
            temp.__insert__(result-temp.data, _new);
        }
        return temp;
    }

    str upper() {
        str temp = self;
        for _range(temp.length, idx) {
            c_upper(temp[idx]);
        }
        return temp;
    }

    str lower() {
        str temp = self;
        for _range(temp.length, idx) {
            c_lower(temp[idx]);
        }
        return temp;
    }

    str swapcase() {
        str temp = self;
        for _range(temp.length, idx) {
            if ((temp[idx] >> 5) & 1) {
                c_upper(temp[idx]);
            } else {
                c_lower(temp[idx]);
            }
        }
        return temp;
    }

    str capitalize() {
        str temp = lower();
        if (temp.length) {
            c_upper(temp[0])
        }
        for _range(length-1, idx) {
            if (temp[idx] == ' ') {
                c_upper(temp[idx+1]);
            }
        }
        return temp;
    }

    str center(int width, const char* fillchar) {
        return center(width, str(fillchar));
    }

    str center(int width, str fillchar) {
        str temp = self;
        if (width <= length) {
            return temp;
        }
        float total = width - length;
        total /= 2;
        int before = floor(total);
        int after = ceiling(total);
        temp = (fillchar * before) + self + (fillchar * after);
        return temp;
    }

    std::vector<str> split(char chr = '\n') {
        std::vector<str> vemp;
        str temp;
        for (char c : self) {
            if (c == chr) {
                vemp.push_back(temp);
                temp.clear();
            } else {
                temp += c;
            }
        }
        if (!temp.empty()) {
            vemp.push_back(temp);
        }
        return vemp;
    }

    str join(std::vector<str> &vec) {
        str temp;
        for _range(vec.size(), idx) {
            if (idx != 0) {
                temp += self;
            }
            temp += vec[idx];
        }
        return temp;
    }

    str substr(int start = 0, int end = 0) {
        if (start < 0) { start = length + start; }
        if (end < 0) { end = length + end; }
        str temp;
        for _range(end-start, idx) {
            temp += at(idx+start);
        }
        return temp;
    }

    int count(const char* _str) {
        char* result;
        int _len = strlen(_str);
        int offset = 0;
        int count = 0;
        while ((result = strstr(data + offset, _str)) != NULL) {
            offset = result - data + _len;
            count++;
        }
        return count;
    }

    int count(str &_str) {
        return count(_str.encode());
    }
    
    bool endswith(const char* _str) {
        int _len = strlen(_str);
        char* _data = data;
        _data += (length - _len);
        return strstr(_data, _str) != NULL;
    }

    bool endswith(str &_str) {
        return endswith(_str.encode());
    }

    str expandtabs(int tabsize = 4) {
        return replace("\t", (str(" ")*tabsize).c_str());
    }

    int find(const char* _str, int start = 0) {
        char* result;
        if ((result = strstr(data+start, _str)) != NULL) {
            return result - data;
        } else { return -1; }
    }

    int find(const str &_str, int start = 0) { return find(_str.data, start); }
    int index(const char* _str) { return find(_str); }
    int index(const str& _str) { return find(_str); }

    int rfind(const char* _str) {
        int pos;
        for _irange(length, idx) {
            if ((pos = find(_str, idx)) != -1) {
                return pos;
            }
        }
        return -1;
    }
    int rfind(const str& _str) { return rfind(_str.data); }
    int rindex(const char* _str) { return rfind(_str); }
    int rindex(const str& _str) { return rfind(_str); }
    
    bool isalnum() {
        for (char s : self) {
            if (!(is_letter(s) or is_number(s))) {
                return 0;
            }
        }
        return 0;
    }

    bool isalpha() {
        for (char s : self) {
            if (!is_letter(s)) {
                return 0;
            }
        }
        return 0;
    }

    bool isdecimal() {
        for (char s : self) {
            if (!is_number(s)) {
                return 0;
            }
        }
        return 0;
    }

    bool isdigit() { return isdecimal(); }

    bool islower() {
        bool cond = 1;
        for (char s : self) {
            char nc = s;
            c_lower(nc);
            if (s != nc) {
                return 0;
            }
        }
        return 1;
    }

    bool isupper() {
        for (char s : self) {
            char nc = s;
            c_upper(nc);
            if (s != nc) {
                return 0;
            }
        }
        return 1;
    }

    bool isspace() {
        for (char s : self) {
            if (s != ' ') {
                return 0;
            }
        }
        return 1;
    }

    str lstrip(const char* chars) {
        str temp = self;
        int count = 0;
        for (char c : self) {
            printf("%c", c);
            if (str(chars).find(str(c)) != -1) {
                printf("-");
                count++;
            } else {
                printf("H");
                break;
            }
        }
        temp.__remove__(0, count);
        return temp;
    }
    str lstrip(const str& chars) { return lstrip(chars.data); }

    std::vector<str> partition(const char* sep) {
        std::vector<str> vemp;
        int position = find(sep);
        if (position == -1 or strlen(sep) == 0) {
            vemp.push_back(self);
            vemp.push_back(str());
            vemp.push_back(str());
        } else {
            vemp.push_back(substr(0,position));
            vemp.push_back(substr(position,position+strlen(sep)));
            vemp.push_back(substr(position+strlen(sep), length));
        }   
        return vemp;
    }
    std::vector<str> partition(const str& sep) { return partition(sep.data); }

    str ljust(int width, const char* chr = " ") {
        str temp = self;
        if (width <= length) { return temp; }
        temp = (str(chr)*(width-length)) + temp;
        return temp;
    }

    str ljust(int width, const char chr) {
        return ljust(width, str(chr).c_str());
    }

    str removesuffix(const char* _str) {
        int _len = strlen(_str);
        int position = find(_str, length-_len);
        if (position != -1) {
            return substr(0,-_len);
        } else {
            return str(self);
        }
    }
    str removesuffix(const str& _str) { return removesuffix(_str.data); }

    str removeprefix(const char* _str) {
        int _len = strlen(_str);
        int position = find(_str);
        if (position == 0) {
            return substr(_len, length);
        } else {
            return self;
        }
    }
    str removeprefix(const str& _str) { return removeprefix(_str.data); }

    // OPERATORS

    str& operator= (const str& other) {
        __overwrite_self__(other);
        return self;
    }

    str& operator= (const char chr) {
        str temp(chr);
        __overwrite_self__(temp);
        return self;
    }

    str& operator+= (str other) {
        return __add__(other);
    }

    str& operator+= (const char chr) {
        return self += str(chr);
    }

    str& operator+= (const char* _data) {
        return __insert__(length, _data);
    } 

    str operator+ (str other) {
        str temp = self;
        temp.__add__(other);
        return temp;
    }

    str operator* (int times) {
        str temp = self;
        for _range(times-1, idx) {
            temp += self;
        }
        return temp;
    }

    bool operator== (str& other) {
        return strcmp(data, other.c_str()) == 0;
    }

    // DESTRUCTOR

    char* begin() {
        return data;
    }

    char* end() {
        return data + length;
    }

    ~str () {
        free(data);
        destprint("<class: str at: 0x%p> destructed\n", this);
    }

    void verify() {
        
    }
};

str operator* (int times, str& self) {
    str temp = self;
    for _range(times-1, idx) {
        temp += self;
    }
    return temp;
}

template<typename T>
T& operator<< (T& buffer, const str& self) {
    buffer << self.data;
    return buffer;
}

std::string str_to_string(str _str) {
    return std::string(_str.data);
}

str string_to_str(std::string _str) {
    return str(_str.c_str());
}