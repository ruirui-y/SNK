#ifndef VIKEY_API
#define VIKEY_API

typedef enum 
{
	ViKeyAPP = 0,						//实用型加密狗
	ViKeySTD = 1,						//标准型加密狗
	ViKeyNET = 2,						//网络型加密狗
	ViKeyPRO = 3,						//专业型加密狗
	ViKeyWEB = 4,						//身份认证型加密狗
	ViKeyTIME = 5,						//时钟型加密狗，内部独立时钟
	ViKeyMultiFunctional = 0x0A,		//多功能加密狗  支持软件加密 支持文档加密
	ViKeyMultiFunctionalTime = 0x0B,	//多功能时钟加密狗
	ViKeyInvalid	//无效类型
} VikeyType;


#define ViKeyNoLevel		0	//匿名权限(没有登录过加密狗时，此时权限为匿名权限)
#define ViKeyUserLevel		1	//用户权限(调用VikeyUserLogin登录加密狗成功后，此时权限为用户权限)
#define ViKeyAdminLevel		2	//管理员权限(调用VikeyAdminLogin登录加密狗成功后，此时权限为管理员权限)

#define VIKEY_SUCCESS						0x00000000 //成功
#define VIKEY_ERROR_NO_VIKEY				0x80000001 //没有找到ViKey加密锁
#define VIKEY_ERROR_INVALID_PASSWORD		0x80000002 //密码错误
#define VIKEY_ERROR_NEED_FIND				0x80000003 //请先查找加密锁
#define VIKEY_ERROR_INVALID_INDEX			0x80000004 //无效的句柄
#define VIKEY_ERROR_INVALID_VALUE			0x80000005 //数值错误
#define VIKEY_ERROR_INVALID_KEY				0x80000006 //密钥无效
#define VIKEY_ERROR_GET_VALUE				0x80000007 //读取信息错误
#define VIKEY_ERROR_SET_VALUE				0x80000008 //设置信息错误
#define VIKEY_ERROR_NO_CHANCE				0x80000009 //没有机会
#define VIKEY_ERROR_NO_TAUTHORITY			0x8000000A //权限不足
#define VIKEY_ERROR_INVALID_ADDR_OR_SIZE	0x8000000B //地址或长度错误
#define VIKEY_ERROR_RANDOM					0x8000000C //获取随机数错误
#define VIKEY_ERROR_SEED					0x8000000D //获取种子错误
#define VIKEY_ERROR_CONNECTION				0x8000000E //通信错误
#define VIKEY_ERROR_CALCULATE				0x8000000F //算法或计算错误
#define VIKEY_ERROR_MODULE					0x80000010 //计数器错误
#define VIKEY_ERROR_GENERATE_NEW_PASSWORD	0x80000011 //产生密码错误
#define VIKEY_ERROR_ENCRYPT_FAILED			0x80000012 //加密数据错误
#define VIKEY_ERROR_DECRYPT_FAILED			0x80000013 //解密数据错误
#define VIKEY_ERROR_ALREADY_LOCKED			0x80000014 //ViKey加密锁已经被锁定
#define VIKEY_ERROR_UNKNOWN_COMMAND			0x80000015 //无效的命令
#define VIKEY_ERROR_NO_SUPPORT				0x80000016 //当前ViKey加密锁不支持此功能
#define VIKEY_ERROR_CATCH					0x80000017 //发生异常
#define VIKEY_ERROR_GET_USBDATA				0x80000018 //读取USB数据错误
#define VIKEY_ERROR_SET_USBDATA				0x80000019 //设置USB数据错误
#define VIKEY_ERROR_TIME_MODULE				0x8000001A //硬件时钟模块异常
#define VIKEY_ERROR_TIME_OUTAGE				0x8000001B //硬件时钟发生过断电，时间可能不准确，需校准时间
#define VIKEY_ERROR_MAX_CONNECTION			0x8000001C //加密狗达到最大通信链接数
#define VIKEY_ERROR_COMMUNICATION			0x8000001D //加密狗通信数据错误
#define VIKEY_ERROR_TIME					0x8000001E //加密狗时间错误	需校准时间
#define VIKEY_ERROR_TIME_EXPIRE				0x8000001F //加密狗时间限制已经到期
#define VIKEY_ERROR_TIME_DISORDER			0x80000020 //加密狗时间失调	需校准时间
#define VIKEY_ERROR_API_KEY					0x80000021 //API密钥错误
#define VIKEY_ERROR_DYNAMIC_CODE			0x80000022 //动态码错误 需要重新生成动态码
#define VIKEY_ERROR_USER_SN					0x80000023 //加密狗的用户SN错误
#define VIKEY_ERROR_NOT_ACTIVE				0x80000024 //加密狗尚未激活
#define VIKEY_ERROR_UNSET_API_KEY			0x80000025 //没有设置API密钥
#define VIKEY_ERROR_API_KEY_CALC_1			0x80000026 //计算API密钥错误1
#define VIKEY_ERROR_API_KEY_CALC_2			0x80000027 //计算API密钥错误2
#define VIKEY_ERROR_CHECK_API_KEY			0x80000028 //API密钥检测错误
#define VIKEY_ERROR_UPDATE_CODE_LEN			0x80000029 //续期码(或授权码)长度错误
#define VIKEY_ERROR_UPDATE_CODE_CHECK		0x8000002A //续期码(或授权码)与动态码不匹配
#define VIKEY_ERROR_UPDATE_CODE_HARD		0x8000002B //续期码(或授权码)与加密狗不匹配
#define VIKEY_ERROR_UPDATE_CODE_PWD			0x8000002C //续期码(或授权码)与加密狗密码不匹配
#define VIKEY_ERROR_UPDATE_CODE_VALUE		0x8000002D //续期码(或授权码)的初始化值错误
#define VIKEY_ERROR_UPDATE_CODE_OTHER		0x8000002E //续期码(或授权码)的其他错误


#define VIKEY_ERROR_INIT_USB				0x80001021 //安卓系统-初始化USB失败
#define VIKEY_ERROR_OPEN_USB				0x80001022 //安卓系统-打开USB设备失败
#define VIKEY_ERROR_UNKNOWN_ERROR			0xFFFFFFFF //未知错误

//LED灯状态定义
#define LED_OFF			           0 //常灭
#define LED_ON			           1 //常亮
#define LED_BLINK		           2 //灯闪
#define LED_OFF_BLINK		       3 //平时常灭-通信时灯闪
#define LED_ON_BLINK		       4 //平时常亮-通信时灯闪

//加解密运算模式
#define VAM_ECB 0  
#define VAM_CBC 1  
#define VAM_OFB 2  
#define VAM_CTR 3 

//加解密填充模式
#define VPadding_None		0
#define VPadding_PKCS7		1
#define VPadding_Zeros		2
#define VPadding_ANSIX923	3
#define VPadding_ISO10126	4

#ifndef _MSC_VER
typedef 	unsigned short 		WORD;
typedef 	unsigned int   		DWORD;
typedef   	unsigned char  		BYTE;
typedef   	unsigned char  		CHAR;
typedef 	unsigned short 		WCHAR;
#define __stdcall 
#define CALLBACK 
#endif

#ifdef __cplusplus
extern "C"{
#endif

//【说明】为了让程序员对以下函数一目了然，这里只列出函数的原型，对于函数功能和参数的解释在帮助手册中列出。
//【说明】请到开发包“1-[文档及视频教程]”目录下查看《ViKey系列加密狗使用说明手册》文档。

/*********For ViKeyAPP、ViKeySTD、ViKeyNet、ViKeyPRO、ViKeyTime APIs**********/
/*****************************************************************************/
/*                                      API                                  */
/*****************************************************************************/

	
/** 
 * @说明   获取接口库件版本
 * @param  pVersion [out]    返回接口库版本  需要提供16字节的缓存
 * @return 0     表示操作成功
 */
DWORD __stdcall  VikeyGetLibraryVersion(char * pVersion);

/** 
* @说明   查找加密狗之前设置UserSN   硬件版本7.4.6版本(含)以后的加密狗必须输入正确的UserSN才能查找到加密狗
* @param  pUserSN   [in]             UserSN在ViKey加密管理工具的->帮助中心页面->复制出来
                             

 * @return 0     表示操作成功
 */
DWORD __stdcall  VikeySetUserSN(char * pUserSN);

/** 
 * @说明   查找加密锁VikeyFind 使用其他API前必须先调用此函数。
 * @param  pdwCount [out]     如果查找到系统中存在加密狗，返回查找到加密狗的个数
 * @return 0     表示系统中存在ViKey加密狗。
 */
DWORD __stdcall VikeyFind(DWORD* pdwCount);

/** 
 * @说明   查找加密锁VikeyFindEx 使用其他API前必须先调用此函数。
 * @param  pdwCount [out]     如果查找到系统中存在加密狗,返回查找到加密狗的个数（排除虚拟加密狗）
 * @return 0     表示系统中存在ViKey加密狗。
 */
DWORD __stdcall VikeyFindEx(DWORD* pdwCount);

/** 
 * @说明   api操作加密前要进行设置ApiKey(API密钥)，只有设置ApiKey成功后才可能对加密狗进行操作，API密钥使用加密管理工具生成。
 * @param  Index     [in]     指定要操作加密狗的序号
 * @param  pApiKey   [in]     API密钥字符串
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySetApiKey(WORD Index, char *pApiKey);


/** 
 * @说明   清理缓存。当不在使用加密狗相关的API时，需要调用此函数来清理缓存。
 * @说明	         建议在程序退出前，调用此函数。
* @说明	             注意：如果调用此函数后，想继续使用加密狗相关API，需要再次调用VikeyFind
 * @return 0     表示清理成功。
 */
DWORD __stdcall VikeyUninitialization();


typedef void (CALLBACK  *PCallBackViKeyArrival)(DWORD dwHID);
typedef void (CALLBACK  *PCallBackViKeyRemove)(DWORD dwHID);

/** 
 * @说明   注册加密锁插拔通知。当检测到加密狗插拔时会主动通知回调函数。只有DLL支持此函数
* @return 0     表示注册成功。
 */
DWORD __stdcall RegisterViKeyCallBack(PCallBackViKeyArrival pfnCallBackViKeyArrival, PCallBackViKeyRemove pfnCallBackViKeyRemove);


/** 
 * @说明   获取加密狗的硬件ID,加密狗的硬件ID是加密狗的唯一标识，每个加密狗的硬件ID都不一样。
 * @param  Index    [in]     指定要操作加密狗的序号
 * @param  pdwHID   [out]    返回加密狗的硬件ID
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyGetHID(WORD Index, DWORD *pdwHID);

/** 
 * @说明   获取加密狗的硬件型号。
 * @param  Index    [in]     指定要操作加密狗的序号
 * @param  pdwHID   [out]    返回加密狗的硬件型号。
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyGetType(WORD Index, VikeyType *pType);

/** 
 * @说明   获取加密狗的当前权限
 * @param  Index    [in]     指定要操作加密狗的序号
 * @param  pLevel   [out]    返回加密狗的当前权限 0表示加密狗尚未登录  1表示加密狗是用户权限  2表示加密狗是管理员权限
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyGetLevel(WORD Index, BYTE *pLevel);

/** 
 * @说明   获取加密狗的硬件版本
 * @param  Index    [in]     指定要操作加密狗的序号
 * @param  pVersion [out]    返回加密狗的硬件版本  需要提供16字节的缓存
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyGetFirmVersion(WORD Index, char * pVersion);

/** 
 * @说明   获取加密狗的用户数据区大小
 * @param  Index    [in]     指定要操作加密狗的序号
 * @param  pBytes [out]      返回用户数据区大小 单位是字节
 * @return 0     表示操作成功
 */
DWORD __stdcall  VikeyGetUserDataSize(WORD Index, DWORD *pBytes);


/** 
 * @说明   获取加密狗的管理员数据区大小
 * @param  Index    [in]     指定要操作加密狗的序号
 * @param  pBytes [out]      返回管理员数据区大小 单位是字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyGetAdminDataSize(WORD Index, DWORD *pBytes);

/** 
 * @说明   设置机加密狗的产品名称
 * @param  Index    [in]     指定要操作加密狗的序号
 * @param  szName   [in]    16个字节的产品名称
 * @return 0     表示操作成功
 */
DWORD __stdcall  VikeySetPtroductName(WORD Index, WCHAR szName[16]);
DWORD __stdcall  VikeySetPtroductNameA(WORD Index, char szName[16]); //只能设置英文

/** 
 * @说明   获取机加密狗的产品名称
 * @param  Index    [in]     指定要操作加密狗的序号
 * @param  szName   [out]    16个字节的产品名称
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyGetPtroductName(WORD Index, WCHAR szName[16]);
DWORD __stdcall VikeyGetPtroductNameA(WORD Index, char szName[16]);

//登陆、注销加密狗
//加密狗密码长度8个字符 字母或数字

/** 
 * @说明   以用户权限登录加密狗
 * @param  Index    [in]     指定要操作加密狗的序号
 * @param  szName   [in]     8字节的用户密码
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyUserLogin(WORD Index, char * pUserPassWord);

/** 
 * @说明   以管理员权限登录加密狗
 * @param  Index    [in]     指定要操作加密狗的序号
 * @param  szName   [in]     8字节的管理员密码
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAdminLogin(WORD Index, char * pAdminPassWord);

/** 
 * @说明   注销登录加密狗 注销后的加密狗权限为0 可以理解为取消登录状态
 * @param  Index    [in]     指定要操作加密狗的序号
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyLogoff(WORD Index);

//设置密码尝试次数、修改密码

/** 
 * @说明   设置用户权限密码最大错误尝试次数
 * @param  Index      [in]     指定要操作加密狗的序号
 * @param  cAttempt   [in]     用户权限密码最大错误尝试次数  如果设置为0表示无限制
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySetUserPassWordAttempt(WORD Index, BYTE cAttempt);

/** 
 * @说明   设置管理员权限密码最大错误尝试次数
 * @param  Index      [in]     指定要操作加密狗的序号
 * @param  cAttempt   [in]     管理员权限密码最大错误尝试次数  如果设置为0表示无限制
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySetAdminPassWordAttempt(WORD Index, BYTE cAttempt);

/** 
 * @说明   获取用户权限密码当前尝试次数和最大错误尝试次数
 * @param  Index              [in]      指定要操作加密狗的序号
 * @param  pcCurrentAttempt   [out]     用户权限密码当前误尝试次数  
 * @param  pcMaxAttempt       [out]     用户权限密码最大错误尝试次数  如果设置为0表示无限制
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyGetUserPassWordAttempt(WORD Index, BYTE *pcCurrentAttempt, BYTE *pcMaxAttempt);


/** 
 * @说明   获取管理员权限密码当前尝试次数和最大错误尝试次数
 * @param  Index              [in]      指定要操作加密狗的序号
 * @param  pcCurrentAttempt   [out]     管理员权限密码当前误尝试次数  
 * @param  pcMaxAttempt       [out]     管理员权限密码最大错误尝试次数  如果设置为0表示无限制
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyGetAdminPassWordAttempt(WORD Index, BYTE *pcCurrentAttempt, BYTE *pcMaxAttempt);

/** 
 * @说明   设置加密狗的用户权限密码    注意调用此函数时请确保已经用户登录或管理员登录  否则会调用失败
 * @param  Index              [in]     指定要操作加密狗的序号
 * @param  pNewUserPassWord   [in]     8字节的用户权限密码 
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySetUserPassword(WORD Index, char * pNewUserPassWord);

/** 
 * @说明   设置加密狗的管理员权限密码  注意调用此函数时请确保已经管理员登录  否则会调用失败
 * @param  Index              [in]     指定要操作加密狗的序号
 * @param  pNewUserPassWord   [in]     8字节的管理员权限密码 
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySetAdminPassword(WORD Index, char * pNewAdminPassWord);

/** 
 * @说明   设置加密狗的用户权限密码和管理员权限密码  注意调用此函数时确保已经管理员权限登录  否则会调用失败
 * @param  Index              [in]     指定要操作加密狗的序号
 * @param  pNewUserPassWord   [in]     8字节的用户权限密码 
 * @param  pNewAdminPassWord  [in]     8字节的管理员权限密码
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyResetPassword(WORD Index, char * pNewUserPassWord, char * pNewAdminPassWord);

/** 
 * @说明   设置LED灯的状态    硬件版本7.4以上支持此功能
 * @param  Index              [in]     指定要操作加密狗的序号
 * @param  cLedState		  [in]     0 ->灯灭 1 ->灯亮 2 ->灯闪
 * @return 0     表示操作成功		   
 */									   
DWORD __stdcall VikeySetLED(WORD Index, BYTE cLedState);


//软件ID读写
//软件ID长度8个字符 字母或数字

/** 
 * @说明   设置加密狗的8个字节软件ID字符串
 * @param  Index              [in]     指定要操作加密狗的序号
 * @param  pSoftIDString      [in]     8个字节软件ID字符串
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySetSoftIDString(WORD Index, char * pSoftIDString);

/** 
 * @说明   获取加密狗的8个字节软件ID字符串
 * @param  Index              [in]     指定要操作加密狗的序号
 * @param  pSoftIDString      [out]    8个字节软件ID字符串
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyGetSoftIDString(WORD Index, char * pSoftIDString);

//读写数据
/** 
 * @说明   读取加密狗中的数据
 * @param  Index       [in]     指定要操作加密狗的序号
 * @param  Addr        [in]     读取的地址 起始位置
 * @param  Length      [in]     读取的数据长度
 * @param  buffer      [out]    返回读取的数据内容
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyReadData(WORD Index, DWORD Addr, DWORD Length, BYTE * buffer);


/** 
 * @说明   向加密狗中的写入数据
 * @param  Index       [in]     指定要操作加密狗的序号
 * @param  Addr        [in]     读取的地址 起始位置
 * @param  Length      [in]     读取的数据长度
 * @param  buffer      [in]     写入的数据内容
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyWriteData(WORD Index, DWORD Addr, DWORD Length, BYTE * buffer);

/** 
 * @说明   从加密狗中获取4个双字的随机数
 * @param  Index          [in]      指定要操作加密狗的序号
 * @param  pwRandom1      [out]     随机数1
 * @param  pwRandom2      [out]     随机数2
 * @param  pwRandom3      [out]     随机数3
 * @param  pwRandom4      [out]     随机数4
 * @return 0     表示操作成功
 */
DWORD __stdcall ViKeyRandom(WORD Index, WORD* pwRandom1, WORD* pwRandom2, WORD* pwRandom3, WORD* pwRandom4);

/** 
 * @说明   从加密狗中获取dwLength个随机数字节
 * @param  Index          [in]      指定要操作加密狗的序号
 * @param  dwLength       [in]      获取随机数字节的长度
 * @param  pRandoms       [out]     随机数字节
 * @return 0     表示操作成功
 */
DWORD __stdcall ViKeyRandomBytes(WORD Index, DWORD dwLength, BYTE* pRandoms);

//计数器模块
/** 
 * @说明   设置加密狗中的计数器
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  wModuleIndex   [in]     计数器的序号 从0开始
 * @param  wValue         [in]     计数器中的初始值
 * @param  wMode          [in]     计数器的模式   1：允许递减  0：不允许递减
 * @return 0     表示操作成功
 */
DWORD __stdcall ViKeySetModule(WORD Index, WORD wModuleIndex, WORD wValue, WORD wMode);

/** 
 * @说明   读取加密狗中的计数器
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  wModuleIndex   [in]     计数器的序号 从0开始
 * @param  pwValue        [out]    计数器中的当前值
 * @return 0     表示操作成功
 */
DWORD __stdcall ViKeyGetModule(WORD Index, WORD wModuleIndex, WORD* pwValue);

/** 
 * @说明   对加密狗中的计数器进行递减操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  wModuleIndex   [in]     计数器的序号 从0开始
 * @return 0     表示操作成功
 */
DWORD __stdcall ViKeyDecraseModule(WORD Index, WORD wModuleIndex);

/** 
 * @说明   对加密狗中的计数器进行检测
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  wModuleIndex   [in]     计数器的序号 从0开始
 * @param  IsZero         [out]    返回计数器中的当前值是否为0
 * @param  CanDecrase     [out]    返回计数器是否允许递减
 * @return 0     表示操作成功
 */
DWORD __stdcall ViKeyCheckModule(WORD Index, WORD wModuleIndex, WORD *IsZero, WORD* CanDecrase);

//自动打开网页接口
/** 
 * @说明   设置加密狗中自动打开网址
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pUrl           [in]     网址
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySetAutoRunUrl(WORD Index, BYTE *pUrl);

/** 
 * @说明   读取加密狗中自动打开网址
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pUrl           [out]    网址
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyGetAutoRunUrl(WORD Index, BYTE *pUrl);

//For ViKeyNET、ViKeyPRO
/** 
 * @说明   设置网络加密狗中最大链接的客户端数量
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwCount        [in]     最大链接的客户端数量
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySetMaxClientCount(WORD Index, WORD dwCount);

/** 
 * @说明   获取网络加密狗中最大链接的客户端数量
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pdwCount       [out]    返回最大链接的客户端数量
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyGetMaxClientCount(WORD Index, WORD* pdwCount);

//For ViKeyWEB、ViKeyPRO
/** 
 * @说明   执行加密狗中MD5算法
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  length        [in]     数据长度
 * @param  pText         [in]     数据内容
 * @param  pResult       [out]    返回MD5算法结果  16字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyMD5(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   设置加密狗中HMAC-MD5算法的密钥
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  pMD5key       [in]     密钥内容 长度32字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySetMD5Key(WORD Index, BYTE * pMD5key);

/** 
 * @说明   执行加密狗中HMAC-MD5算法
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  length        [in]     数据长度
 * @param  pText         [in]     数据内容
 * @param  pResult       [out]    返回HMAC-MD5算法的加密结果
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyHmacMD5(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   执行加密狗中SHA1算法
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  length        [in]     数据长度
 * @param  pText         [in]     数据内容
 * @param  pResult       [out]    返回SHA1算法结果  20字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySHA1(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   执行加密狗中SHA224算法
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  length        [in]     数据长度
 * @param  pText         [in]     数据内容
 * @param  pResult       [out]    返回SHA1算法结果  28字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySHA224(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   执行加密狗中SHA256算法
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  length        [in]     数据长度
 * @param  pText         [in]     数据内容
 * @param  pResult       [out]    返回SHA1算法结果  32字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySHA256(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   执行加密狗中SHA256算法
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  length        [in]     数据长度
 * @param  pText         [in]     数据内容
 * @param  pResult       [out]    返回SHA1算法结果  48字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySHA384(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   执行加密狗中SHA512算法
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  length        [in]     数据长度
 * @param  pText         [in]     数据内容
 * @param  pResult       [out]    返回SHA1算法结果  64字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySHA512(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   设置加密狗中HMAC-SHA1算法的密钥
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  pSHA1key      [in]     密钥内容 长度32字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySetSHA1Key(WORD Index, BYTE * pSHA1key);

/** 
 * @说明   执行加密狗中HMAC-SHA1算法
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  length        [in]     数据长度
 * @param  pText         [in]     数据内容
 * @param  pResult       [out]    返回HMAC-SHA1算法的加密结果
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyHmacSHA1(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   执行加密狗中HMAC-SHA224算法
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  length        [in]     数据长度
 * @param  pText         [in]     数据内容
 * @param  pResult       [out]    返回HMAC-SHA224算法的加密结果
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyHmacSHA224(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   执行加密狗中HMAC-SHA256算法
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  length        [in]     数据长度
 * @param  pText         [in]     数据内容
 * @param  pResult       [out]    返回HMAC-SHA256算法的加密结果
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyHmacSHA256(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   执行加密狗中HMAC-SHA384算法
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  length        [in]     数据长度
 * @param  pText         [in]     数据内容
 * @param  pResult       [out]    返回HMAC-SHA384算法的加密结果
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyHmacSHA384(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   执行加密狗中HMAC-SHA512算法
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  length        [in]     数据长度
 * @param  pText         [in]     数据内容
 * @param  pResult       [out]    返回HMAC-SHA512算法的加密结果
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyHmacSHA512(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   执行加密狗国密SM3算法
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  length        [in]     输入数据的长度
 * @param  pText         [in]     输入数据的内容
 * @param  pResult       [out]    返回SM3算法结果  长度为32字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM3(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   设置加密狗中HMAC-SM3算法的密钥
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  pMD5key       [in]     密钥内容 长度32字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySetSM3Key(WORD Index, BYTE * pSm3Key);

/** 
 * @说明   执行加密狗中HMAC-SM3算法
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  length        [in]     数据长度
 * @param  pText         [in]     数据内容
 * @param  pResult       [out]    返回HMAC-SM3算法的加密结果
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyHmacSM3(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   设置加密狗中DES算法中的密钥
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pKey           [in]     DES算法中的密钥 密钥长度固定为8个字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyDesSetKey(WORD Index, BYTE * pKey);

/** 
 * @说明   设置加密狗中DES算法中的IV
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pIV            [in]     DES算法中的IV 长度固定为8个字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyDesSetIV(WORD Index, BYTE * pIV);

/** 
 * @说明   执行加密狗中DES算法中的加密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  length         [in]     数据长度 明文长度必须为8的倍数
 * @param  pText          [in]     要加密的数据内容
 * @param  pResult        [out]    返回加密数据结果
 * @param  OutLength      [out]    返回加密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyDesEncrypt(WORD Index, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中DES算法中的解密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  length         [in]     数据长度 密文文长度必须为8的倍数
 * @param  pText          [in]     要解密的数据内容
 * @param  pResult        [out]    返回解密数据结果
 * @param  OutLength      [out]    返回解密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyDesDecrypt(WORD Index, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中DES算法中的加密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     数据长度 明文长度必须为8的倍数
 * @param  pText          [in]     要加密的数据内容
 * @param  pResult        [out]    返回加密数据结果
 * @param  OutLength      [out]    返回加密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyDesEncryptEx(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中DES算法中的解密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     数据长度 密文文长度必须为8的倍数
 * @param  pText          [in]     要解密的数据内容
 * @param  pResult        [out]    返回解密数据结果
 * @param  OutLength      [out]    返回解密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyDesDecryptEx(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

//加解密
typedef enum 
{
	Des3KeyLength16 = 0,
	Des3KeyLength24 = 1
}Des3KeyLengthType;
/** 
 * @说明   设置加密狗中3DES算法中的密钥
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pKey           [in]     3DES算法中的密钥
 * @param  KeyType        [in]     3DES算法中的密钥类型  0表示密钥长度16字节 1表示密钥长度24字节
 * @return 0     表示操作成功
 */
DWORD __stdcall Vikey3DesSetKey(WORD Index, BYTE * pKey, Des3KeyLengthType KeyType);

/** 
 * @说明   设置加密狗中3DES-128算法中的密钥
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pKey           [in]     3DES-128算法中的密钥
 * @param  KeyType        [in]     3DES-128算法中的密钥类型 密钥长度16字节
 * @return 0     表示操作成功
 */

DWORD __stdcall Vikey3Des128SetKey(WORD Index, BYTE * pKey);
/** 
 * @说明   设置加密狗中3DES-192算法中的密钥
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pKey           [in]     3DES算法中的密钥
 * @param  KeyType        [in]     3DES算法中的密钥类型  密钥长度24字节
 * @return 0     表示操作成功
 */
DWORD __stdcall Vikey3Des192SetKey(WORD Index, BYTE * pKey);

/** 
 * @说明   设置加密狗中3DES-128算法中的IV
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pIV            [in]     3DES算法中的IV 长度固定位8个字节
  * @return 0     表示操作成功
 */
DWORD __stdcall Vikey3Des128SetIV(WORD Index, BYTE * pIV);

/** 
 * @说明   设置加密狗中3DES-192算法中的IV
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pIV            [in]     3DES算法中的IV 长度固定位8个字节
  * @return 0     表示操作成功
 */
DWORD __stdcall Vikey3Des192SetIV(WORD Index, BYTE * pIV);

/** 
 * @说明   执行加密狗中3DES算法中的加密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  length         [in]     数据长度 明文长度必须为8的倍数
 * @param  pText          [in]     要加密的数据内容
 * @param  pResult        [out]    返回加密数据结果
 * @return 0     表示操作成功
 */
DWORD __stdcall Vikey3DesEncrypt(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   执行加密狗中3DES算法中的解密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  length         [in]     数据长度 密文长度必须为8的倍数
 * @param  pText          [in]     要解密的数据内容
 * @param  pResult        [out]    返回解密数据结果
 * @return 0     表示操作成功
 */
DWORD __stdcall Vikey3DesDecrypt(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

/** 
 * @说明   执行加密狗中3DES-128算法中的加密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     数据长度 明文长度必须为8的倍数
 * @param  pText          [in]     要加密的数据内容
 * @param  pResult        [out]    返回加密数据结果
 * @param  OutLength      [out]    返回加密数据的长度
 * @return 0     表示操作成功
 */
DWORD __stdcall Vikey3Des128Encrypt(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD length, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中3DES-128算法中的解密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     数据长度 密文长度必须为8的倍数
 * @param  pText          [in]     要解密的数据内容
 * @param  pResult        [out]    返回解密数据结果
 * @param  OutLength      [out]    返回解密数据的长度
 * @return 0     表示操作成功
 */
DWORD __stdcall Vikey3Des128Decrypt(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD length, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中3DES-192算法中的加密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     数据长度 明文长度必须为8的倍数
 * @param  pText          [in]     要加密的数据内容
 * @param  pResult        [out]    返回加密数据结果
 * @return 0     表示操作成功
 */
DWORD __stdcall Vikey3Des192Encrypt(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD length, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中3DES-192算法中的解密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     数据长度 密文长度必须为8的倍数
 * @param  pText          [in]     要解密的数据内容
 * @param  pResult        [out]    返回解密数据结果
 * @return 0     表示操作成功
 */
DWORD __stdcall Vikey3Des192Decrypt(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD length, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   设置加密狗中AES-128算法中的密钥
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pKey           [in]     AES-128算法中的密钥 密钥长度固定为16个字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAesSetKey(WORD Index, BYTE * pKey);

/** 
 * @说明   设置加密狗中AES-128算法中的IV
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pIV            [in]     AES-128算法中的IV 长度固定为16个字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAesSetIV(WORD Index, BYTE * pIV);

/** 
 * @说明   执行加密狗中AES-128算法中的加密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  length         [in]     数据长度 长度必须为16的倍数
 * @param  pText          [in]     要加密的数据内容

 * @param  pResult        [out]    返回加密数据结果
 * @param  OutLength      [out]    返回加密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAesEncrypt(WORD Index, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中AES-128算法中的解密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  length         [in]     密文数据长度 长度必须为16的倍数
 * @param  pText          [in]     要解密的密文数据内容

 * @param  pResult        [out]    返回解密数据结果
 * @param  OutLength      [out]    返回解密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAesDecrypt(WORD Index, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中AES-128算法中的加密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     数据长度 长度必须为16的倍数
 * @param  pText          [in]     要加密的数据内容

 * @param  pResult        [out]    返回加密数据结果
 * @param  OutLength      [out]    返回加密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAes128Encrypt(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中AES-128算法中的解密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     密文数据长度 长度必须为16的倍数
 * @param  pText          [in]     要解密的密文数据内容

 * @param  pResult        [out]    返回解密数据结果
 * @param  OutLength      [out]    返回解密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAes128Decrypt(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   设置加密狗中AES-192算法中的密钥
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pKey           [in]     AES-192算法中的密钥 密钥长度固定为24个字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAes192SetKey(WORD Index, BYTE * pKey);

/** 
 * @说明   设置加密狗中AES-192算法中的IV
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pIV            [in]     AES-256算法中的IV 长度固定为16个字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAes192SetIV(WORD Index, BYTE * pIV);

/** 
 * @说明   执行加密狗中AES-192算法中的加密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     数据长度 长度必须为16的倍数
 * @param  pText          [in]     要加密的数据内容

 * @param  pResult        [out]    返回加密数据结果
 * @param  OutLength      [out]    返回加密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAes192Encrypt(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中AES-192算法中的解密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     密文数据长度 长度必须为16的倍数
 * @param  pText          [in]     要解密的密文数据内容

 * @param  pResult        [out]    返回解密数据结果
 * @param  OutLength      [out]    返回解密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAes192Decrypt(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   设置加密狗中AES-256算法中的密钥
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pKey           [in]     AES-256算法中的密钥 密钥长度固定为32个字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAes256SetKey(WORD Index, BYTE * pKey);

/** 
 * @说明   设置加密狗中AES-256算法中的IV
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pIV            [in]     AES-256算法中的IV 长度固定为16个字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAes256SetIV(WORD Index, BYTE * pIV);

/** 
 * @说明   执行加密狗中AES-256算法中的加密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     数据长度 长度必须为16的倍数
 * @param  pText          [in]     要加密的数据内容

 * @param  pResult        [out]    返回加密数据结果
 * @param  OutLength      [out]    返回加密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAes256Encrypt(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中AES-256算法中的解密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     密文数据长度 长度必须为16的倍数
 * @param  pText          [in]     要解密的密文数据内容

 * @param  pResult        [out]    返回解密数据结果
 * @param  OutLength      [out]    返回解密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyAes256Decrypt(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   设置加密狗中SM1-128算法中的密钥
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pKey           [in]     SM1算法中的密钥 密钥长度固定为16个字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM1SetKey(WORD Index, BYTE * pKey);

/** 
 * @说明   设置加密狗中SM1-128算法中的IV
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pIV            [in]     SM1算法中的IV 长度固定为16个字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM1SetIV(WORD Index, BYTE * pIV);

/** 
 * @说明   执行加密狗中SM1-128算法中的加密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     数据长度 长度必须为16的倍数
 * @param  pText          [in]     要加密的数据内容

 * @param  pResult        [out]    返回加密数据结果
 * @param  OutLength      [out]    返回加密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM1Encrypt(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中SM1-128算法中的解密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     密文数据长度 长度必须为16的倍数
 * @param  pText          [in]     要解密的密文数据内容

 * @param  pResult        [out]    返回解密数据结果
 * @param  OutLength      [out]    返回解密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM1Decrypt(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   设置加密狗中SM1-256算法中的密钥
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pKey           [in]     SM1算法中的密钥 密钥长度固定为32个字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM1SetKey256(WORD Index, BYTE * pKey);

/** 
 * @说明   设置加密狗中SM1-256算法中的IV
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pIV            [in]     SM1算法中的IV 长度固定为16个字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM1SetIV256(WORD Index, BYTE * pIV);

/** 
 * @说明   执行加密狗中SM1-256算法中的加密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     数据长度 长度必须为16的倍数
 * @param  pText          [in]     要加密的数据内容

 * @param  pResult        [out]    返回加密数据结果
 * @param  OutLength      [out]    返回加密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM1Encrypt256(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中SM1-256算法中的解密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     密文数据长度 长度必须为16的倍数
 * @param  pText          [in]     要解密的密文数据内容

 * @param  pResult        [out]    返回解密数据结果
 * @param  OutLength      [out]    返回解密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM1Decrypt256(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   设置加密狗中SM4算法中的密钥
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pKey           [in]     SM4算法中的密钥 密钥长度固定为16个字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM4SetKey(WORD Index, BYTE * pKey);

/** 
 * @说明   设置加密狗中SM4算法中的密钥
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pIV            [in]     SM4算法中的IV 长度固定为16个字节
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM4SetIV(WORD Index, BYTE * pIV);

/** 
 * @说明   执行加密狗中SM4算法中的加密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  length         [in]     数据长度 长度必须为16的倍数
 * @param  pText          [in]     要加密的数据内容

 * @param  pResult        [out]    返回加密数据结果
 * @param  OutLength      [out]    返回加密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM4Encrypt(WORD Index, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中SM4算法中的解密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  length         [in]     密文数据长度 长度必须为16的倍数
 * @param  pText          [in]     要解密的密文数据内容

 * @param  pResult        [out]    返回解密数据结果
 * @param  OutLength      [out]    返回解密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM4Decrypt(WORD Index, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中SM4算法中的加密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     数据长度 长度必须为16的倍数
 * @param  pText          [in]     要加密的数据内容

 * @param  pResult        [out]    返回加密数据结果
 * @param  OutLength      [out]    返回加密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM4EncryptEx(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行加密狗中SM4算法中的解密操作
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  dwMode         [in]     运算模式 0: ECB 或 1: CBC 或 2: OFB 或 3: CTR
 * @param  dwPaddingType  [in]     填充模式 0: None 或 1: PKCS7 或 2: Zeros 或 3: ANSIX923 或 4: ISO10126
 * @param  length         [in]     密文数据长度 长度必须为16的倍数
 * @param  pText          [in]     要解密的密文数据内容

 * @param  pResult        [out]    返回解密数据结果
 * @param  OutLength      [out]    返回解密数据长度
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM4DecryptEx(WORD Index, DWORD dwMode, DWORD dwPaddingType, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

/** 
 * @说明   执行非对称加密SM2算法的创建密钥对
 * @param  Index          [in]     指定要操作加密狗的序号

 * @param  pPrivateKey    [out]    返回私钥
 * @param  pPublicKey     [out]    返回公钥
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM2CreateKey(WORD Index, BYTE * pPrivateKey, BYTE* pPublicKey);

/** 
 * @说明   SM2算法通过私钥计算公钥
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pPrivateKey    [in]     输入私钥 长度为32字节

 * @param  pPublicKey     [out]    返回公钥
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM2CalcPubKey(WORD Index, BYTE * pPrivateKey, BYTE* pPublicKey);

/** 
 * @说明   将SM2私钥写入到加密狗中
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pPrivateKey    [in]     输入私钥 长度为32字节

 * @param  pPublicKey     [out]    返回公钥
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySetSM2PrivateKey(WORD Index, BYTE* pPrivateKey);

/** 
 * @说明   将SM2公钥写入到加密狗中
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pPublicKey     [in]     输入公钥 长度为64字节 

 * @param  pPublicKey     [out]    返回公钥
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySetSM2PublicKey(WORD Index, BYTE* pPublicKey);

/** 
 * @说明   SM2算法通过私钥签名数据
 * @param  Index			[in]     指定要操作加密狗的序号
 * @param  pUserID			[in]     输入用户ID
 * @param  wUserIDLength    [in]     输入用户ID长度
 * @param  pData			[in]     输入要签名的数据
 * @param  wDataLength		[in]     输入要签名的数据长度

 * @param  pSignR			[out]    返回签名R
 * @param  pSignS			[out]    返回签名S
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySM2Sign(WORD Index, BYTE* pUserID, WORD wUserIDLength, BYTE* pData, WORD wDataLength, BYTE* pSignR, BYTE* pSignS);

/** 
 * @说明   SM2算法通过公钥验证签名
 * @param  Index			[in]     指定要操作加密狗的序号
 * @param  pUserID			[in]     输入用户ID
 * @param  wUserIDLength    [in]     输入用户ID长度
 * @param  pData			[in]     输入要验签的数据
 * @param  wDataLength		[in]     输入要验签的数据长度
 * @param  pSignR			[in]     输入签名R
 * @param  pSignS			[in]     输入签名S

 * @return 0     表示操作验证签名成功
 */
DWORD __stdcall VikeySM2Verify(WORD Index, BYTE* pUserID, WORD wUserIDLength, BYTE* pData, WORD wDataLength, BYTE* pSignR, BYTE* pSignS);

/** 
 * @说明   SM2算法通过公钥进行数据加密
 * @param  Index			[in]     指定要操作加密狗的序号
 * @param  pData			[in]     输入加密的数据
 * @param  wDataLength		[in]     输入加密的数据长度

 * @param  pResult			[out]    输出加密后的结果密文
 * @param  wResultLength	[out]    输出加密后的结果密文长度

 * @return 0     表示操作加密成功
 */
DWORD __stdcall VikeySM2Encrypt(WORD Index, BYTE* pData, WORD wDataLength, BYTE* pResult, WORD *wResultLength);

/** 
 * @说明   SM2算法通过私钥进行数据解密
 * @param  Index			[in]     指定要操作加密狗的序号
 * @param  pData			[in]     输入解密的数据
 * @param  wDataLength		[in]     输入解密的数据长度

 * @param  pResult			[out]    输出解密后的结果明文
 * @param  wResultLength	[out]    输出解密后的结果明文长度

 * @return 0     表示操作解密成功
 */
DWORD __stdcall VikeySM2Decrypt(WORD Index, BYTE* pData, WORD wDataLength, BYTE* pResult, WORD *wResultLength);

/** 
 * @说明   执行非对称加密ECC算法的创建密钥对
 * @param  Index          [in]     指定要操作加密狗的序号

 * @param  pPrivateKey    [out]    返回私钥
 * @param  pPublicKey     [out]    返回公钥
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyEccCreateKey(WORD Index, BYTE * pPrivateKey, BYTE* pPublicKey);

/** 
 * @说明   ECC算法通过私钥计算公钥
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pPrivateKey    [in]     输入私钥 长度为32字节

 * @param  pPublicKey     [out]    返回公钥
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyEccCalcPubKey(WORD Index, BYTE * pPrivateKey, BYTE* pPublicKey);

/** 
 * @说明   ECDH算法 密钥协商 通过加密狗内部的Ecc私钥和输入的公钥生成共享密钥
 * @param  Index			[in]     指定要操作加密狗的序号
 * @param  pOtherPublicKey  [in]     输入的公钥 长度为64字节

 * @param  pShareKey        [out]    返回共享密钥
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyEccECDH(WORD Index, BYTE * pOtherPublicKey, BYTE * pShareKey);

/** 
 * @说明   将ECC私钥写入到加密狗中
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pPrivateKey    [in]     输入私钥 长度为32字节

 * @param  pPublicKey     [out]    返回公钥
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyEccSetPrivateKey(WORD Index, BYTE* pPrivateKey);

/** 
 * @说明   将Ecc公钥写入到加密狗中
 * @param  Index          [in]     指定要操作加密狗的序号
 * @param  pPublicKey     [in]     输入公钥 长度为64字节 

 * @param  pPublicKey     [out]    返回公钥
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyEccSetPublicKey(WORD Index, BYTE* pPublicKey);

/** 
 * @说明   ECDSA算法通过私钥签名数据
 * @param  Index			[in]     指定要操作加密狗的序号
 * @param  pData			[in]     输入要签名的数据 长度为32字节

 * @param  pSignR			[out]    返回签名R
 * @param  pSignS			[out]    返回签名S
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyEccSign(WORD Index, BYTE* pDigest, BYTE* pSignR, BYTE* pSignS);

/** 
 * @说明   ECDSA算法通过公钥验证签名
 * @param  Index			[in]     指定要操作加密狗的序号
 * @param  pData			[in]     输入要验签的数据 长度为32字节
 * @param  pSignR			[in]     输入签名R
 * @param  pSignS			[in]     输入签名S

 * @return 0     表示操作验证签名成功
 */
DWORD __stdcall VikeyEccVerify(WORD Index, BYTE* pDigest, BYTE* pSignR, BYTE* pSignS);

/** 
 * @说明   ECIES算法通过公钥进行数据加密
 * @param  Index			[in]     指定要操作加密狗的序号
 * @param  pData			[in]     输入加密的数据
 * @param  wDataLength		[in]     输入加密的数据长度

 * @param  pResult			[out]    输出加密后的结果密文
 * @param  wResultLength	[out]    输出加密后的结果密文长度

 * @return 0     表示操作加密成功
 */
DWORD __stdcall VikeyEccEncrypt(WORD Index, BYTE* pData, WORD wDataLength, BYTE* pResult, WORD *wResultLength);

/** 
 * @说明   ECIES算法通过私钥进行数据解密
 * @param  Index			[in]     指定要操作加密狗的序号
 * @param  pData			[in]     输入解密的数据
 * @param  wDataLength		[in]     输入解密的数据长度

 * @param  pResult			[out]    输出解密后的结果明文
 * @param  wResultLength	[out]    输出解密后的结果明文长度

 * @return 0     表示操作解密成功
 */
DWORD __stdcall VikeyEccDecrypt(WORD Index, BYTE* pData, WORD wDataLength, BYTE* pResult, WORD *wResultLength);

//For ViKeyTIME
typedef struct _VIKEY_TIME 
{ 
	BYTE cYear;	      //年	
	BYTE cMonth;      //月
	BYTE cDay;        //日
	BYTE cHour;       //时
	BYTE cMinute;     //分
	BYTE cSecond;     //秒
#ifdef _MSC_VER
	bool operator < (const _VIKEY_TIME &another) const;
	bool operator > (const _VIKEY_TIME &another) const;
#endif
} SVikeyTime, *PVIKEYTIME;

/** 
 * @说明   获取时钟型加密狗中的内部独立时间
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  pTime         [out]    返回时钟型加密狗中的当前时间 6个字节 XX年XX月XX日XX时XX分XX秒
 * @return 0     表示操作成功
 */

DWORD __stdcall VikeyGetTime(WORD Index, PVIKEYTIME pTime);

/** 
 * @说明   获取时钟型加密狗中的到期时间
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  pTime         [out]    返回时钟型加密狗中的到期时间 6个字节 XX年XX月XX日XX时XX分XX秒
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyGetValidTime(WORD Index, PVIKEYTIME pTime);

/** 
 * @说明   设置时钟型加密狗中的到期时间
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  pTime         [in]    返回时钟型加密狗中的到期时间 6个字节 XX年XX月XX日XX时XX分XX秒
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeySetValidTime(WORD Index, PVIKEYTIME pTime);

/** 
 * @说明   检测时钟型加密狗的到期时间是否到期
 * @param  Index         [in]     指定要操作加密狗的序号
 * @param  pTime         [out]    返回是否到期结果 1表示没有到期   0表示已经到期
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyCheckValidTime(WORD Index, BYTE * pIsValid);

/** 
 * @说明   生成更新动态码 使用管理工具通过动态码生成对应的续期码和授权码来更新到期时间、更新限制次数、更新绑定电脑、更新内存数据
 * @param  Index				[in]     指定要操作加密狗的序号
 * @param  pDynamicCodeString	[out]    返回动态码 长度为64个Assic字符
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyGetDynamicCode(WORD Index, char * pDynamicCodeString);

/** 
 * @说明   刷新动态码 使用管理工具通过动态码生成对应的续期码和授权码来更新到期时间、更新限制次数、更新绑定电脑、更新内存数据
 * @param  Index				[in]     指定要操作加密狗的序号
 * @param  pDynamicCodeString	[out]    返回动态码 长度为64个Assic字符
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyRefreshDynamicCode(WORD Index, char * pDynamicCodeString);

/** 
 * @说明   通过续期码更新时钟型加密狗的到期时间
           续期码的生成:ViKey加密管理工具->加密狗管理->远程更新->更新到期时间->生成续期码
 * @param  Index				[in]     指定要操作加密狗的序号
 * @param  pUpdateCodeString	[in]     续期码 长度为64个Assic字符
 * @param  pNewValidTime		[out]    返回续期成功后的到期时间
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyUpdateValidTime(WORD Index, char * pUpdateCodeString, PVIKEYTIME pNewValidTime);

/** 
 * @说明   通过授权码更新加密软件的次数限制
           授权码的生成:ViKey加密管理工具->加密狗管理->远程更新->更新次数限制->生成授权码
 * @param  Index				[in]     指定要操作加密狗的序号
 * @param  pUpdateCodeString	[in]     授权码 长度为64个Assic字符
 * @param  pwLimitCount			[out]    返回更新后的限制次数
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyUpdateLimitCount(WORD Index, char * pUpdateCodeString, WORD * pwLimitCount);

/** 
 * @说明   通过授权码更新加密狗的数据区
           授权码的生成:ViKey加密管理工具->加密狗管理->远程更新->更新内存数据->生成授权码
 * @param  Index				[in]     指定要操作加密狗的序号
 * @param  pUpdateCodeString	[in]     授权码 长度为64个Assic字符
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyUpdateMemoryData(WORD Index, char * pUpdateCodeString);

/** 
 * @说明   通过授权码清除加密狗中的电脑绑定信息
           授权码的生成:ViKey加密管理工具->加密狗管理->远程更新->更新绑定电脑->生成授权码
 * @param  Index				[in]     指定要操作加密狗的序号
 * @param  pUpdateCodeString	[in]     授权码 长度为64个Assic字符
 * @return 0     表示操作成功
 */
DWORD __stdcall VikeyUpdateClearBinding(WORD Index, char * pUpdateCodeString);

#ifdef __cplusplus
} //  extern "C"{
#endif

#endif
