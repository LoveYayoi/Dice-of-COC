#include "INItool.h"
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
using namespace ini;

char* LTrim(char * buff)
{
    while (*buff == '\t' || *buff == ' ')++buff;
    return buff;
}
void RTrim(char * buff)
{
    while (*buff != 0)++buff; --buff;
    while (*buff == '\t' || *buff == ' ')--buff;
    *(buff + 1) = 0;
}
char* trim(char * buff){ RTrim(buff); return LTrim(buff); }

#define buffmax 8096
void INItool::Analyse(istream&in)
{
    char buff[buffmax + 1];
    Comments commentses;
    Sections*nowsections = nullptr;
    while (!in.eof())
    {
        in.getline(buff, buffmax);
        buff[buffmax] = 0;//��ֹ�쳣..
        auto start = LTrim(buff);//ɾ��ǰ���հ�
        switch (*start)
        {
            //ע��
        case ';':
        case '#':
            commentses.push_back(start);
            break;
            //��
        case '[':
        {
                    auto end = start;
                    while (*end != 0)++end;//��ĩβ
                    ++start;
                    while (*end != ']'&&end != start)--end;
                    if (end == start)break;//��Ч����
                    *end = 0;
                    if (has(start))
                    {
                        nowsections = &get(start);
                    }
                    else
                    {
                        sectionses.emplace_back(start, commentses);//�½���
                        commentses.clear();
                        nowsections = &sectionses.back();
                    }
                    break;
        }
        default:
            //������Ч��
            auto str = start;
            while (*start != '=' && *start != 0)
                ++start;
            if (start == str //��key
                //|| *(tep + 1)==0//��ֵ,�������������,����ע�͵�
                || *start == 0 //����
                )break;
            //��
            *start = 0; ++start;
            if (nowsections == nullptr)break;
            nowsections->push_back({ trim(str), start, commentses });
            commentses.clear();
            break;
        }
    }
}

INItool::INItool(string filename)
:filename(filename)
{
    ifstream in(filename);
    if (!in.is_open())
        return;

    Analyse(in);
}

void INItool::load(string string)
{
    Analyse(stringstream(string));
}

bool INItool::has(std::string name)
{
    for (auto&s : sectionses)
    if (s.name == name)
        return true;
    return false;
}

Sections&INItool::operator[](string sectionsName)
{
    return get(sectionsName);
}

Sections& INItool::get(std::string sectionsName)
{
    for (auto&s : sectionses)
    if (s.name == sectionsName)
        return s;
    sectionses.emplace_back(sectionsName);
    return sectionses.back();
}

ostream&ini::operator<<(ostream& out, INItool&t)
{
    for (auto&y : t.sectionses)
        out << y;
    return out;
}



Parameters::Parameters(string key, string val, Comments commentses)
:key(key), val(val), commentses(commentses), noshow(false)
{}

Parameters::Parameters(std::string key)
: key(key), noshow(true)
{}

Parameters& Parameters::operator=(std::string val)
{
    this->val = val;
    return *this;
}

ostream& ini::operator<<(ostream& out, Comments&t)
{
    for (auto&y : t)
    {
        auto c = LTrim(const_cast<char*>(y.c_str()));
        if (*c != ';' && *c != '#')
            out << "; ";
        out << c << endl;
    }
    return out;
}
ostream& ini::operator<<(ostream& out, Parameters&t)
{
    out << t.commentses
        << t.key.c_str() << "=" << t.val.c_str()
        << endl;
    return out;
}

Parameters& ini::operator>>(Parameters& t, std::string s)
{
    s = t.val;
    return t;
}

Sections::Sections(std::string name) 
:name(name)
{}

Sections::Sections(string name, Comments commentses)
:name(name), commentses(commentses)
{}

Parameters& Sections::operator[](std::string parametersName)
{
    return get(parametersName, 0);
}

Parameters& Sections::get(std::string parametersName, int index)
{
    int thisIndex = 0; Parameters *tep;
    for (auto&v : *this)
    if (v.key == parametersName)
    {
        if (thisIndex == index) return v;//�ҵ�ָ�����еļ�,����ȥ
        else { ++thisIndex; tep = &v; }
    }
    if (thisIndex == 0)
    {
        this->emplace_back(parametersName);
        return this->back();
    }
    else return*tep;
}

ostream& ini::operator<<(ostream& out, Sections&t)
{
    if (t.size())
    {
        out << t.commentses
            << "[" << t.name.c_str() << "]" << endl;
        for (auto&p : t)
            out << p;
    }
    return out;
}
