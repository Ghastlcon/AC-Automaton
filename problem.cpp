#include <iostream>
#include <string>
#include <cstdio>
#define MAXPOINT 1000
using namespace std;

int n;
string x[MAXPOINT];
string FileName,tmp;

#ifdef WIN32
    #include <io.h>

    void EnumFile(void)
    {
        _finddata_t v;
        int h;

        tmp = "../../data/" + FileName + "/*.in";
        h = _findfirst(tmp.c_str(), &v);
        do
            if(!(v.attrib & _A_SUBDIR))
            {
                x[n   ]  = "../../data/"+FileName+"/";
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

        tmp = "../../data/" + FileName + "/";
        dir = opendir(tmp.c_str());
        while ((file = readdir(dir)))
            if ((file->d_type & DT_REG) && is_in(file->d_name)) {
                x[n  ]  = "../../data/" + FileName + "/";
                x[n++] += file->d_name;
            }
    }

#endif

int MatchFile(void)
{
    FILE *a, *b;
    int i, c;

    tmp = FileName + ".in";
    b = fopen(tmp.c_str(), "r");
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
    tmp = FileName + ".out";
    o = fopen(tmp.c_str(), "w");
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

void GetFileName()
{
    FileName=__FILE__;
    int pos;
    #ifdef WIN32
        pos = FileName.rfind('\\');
    #else
        pos = FileName.rfind('/');
    #endif
    FileName.erase(0,pos+1);
    FileName.erase(FileName.size()-4,4);
}

int main(void)
{
    GetFileName();
    EnumFile();
    OutputFile(MatchFile());

    return 0;
}
