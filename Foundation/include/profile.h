#ifndef __PROFILE_H_YANGWS_20100925__
#define __PROFILE_H_YANGWS_20100925__

#define LogCheckPoint	LogInfoLine(__FILE__, __LINE__)
#define LogInfo(str)	LogInfoLine(str, __LINE__)

#define _ENABLE_CHECK_POINT_ 1
#if defined(_ENABLE_CHECK_POINT_)
void LogInfoLine(const char * pszFile, int nLine);
#else  // _ENABLE_PROFILE_
#define LogInfoLine(pszFile, nLine)
#endif // _ENABLE_PROFILE_

#define ProfileFunction(func, info) \
	{ \
	static unsigned int s_funcId; \
	s_funcId = ProfileStart(s_funcId, info); \
	func; \
	ProfileStop(s_funcId); \
	}\

#define  _ENABLE_PROFILE_ 1
#ifdef _ENABLE_PROFILE_

/**
@brief	初始化Profile模块。 
*/
void ProfileInit();

/**
@brief	开始某个代码段的计时。 
@param	uId 代码段的编号，如果此函数第一次执行，则此函数须为0,此函数会自动分配一个Id。 
@param	pszInfo	此代码段的说明，最好为static const char 数组 
@return	此代码段的ID。 
*/
unsigned int ProfileStart(unsigned int uId, const char * pszInfo);
void ProfileStop(unsigned int uId);
void ProfileUnInit();

#else  // _ENABLE_PROFILE_

#define ProfileInit()
#define ProfileStart(uId, pszInfo)	0
#define ProfileStop(uId)
#define ProfileUnInit()

#endif // _ENABLE_PROFILE_

#endif	// end of __PROFILE_H_YANGWS_20100925__