#include <iostream>
#include <string>
#include <cstdio>
#define MAXPOINT 1000
#define FILENAME "problem"
using namespace std;

int n;
string x[MAXPOINT];

#ifdef WIN32
    #include <io.h>

    void EnumFile(void)
    {
        _finddata_t v;
        int h;

        h = _findfirst("../../data/"FILENAME"/*.in", &v);
        do
            if(!(v.attrib & _A_SUBDIR))
            {
                x[n   ]  = "../../data/"FILENAME"/";
                x[n ++] += v.name;
            }
        while(!_findnext(h, &v));
        _findclose(h);

        return;
    }
#else

    #include <dirent.h>
    #include <cstring>
    bool is_in(const char *file) {
        int len = strlen(file);
        return file[len-1] == 'n' && file[len-2] == 'i' && file[len-3] == '.';
    }
    void EnumFile(void) {
        DIR *dir;
        struct dirent *file;

        dir = opendir("../../data/"FILENAME"/");
        while ((file = readdir(dir)))
            if ((file->d_type & DT_REG) && is_in(file->d_name)) {
                x[n  ]  = "../../data/"FILENAME"/";
                x[n++] += file->d_name;
            }
    }

#endif

int MatchFile(void)
{
    FILE *a, *b;
    int i, c;

    b = fopen(FILENAME".in", "r");
    for(i = 0;i < n;i ++)
    {
        a = fopen(x[i].c_str(), "r");
        try
        {
            while((c = fgetc(b)) != EOF)
                if(c != fgetc(a))
                    throw "";
            if(fgetc(a) != EOF)
                throw "";
        }
        catch(...)
        {
            rewind(b);
            fclose(a);
            continue;
        }

        rewind(b);
        fclose(a);
        break;
    }
    fclose(b);

    return i;
}

void OutputFile(int p)
{
    int k, c;
    string t, b[2];
    FILE *i, *o;

    b[0] = "out";
    b[1] = "ans";
    o = fopen(FILENAME".out", "w");
    for(k = 0;k < 2;k ++)
    {
        t = x[p].substr(0, x[p].size() - 2) + b[k];
        if(!(i = fopen(t.c_str(), "r")))
            continue;
        while((c = fgetc(i)) != EOF)
            fputc(c, o);
        fclose(i);
    }

    return;
}

int main(void)
{
    EnumFile();
    OutputFile(MatchFile());

    return 0;
}
