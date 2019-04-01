#ifndef XFILE_H_INCLUDED
#define XFILE_H_INCLUDED
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
typedef unsigned long ULONG;
const int LINE_LEN = 1024;

class xFile
{
public:
    xFile();
    xFile(const char* szFilename, const char* mode);
    virtual ~xFile();

    virtual bool Open(const char* szFilename, const char* mode);
    virtual ULONG Read(void* pBuffer, int size);
    virtual bool ReadString(char* szBuffer, int size);
    virtual char* ReadString();
    virtual void Write(const void* pBuffer, int nCount);
	virtual void WriteString(const char* sz);

    ULONG SeekToEnd();
    void SeekToBegin();
    virtual ULONG Seek(ULONG offset, int nFrom);
    virtual ULONG GetFileSize();
    virtual ULONG GetPosition() const;
    virtual char* GetFileName() const;
    virtual char* GetFileFmt() const;
    virtual void Flush();
    virtual void Close();
	FILE* pFile;
private:
    char* mFilename;
    char *pString;
    
};

#endif /*XFILE_H_INCLUDED*/


