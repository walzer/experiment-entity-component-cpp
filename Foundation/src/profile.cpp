#include "pch.h"

#define LOG_FILE_PATH			s_szFilePath
#define LOG_FILE_PATH_BAK		s_szFileBak
#define INFO_LEN				60			// �����ͷ�ĳ��� 

#if defined(WIN32)
// ��Win32 ��ʵ�� 
#include <windows.h>

#include <stdio.h>
#include <vector>
#include <time.h>

static char s_szFilePath[256]	= "";
static char s_szFileBak[256]    = "";

using namespace std;

typedef struct  
{
    float			fTotal;		// �ܹ����ĵ�ʱ�� 
    float           fMax;
    float           fMin;
    unsigned int    uTimes;		// �����õĴ��� 
    LARGE_INTEGER	uStart;
    const char *	pszInfo;
} T_ITEM;

static bool				s_bProfile;
static tm				s_tBegin;
LARGE_INTEGER			s_uStart;		
static vector<T_ITEM>	s_Items;

static LARGE_INTEGER	s_uFreq;	

void ProfileInit()
{
    // �����ָ�����ļ�Path���ҳ����ܷŵ��£����ļ�����Ϊ��ָ����Path����ʹ��Ĭ�ϵ�·�� 
    if (strlen(s_szFilePath) <= 0)
    {
        std::string fullPath = "/profile.txt";
        strcpy_s(s_szFilePath, sizeof(s_szFilePath), fullPath.c_str());

        std::string bakFullPath = "/profile_bak.txt";
        strcpy_s(s_szFileBak, sizeof(s_szFileBak), bakFullPath.c_str());
    }

    // ��ȡ��ʼʱ�ļ�ʱʱ�� 
    QueryPerformanceFrequency(&s_uFreq);
    QueryPerformanceCounter(&s_uStart);	

    // ��¼��ʼʱ����Ȼʱ�� 
    time_t ti = time(NULL);
    localtime_s(&s_tBegin, &ti);
    s_Items.clear();
    s_bProfile = true;
}

unsigned int ProfileStart(unsigned int uId, const char * pszInfo)
{
    if (! s_bProfile)
    {
        return uId;
    }
    unsigned int uCount = s_Items.size();

    if (uId && uId <= uCount)
    {
        // �Ѵ��� 
        QueryPerformanceCounter(&s_Items[uId - 1].uStart);
        return uId;
    }
    else
    {
        // ��û�д˼�ʱ 
        T_ITEM item;
        item.fTotal = 0.0f;
        item.fMax = 0.0f;
        item.fMin = 9999999.0f;
        item.uTimes = 0;
        item.pszInfo = pszInfo;
        QueryPerformanceCounter(&item.uStart);
        s_Items.push_back(item);
        return uCount + 1;
    }
}

void ProfileStop(unsigned int uId)
{
    if (! s_bProfile)
    {
        return;
    }
    unsigned int uCount = s_Items.size();

    if (uId && uId <= uCount)
    {
        LARGE_INTEGER uNow;
        QueryPerformanceCounter(&uNow);

        T_ITEM& item = s_Items[uId - 1];
        float temp = float(uNow.QuadPart - item.uStart.QuadPart);
        float tempTime = temp * 1000 / s_uFreq.QuadPart;
        item.fTotal += temp;
        item.fMax = (temp > item.fMax) ? temp : item.fMax;
        item.fMin = (temp < item.fMin) ? temp : item.fMin;
        ++item.uTimes;
    }
}

void ProfileUnInit()
{
    unsigned int uNum = s_Items.size();
    if (! uNum)
    {
        return;
    }
    FILE * pf = 0;
    fopen_s(&pf, LOG_FILE_PATH, "a+");
    if (! pf)
    {
        return;
    }

    char szBuf[256];
    int nCount;

    // ��ȡ����ʱ�� 
    LARGE_INTEGER uNow;
    QueryPerformanceCounter(&uNow);
    float fProfileLast = (float)(uNow.QuadPart - s_uStart.QuadPart);

    // �����ʼ��Ϣ 
    char szAsctime[256];
    asctime_s(szAsctime, &s_tBegin);
    nCount = sprintf_s(szBuf, "profile begin at %s\r\n", szAsctime);
    fwrite(szBuf, 1, nCount, pf);


    // �����ͷ 
    nCount = sprintf_s(szBuf, "%s", "|                       function info                       |  call num  |   min(ms)  |   max(ms)  |  total(ms) |   avg(ms)  |   percent  |\r\n");
    fwrite(szBuf, 1, nCount, pf);

    // ��ʼ��������� 
    for (unsigned int i = 0; i < uNum; ++i)
    {
        T_ITEM& item = s_Items[i];

        nCount = (item.pszInfo) ? sprintf_s(szBuf, "| %s", item.pszInfo) 
            : sprintf_s(szBuf, "|                             ");
        if (nCount > INFO_LEN)
        {
            szBuf[INFO_LEN + 1] = 0;
            nCount = INFO_LEN;
        }
        nCount = fwrite(szBuf, 1, nCount, pf);
        if (nCount < INFO_LEN)
        {
            nCount = INFO_LEN - nCount;
            memset(szBuf, ' ', nCount);
            szBuf[nCount + 1] = '\0';
            fwrite(szBuf, 1, nCount, pf);
        }

        // call num
        nCount = sprintf_s(szBuf, "| %10d ", item.uTimes);
        fwrite(szBuf, 1, nCount, pf);

        // min
        nCount = sprintf_s(szBuf, "| %10.3f ", item.fMin * 1000 / s_uFreq.QuadPart);
        fwrite(szBuf, 1, nCount, pf);

        // max
        nCount = sprintf_s(szBuf, "| %10.3f ", item.fMax * 1000 / s_uFreq.QuadPart);
        fwrite(szBuf, 1, nCount, pf);

        // total
        nCount = sprintf_s(szBuf, "| %10.3f ", item.fTotal * 1000 / s_uFreq.QuadPart);
        fwrite(szBuf, 1, nCount, pf);

        // avg
        nCount = sprintf_s(szBuf, "| %10.3f ", (item.uTimes) ? item.fTotal * 1000/ (s_uFreq.QuadPart * item.uTimes) : 0.0f);
        fwrite(szBuf, 1, nCount, pf);

        // percent
        nCount = sprintf_s(szBuf, "| %10.3f |\r\n", item.fTotal / fProfileLast);
        fwrite(szBuf, 1, nCount, pf);

    }

    // �����β 
    time_t ti = time(NULL);
    tm	tmNow;
    localtime_s(&tmNow, &ti);
    asctime_s(szAsctime, &tmNow);
    nCount = sprintf_s(szBuf, "profile continued %d s, end at %s\r\n", 
        (ti - mktime(&s_tBegin)), szAsctime);
    fwrite(szBuf, 1, nCount, pf);

    fflush(pf);
    fclose(pf);
    s_Items.clear();
    s_bProfile = false;
}

#else	// (CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM)

// ��Linux��ʵ�� 
#include <stdio.h>
#include <sys/time.h>
#include <vector>
#include <time.h>

static char s_szFilePath[256]	= "";
static char s_szFileBak[256]    = "";

using namespace std;

typedef struct  
{
    float			fTotal;		// �ܹ����ĵ�ʱ�� 
    float           fMax;
    float           fMin;
    unsigned int    uTimes;		// �����õĴ��� 
    struct timeval	tStart;		// ���һ�ε��õ�ʱ�� 
    const char *	pszInfo;
} T_ITEM;

static bool				s_bProfile;
static tm				s_tBegin;
struct timeval			s_tStart;		
static vector<T_ITEM>	s_Items;

static struct timezone	tz;	

void ProfileInit()
{
    // �����ָ�����ļ�Path���ҳ����ܷŵ��£����ļ�����Ϊ��ָ����Path����ʹ��Ĭ�ϵ�·�� 
    if (strlen(s_szFilePath) <= 0)
    {
        std::string fullPath = "/mnt/sdcard/profile.txt";
        strcpy(s_szFilePath, fullPath.c_str());

        std::string bakFullPath = "/mnt/sdcard/profile_bak.txt";
        strcpy(s_szFileBak, bakFullPath.c_str());
    }
    gettimeofday(&s_tStart, &tz);
    time_t ti = time(NULL);
    s_tBegin = *localtime(&ti);
    s_Items.clear();
    s_bProfile = true;
}

unsigned int ProfileStart(unsigned int uId, const char * pszInfo)
{
    if (! s_bProfile)
    {
        return uId;
    }
    unsigned int uCount = s_Items.size();

    if (uId && uId <= uCount)
    {
        // �Ѵ��� 
        gettimeofday(&s_Items[uId - 1].tStart, &tz);
        return uId;
    }
    else
    {
        // ��û�д˼�ʱ 
        T_ITEM item;
        item.fTotal = 0.0f;
        item.fMax = 0.0f;
        item.fMin = 9999999.0f;
        item.uTimes = 0;
        item.pszInfo = pszInfo;
        gettimeofday(&item.tStart, &tz);
        s_Items.push_back(item);
        return uCount + 1;
    }
}

void ProfileStop(unsigned int uId)
{
    if (! s_bProfile)
    {
        return;
    }
    unsigned int uCount = s_Items.size();

    if (uId && uId <= uCount)
    {
        struct timeval tNow;
        gettimeofday(&tNow, &tz);

        T_ITEM& item = s_Items[uId - 1];
        float temp = (tNow.tv_sec * 1000 * 1000 + tNow.tv_usec) - 
            (item.tStart.tv_sec * 1000 * 1000 + item.tStart.tv_usec);
        item.fTotal += temp;
        item.fMax = (temp > item.fMax) ? temp : item.fMax;
        item.fMin = (temp < item.fMin) ? temp : item.fMin;
        ++item.uTimes;
    }
}

void ProfileUnInit()
{
    unsigned int uNum = s_Items.size();
    if (! uNum)
    {
        return;
    }
    FILE * pf = fopen(LOG_FILE_PATH, "a+");
    if (! pf)
    {
        perfLog("Can't open file(%s)", LOG_FILE_PATH);
        return;
    }

    char szBuf[256];
    int nCount;

    // ��ȡ����ʱ�� 
    struct timeval tNow;
    gettimeofday(&tNow, &tz);
    float fProfileLast = (tNow.tv_sec * 1000 * 1000 + tNow.tv_usec) - (s_tStart.tv_sec * 1000 * 1000 + s_tStart.tv_usec);

    // �����ʼ��Ϣ 
    nCount = sprintf(szBuf, "profile begin at %s\r\n", asctime(&s_tBegin));
    fwrite(szBuf, 1, nCount, pf);

#define INFO_LEN	60

    // �����ͷ 
    nCount = sprintf(szBuf, "%s", "|                       function info                       |  call num  |   min(ms)  |   max(ms)  |  total(ms) |   avg(ms)  |   percent  |\r\n");
    fwrite(szBuf, 1, nCount, pf);

    // ��ʼ��������� 
    for (unsigned int i = 0; i < uNum; ++i)
    {
        T_ITEM& item = s_Items[i];
        
        nCount = (item.pszInfo) ? sprintf(szBuf, "| %s", item.pszInfo) 
            : sprintf(szBuf, "|                             ");
        if (nCount > INFO_LEN)
        {
            szBuf[INFO_LEN + 1] = 0;
            nCount = INFO_LEN;
        }
        nCount = fwrite(szBuf, 1, nCount, pf);
        if (nCount < INFO_LEN)
        {
            nCount = INFO_LEN - nCount;
            memset(szBuf, ' ', nCount);
            szBuf[nCount + 1] = '\0';
            fwrite(szBuf, 1, nCount, pf);
        }

        // call num
        nCount = sprintf(szBuf, "| %10d ", item.uTimes);
        fwrite(szBuf, 1, nCount, pf);

        // min
        nCount = sprintf(szBuf, "| %10.3f ", item.fMin / (1000));
        fwrite(szBuf, 1, nCount, pf);

        // max
        nCount = sprintf(szBuf, "| %10.3f ", item.fMax / (1000));
        fwrite(szBuf, 1, nCount, pf);

        // total
        nCount = sprintf(szBuf, "| %10.3f ", item.fTotal / (1000));
        fwrite(szBuf, 1, nCount, pf);

        // avg
        nCount = sprintf(szBuf, "| %10.3f ", (item.uTimes) ? item.fTotal / (1000 * item.uTimes) : 0.0f);
        fwrite(szBuf, 1, nCount, pf);

        // percent
        nCount = sprintf(szBuf, "| %10.3f |\r\n", item.fTotal / fProfileLast);
        fwrite(szBuf, 1, nCount, pf);

    }

    // �����β 
    time_t ti = time(NULL);
    tm	tmNow = *localtime(&ti);
    nCount = sprintf(szBuf, "profile continued %ld s, end at %s\r\n", 
        (ti - mktime(&s_tBegin)), asctime(&tmNow));
    fwrite(szBuf, 1, nCount, pf);

    fflush(pf);
    fclose(pf);
    s_Items.clear();
    s_bProfile = false;
}
#endif	// (CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM)

void LogInfoLine(const char * pszFile, int nLine)
{
    FILE * pf = fopen(LOG_FILE_PATH, "a+");
    if (! pf)
    {
        return;
    }
    int nFileSize = 0;
    if (fseek(pf, 0, SEEK_END) == 0)
    {
        nFileSize = ftell(pf);
    }
    if (nFileSize >= 10*1024*1024)
    {
        fflush(pf);
        fclose(pf);
        remove(LOG_FILE_PATH_BAK);
        rename(LOG_FILE_PATH, LOG_FILE_PATH_BAK);
        pf = fopen(LOG_FILE_PATH, "w+");
        if (!pf)
        {
            return;
        }
    }

    char szBuf[512] = {0};
    unsigned int uTick; // ms
#if defined(WIN32)
    uTick = GetTickCount();
#else // (CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM)
    struct timeval tNow;
    gettimeofday(&tNow, &tz);
    uTick = tNow.tv_sec * 1000 + tNow.tv_usec / 1000;
#endif // (CC_PLATFORM_WIN32 == CC_TARGET_PLATFORM)
    int nLen = sprintf(szBuf, "%d: %8d: %s \r\n", uTick, nLine, pszFile);
    szBuf[511] = '\0';
    fwrite(szBuf, 1, nLen, pf);
    fflush(pf);
    fclose(pf);
}
