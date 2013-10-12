/***************************************************************************
 *   Copyright (C) 2002~2005 by Yuking                                     *
 *   yuking_net@sohu.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _TABLE_H
#define _TABLE_H
//#ifdef __cplusplus
//extern "C"{
//#endif
//#include <limits.h>
//#include "ime.h"
#include <unistd.h>
#include <stdio.h>
//#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#define TABLE_CONFIG_FILENAME "tables.conf"

#define MAX_CODE_LENGTH  30
#define PHRASE_MAX_LENGTH 10
#define FH_MAX_LENGTH  10
#define TABLE_AUTO_SAVE_AFTER 1024
#define AUTO_PHRASE_COUNT 10000
#define SINGLE_HZ_COUNT 33000

#define MAX_CAND_WORD	10
#define MAX_USER_INPUT	300

#define HOT_KEY_COUNT	2

#define MAX_IM_NAME	15
#define PATH_MAX 50
#define TEMP_FILE		"FCITX_DICT_TEMP"
#define True 1
#define False 0

typedef int Bool;
typedef char UINT8;
typedef char INT8;
typedef unsigned short int UINT16;
typedef short int INT16;
typedef unsigned int uint;

typedef enum _SEARCH_MODE {
    SM_FIRST,
    SM_NEXT,
    SM_PREV
} SEARCH_MODE;

typedef enum ADJUST_ORDER {
    AD_NO,
    AD_FAST,
    AD_FREQ
} ADJUSTORDER;

typedef enum _INPUT_RETURN_VALUE {
    //IRV_UNKNOWN = -1,
    IRV_DO_NOTHING = 0,
    IRV_DONOT_PROCESS,
    IRV_DONOT_PROCESS_CLEAN,
    IRV_CLEAN,
    IRV_TO_PROCESS,
    IRV_DISPLAY_MESSAGE,
    IRV_DISPLAY_CANDWORDS,
    IRV_DISPLAY_LAST,
    IRV_PUNC,
    IRV_ENG,
    IRV_GET_LEGEND,
    IRV_GET_CANDWORDS,
    IRV_GET_CANDWORDS_NEXT
} INPUT_RETURN_VALUE;

typedef enum _ENTER_TO_DO {
    K_ENTER_NOTHING,
    K_ENTER_CLEAN,
    K_ENTER_SEND
} ENTER_TO_DO;

typedef enum _SEMICOLON_TO_DO {
    K_SEMICOLON_NOCHANGE,
    K_SEMICOLON_ENG,
    K_SEMICOLON_QUICKPHRASE
} SEMICOLON_TO_DO;

typedef struct _SINGLE_HZ {
    char            strHZ[3];
} SINGLE_HZ;

typedef enum _KEY_RELEASED {
    KR_OTHER = 0,
    KR_CTRL,
    KR_2ND_SELECTKEY,
    KR_2ND_SELECTKEY_OTHER,
    KR_3RD_SELECTKEY,
    KR_3RD_SELECTKEY_OTHER
} KEY_RELEASED;

typedef struct {
    char            strName[MAX_IM_NAME + 1];
    void            (*ResetIM) (void);
                    INPUT_RETURN_VALUE (*DoInput) (int);
                    INPUT_RETURN_VALUE (*GetCandWords) (SEARCH_MODE);
    char           *(*GetCandWord) (int);
    char           *(*GetLegendCandWord) (int);
                    Bool (*PhraseTips) (void);
    void            (*Init) (void);
    void            (*Destroy) (void);
} IM;

typedef struct _RULE_RULE {
    unsigned char   iFlag;	// 1 --> 正序   0 --> 逆序
    unsigned char   iWhich;	//第几个字
    unsigned char   iIndex;	//第几个编码
} RULE_RULE;

typedef struct _RULE {
    unsigned char   iWords;	//多少个字
    unsigned char   iFlag;	//1 --> 大于等于iWords  0 --> 等于iWords
    RULE_RULE      *rule;
} RULE;

typedef struct _TABLE {
    char            strPath[PATH_MAX];
    char            strSymbolFile[PATH_MAX];
    char            strName[MAX_IM_NAME + 1];
    char           *strInputCode;
    unsigned char   iCodeLength;
    unsigned char   iPYCodeLength;
    char           *strEndCode;
    char           *strIgnoreChars;
    char            cMatchingKey;
    char            strSymbol[MAX_CODE_LENGTH + 1];
    char            cPinyin;
    unsigned char   bRule;

    RULE           *rule;
    INT8            iIMIndex;
    unsigned int    iRecordCount;
    ADJUSTORDER     tableOrder;

    Bool            bUsePY;
    INT8            iTableAutoSendToClient;
    INT8            iTableAutoSendToClientWhenNone;
    Bool            bUseMatchingKey;
    Bool            bAutoPhrase;
    INT8            iSaveAutoPhraseAfter;
    Bool            bAutoPhrasePhrase;
    INT8            iAutoPhrase;
    Bool            bTableExactMatch;
    Bool            bHasPinyin;
    char            choose[11];
} TABLE;

typedef struct _RECORD {
    char           *strCode;
    char           *strHZ;
    struct _RECORD *next;
    struct _RECORD *prev;
    unsigned int    iHit;
    unsigned int    iIndex;
    unsigned int    flag:1;
    unsigned int    bPinyin:1;
} RECORD;

typedef struct _RECORD_INDEX {
    RECORD         *record;
    char            cCode;
} RECORD_INDEX;

typedef struct _FH {
    char            strFH[FH_MAX_LENGTH * 2 + 1];
} FH;

typedef struct _AUTOPHRASE {
    char           *strHZ;
    char           *strCode;
    INT8            iSelected;
    unsigned int    flag:1;
    struct _AUTOPHRASE *next;
} AUTOPHRASE;

typedef union {
    AUTOPHRASE     *autoPhrase;
    RECORD         *record;
    char            strPYPhrase[PHRASE_MAX_LENGTH * 2 + 1];
} CANDWORD;

typedef struct _TABLECANDWORD {
    unsigned int    flag:2;
    CANDWORD        candWord;
} TABLECANDWORD;

typedef enum {
    CT_NORMAL = 0,
    CT_AUTOPHRASE,
    CT_PYPHRASE
} CANDTYPE;

//#define mTEST
//#define WBIME 1

TABLE          *table;

INT8            iTableIMIndex = 0;
INT8            iTableCount;

Bool            bTableDictLoaded = False;		//Loads tables only if needed

RECORD         *currentRecord = (RECORD *)NULL;
RECORD	       *recordHead = (RECORD *)NULL;
RECORD         *tableSingleHZ[SINGLE_HZ_COUNT];		//Records the single characters in table to speed auto phrase
RECORD	       *pCurCandRecord = (RECORD *)NULL;	//Records current cand word selected, to update the hit-frequency information
TABLECANDWORD   tableCandWord[MAX_CAND_WORD];


RECORD_INDEX   *recordIndex = (RECORD_INDEX *)NULL;

AUTOPHRASE     *autoPhrase = (AUTOPHRASE *)NULL;
AUTOPHRASE     *insertPoint = (AUTOPHRASE *)NULL;

uint            iAutoPhrase = 0;
uint            iTableCandDisplayed;
uint            iTableTotalCandCount;
INT16           iTableOrderChanged = 0;
char            strTableLegendSource[PHRASE_MAX_LENGTH * 2 + 1] = "\0";

FH             *fh;
int             iFH = 0;
unsigned int    iTableIndex = 0;

Bool            bIsTableDelPhrase = False;
Bool            bIsTableAdjustOrder = False;
Bool            bIsTableAddPhrase = False;

INT8            iTableChanged = 0;
INT8            iTableNewPhraseHZCount;
Bool            bCanntFindCode; /*Records if new phrase has corresponding code - should be always false */
char           *strNewPhraseCode;

SINGLE_HZ       hzLastInput[PHRASE_MAX_LENGTH]; /* Records last HZ input */
INT16           iHZLastInputCount = 0;
Bool            bTablePhraseTips = False;

ADJUSTORDER     PYBaseOrder;

#define mTEST
#ifdef WBIME
extern char     strPYAuto[1];
INT8     iInternalVersion = 0x03;

extern char     strCodeInput[];
extern Bool     bIsDoInputOnly;
extern int      iCandPageCount;
extern int      iCurrentCandPage;
extern int      iCandWordCount;
extern int      iLegendCandWordCount;
extern int      iLegendCandPageCount;
extern int      iCurrentLegendCandPage;
extern int      iCodeInputCount;
extern int      iMaxCandWord;
extern char     strStringGet[5];

extern Bool     bPointAfterNumber;

extern INT8     iIMIndex;
extern Bool     bUseLegend;
extern Bool     bIsInLegend;
extern INT8     lastIsSingleHZ;
extern Bool     bDisablePagingInLegend;
extern Bool     bShowCursor;

extern ADJUSTORDER baseOrder;
extern Bool     bSP;
#endif
#ifdef mTEST
char     strPYAuto[1];
INT8     iInternalVersion = 0x03;
char     strCodeInput[5];
Bool     bIsDoInputOnly;
int      iCandPageCount;
int      iCurrentCandPage;
int      iCandWordCount;
int      iLegendCandWordCount;
int      iLegendCandPageCount;
int      iCurrentLegendCandPage;
int      iCodeInputCount;
int      iMaxCandWord = 5;
char     strStringGet[5];

Bool     bPointAfterNumber;

INT8     iIMIndex;
Bool     bUseLegend;
Bool     bIsInLegend;
INT8     lastIsSingleHZ;
Bool     bDisablePagingInLegend;
Bool     bShowCursor;

ADJUSTORDER baseOrder;
Bool     bSP;
#endif

void            LoadTableInfo (void);
Bool            LoadTableDict (void);
void            TableInit (void);
void            FreeTableIM (void);
void            SaveTableDict (void);
Bool            IsInputKey (int iKey);
Bool            IsIgnoreChar (char cChar);
Bool            IsEndKey (char cChar);
INPUT_RETURN_VALUE DoTableInput (int iKey);
INPUT_RETURN_VALUE TableGetCandWords (SEARCH_MODE mode);
void            TableAddCandWord (RECORD * wbRecord, SEARCH_MODE mode);
void            TableAddAutoCandWord (INT16 which, SEARCH_MODE mode);
INPUT_RETURN_VALUE TableGetLegendCandWords (SEARCH_MODE mode);
void            TableAddLegendCandWord (RECORD * record, SEARCH_MODE mode);
INPUT_RETURN_VALUE TableGetFHCandWords (SEARCH_MODE mode);
INPUT_RETURN_VALUE TableGetPinyinCandWords (SEARCH_MODE mode);
void            TableResetStatus (void);
char           *TableGetLegendCandWord (int iIndex);
char           *TableGetFHCandWord (int iIndex);
Bool            HasMatchingKey (void);
int             TableCompareCode (char *strUser, char *strDict);
int             TableFindFirstMatchCode (void);
void            TableAdjustOrderByIndex (int iIndex);
void            TableDelPhraseByIndex (int iIndex);
void            TableDelPhraseByHZ (char *strHZ);
void            TableDelPhrase (RECORD * record);
RECORD         *TableHasPhrase (char *strCode, char *strHZ);
RECORD         *TableFindPhrase (char *strHZ);
void            TableInsertPhrase (char *strCode, char *strHZ);
char	       *_TableGetCandWord (int iIndex, Bool _bLegend);
char           *TableGetCandWord (int iIndex);
void		TableUpdateHitFrequency (RECORD * record);
void            TableCreateNewPhrase (void);
void            TableCreatePhraseCode (char *strHZ);
Bool            TablePhraseTips (void);
void            TableSetCandWordsFlag (int iCount, Bool flag);
void            TableResetFlags (void);

void            TableCreateAutoPhrase (INT8 iCount);

void            UpdateHZLastInput (char *);

/**********************************************************************
 *
 *
 **********************************************************************/

Bool MyStrcmp (char *str1, char *str2)
{
    //return !strncmp (str1, str2, strlen (str2));
    if(strstr (str1, str2))
        return 1;
    return 0;
}
Bool CheckHZCharset (char *strHZ)
{
    //if (!bUseGBK)
    if(1==0)
    {
        /*
         *GB2312的汉字编码规则为：第一个字节的值在0xA1到0xFE之间(实际为0xF7)，第二个字节的值在0xA1到0xFE之间
         */
        unsigned int i;

        for (i = 0; i < strlen (strHZ); i++) {
            if ((unsigned char) strHZ[i] < (unsigned char) 0xA1 || (unsigned char) strHZ[i] > (unsigned char) 0xF7 || (unsigned char) strHZ[i + 1] < (unsigned char) 0xA1 || (unsigned char) strHZ[i + 1] > (unsigned char) 0xFE)
            return False;
            i++;
        }
    }

    return True;
}
int CalHZIndex (char *strHZ)
{
    return (strHZ[0] + 127) * 255 + strHZ[1] + 128;
}
/*
 * 读取码表输入法的名称和文件路径
 */
void LoadTableInfo (void)
{
    FILE           *fp;
    char            strPath[PATH_MAX];
    int             i;
    char           *pstr;

    FreeTableIM ();

    if (table)
        free (table);
    iTableCount = 0;


    //fp = fopen ("./wbx.conf", "rt");
    fp = fopen ("/home/user/.config/wubi/wbx.conf","rt");
    /*
    snprintf (strPath, sizeof(strPath), "%s/.fcitx/%s",
              getenv ("HOME"),
              TABLE_CONFIG_FILENAME);

    if (access (strPath, 0))
        snprintf (strPath, sizeof(strPath), PKGDATADIR "/data/%s", TABLE_CONFIG_FILENAME);

    fp = fopen (strPath, "rt");
    */

    if (!fp)
    {
        fprintf(stderr, "file load err:%s\n", "wbx.conf");
        exit(-1);
    }
    //首先来看看有多少个码表
    while (1) {
        if (!fgets (strPath, PATH_MAX, fp))
            break;

        i = strlen (strPath) - 1;
        while ((i >= 0) && (strPath[i] == ' ' || strPath[i] == '\n'))
            strPath[i--] = '\0';

        pstr = strPath;
        if (*pstr == ' ')
            pstr++;
        if (pstr[0] == '#')
            continue;

        if (strstr (pstr, "CodeTable"))
        {
            iTableCount++;
        }
    }

    table = (TABLE *) malloc (sizeof (TABLE) * iTableCount);

        table->strInputCode = NULL;
        table->strEndCode = NULL;
        table->strName[0] = '\0';
        table->strPath[0] = '\0';
        table->strSymbolFile[0] = '\0';
        table->tableOrder = AD_NO;
        table->bUsePY = False;
        table->cPinyin = '\0';
        table->iTableAutoSendToClient = -1;
        table->iTableAutoSendToClientWhenNone = -1;
        table->rule = NULL;
        table->bUseMatchingKey = False;
        table->cMatchingKey = '\0';
        table->bTableExactMatch = False;
        table->bAutoPhrase = False;
        table->bAutoPhrasePhrase = True;
        table->iSaveAutoPhraseAfter = 0;
        table->iAutoPhrase = 4;
        //table[0].bPromptTableCode = True;
        table->strSymbol[0] = '\0';
        table->bHasPinyin = False;
        strcpy (table->choose, "1234567890");//初始化为1234567890


    //读取其它属性的默认值
    if (iTableCount)
    {
        rewind (fp);

        while (1)
        {
            if (!fgets (strPath, PATH_MAX, fp))
                break;

            i = strlen (strPath) - 1;
            while ((i >= 0) && (strPath[i] == ' ' || strPath[i] == '\n'))
                strPath[i--] = '\0';

            pstr = strPath;
            if (*pstr == ' ')
                pstr++;
            if (pstr[0] == '#')
                continue;


            else if (MyStrcmp (pstr, "Name=")) {
                pstr += 5;
                strcpy (table->strName, pstr);
            }
            else if (MyStrcmp (pstr, "File=")) {
                pstr += 5;
                strcpy (table->strPath, pstr);
            }
            else if (MyStrcmp (pstr, "AdjustOrder=")) {
                pstr += 12;
                table->tableOrder = (ADJUSTORDER) atoi (pstr);
            }
            else if (MyStrcmp (pstr, "UsePY=")) {
                pstr += 6;
                table->bUsePY = atoi (pstr);
            }
            else if (MyStrcmp (pstr, "PYKey=")) {
                pstr += 6;
                while (*pstr == ' ')
                    pstr++;
                table->cPinyin = *pstr;
            }
            else if (MyStrcmp (pstr, "AutoSend=")) {
                pstr += 9;
                table->iTableAutoSendToClient = atoi (pstr);
            }
            else if (MyStrcmp (pstr, "NoneMatchAutoSend=")) {
                pstr += 18;
                table->iTableAutoSendToClientWhenNone = atoi (pstr);
            }
            else if (MyStrcmp (pstr, "EndKey=")) {
                pstr += 7;
                while (*pstr == ' ' || *pstr == '\t')
                    pstr++;
                table->strEndCode = (char *) malloc (sizeof (char) * (strlen (pstr) + 1));
                strcpy (table->strEndCode, pstr);
            }
            else if (MyStrcmp (pstr, "UseMatchingKey=")) {
                pstr += 15;
                table->bUseMatchingKey = atoi (pstr);

            }
            else if (MyStrcmp (pstr, "MatchingKey=")) {
                pstr += 12;
                while (*pstr == ' ')
                    pstr++;
                    table->cMatchingKey = *pstr;
            }
            else if (MyStrcmp (pstr, "ExactMatch=")) {
                pstr += 11;
                table->bTableExactMatch = atoi (pstr);
            }
            else if (MyStrcmp (pstr, "AutoPhrase=")) {
                pstr += 11;
                table->bAutoPhrase = atoi (pstr);
            }
            else if (MyStrcmp (pstr, "AutoPhraseLength=")) {
                pstr += 17;
                table->iAutoPhrase = atoi (pstr);
            }
            else if (MyStrcmp (pstr, "AutoPhrasePhrase=")) {
                pstr += 17;
                table->bAutoPhrasePhrase = atoi (pstr);
            }
            else if (MyStrcmp (pstr, "SaveAutoPhrase=")) {
                pstr += 15;
                table->iSaveAutoPhraseAfter = atoi (pstr);
            }

            else if (MyStrcmp (pstr, "Symbol=")) {
                pstr += 7;
                strcpy (table->strSymbol, pstr);
            }
            else if (MyStrcmp (pstr, "SymbolFile=")) {
                pstr += 11;
                strcpy (table->strSymbolFile, pstr);
            }
            else if(MyStrcmp (pstr, "候选词选择键=")){
                    pstr += 25;
                    strncpy (table->choose, pstr, 10);
            }
        }
    }

    fclose (fp);
}

Bool LoadTableDict (void)
{
    char            strCode[MAX_CODE_LENGTH + 1];
    char            strHZ[PHRASE_MAX_LENGTH * 2 + 1];
    FILE           *fpDict;
    RECORD         *recTemp;
    char            strPath[PATH_MAX];
    unsigned int    i = 0;
    unsigned int    iTemp, iTempCount;
    char            cChar = 0, cTemp;
    INT8            iVersion = 1;
    int             iRecordIndex;

    //首先，来看看我们该调入哪个码表
    for (i = 0; i < iTableCount; i++)
    {
        if (table[i].iIMIndex == iIMIndex)
            iTableIMIndex = i;
    }

    //读入码表
#ifdef _DEBUG
    fprintf (stderr, "Loading Table Dict\n");
#endif
/*
    snprintf (strPath, sizeof(strPath), "%s/.fcitx/%s",
              getenv ("HOME"),
              table[0].strPath);;

    if (access (strPath, 0))
    snprintf (strPath, sizeof(strPath), PKGDATADIR "/data/%s", table[0].strPath);

    fpDict = fopen (strPath, "rb");
*/
    //fpDict = fopen ("./wbx.mb","rb");
    fpDict = fopen ("/home/user/.config/wubi/wbx.mb", "rb");
    if (!fpDict)
    {
        fprintf (stdout, "Cannot load table file: %s\n", strPath);
        return False;
    }

    //先读取码表的信息
    //判断版本信息
    fread (&iTemp, sizeof (unsigned int), 1, fpDict);

    if (!iTemp)
    {
        fread (&iVersion, sizeof (INT8), 1, fpDict);
        iVersion = (iVersion < iInternalVersion);
        fread (&iTemp, sizeof (unsigned int), 1, fpDict);

    }

    table->strInputCode = (char *) malloc (sizeof (char) * (iTemp + 1));
    fread (table->strInputCode, sizeof (char), iTemp + 1, fpDict);
    //printf("%d table->strInputCode: %s \n",__LINE__, table->strInputCode);
    /*
     * 建立索引，加26是为了为拼音编码预留空间
     */
    recordIndex = (RECORD_INDEX *) malloc ((strlen (table->strInputCode) + 26) * sizeof (RECORD_INDEX));
    for (iTemp = 0; iTemp < strlen (table->strInputCode) + 26; iTemp++)
    {
        recordIndex[iTemp].cCode = 0;
        recordIndex[iTemp].record = NULL;
    }
    /* ********************************************************************** */

    fread (&(table->iCodeLength), sizeof (unsigned char), 1, fpDict);
    //
    //if (table->iTableAutoSendToClient == 0)
      //  table->iTableAutoSendToClient = table->iCodeLength;
    if (table->iTableAutoSendToClientWhenNone == 0)
        table->iTableAutoSendToClientWhenNone = table->iCodeLength;

    if (!iVersion)
        fread (&(table->iPYCodeLength), sizeof (unsigned char), 1, fpDict);
    else
        table->iPYCodeLength = table->iCodeLength;

    fread (&iTemp, sizeof (unsigned int), 1, fpDict);
        table[0].strIgnoreChars = (char *) malloc (sizeof (char) * (iTemp + 1));
    fread (table[0].strIgnoreChars, sizeof (char), iTemp + 1, fpDict);

    fread (&(table[0].bRule), sizeof (unsigned char), 1, fpDict);

    if (table[0].bRule)		//表示有组词规则
    {
        table[0].rule = (RULE *) malloc (sizeof (RULE) * (table[0].iCodeLength - 1));
        for (i = 0; i < table[0].iCodeLength - 1; i++)
        {
            fread (&(table[0].rule[i].iFlag), sizeof (unsigned char), 1, fpDict);
            fread (&(table[0].rule[i].iWords), sizeof (unsigned char), 1, fpDict);
            table[0].rule[i].rule = (RULE_RULE *) malloc (sizeof (RULE_RULE) * table[0].iCodeLength);
            for (iTemp = 0; iTemp < table[0].iCodeLength; iTemp++)
            {
                fread (&(table[0].rule[i].rule[iTemp].iFlag), sizeof (unsigned char), 1, fpDict);
                fread (&(table[0].rule[i].rule[iTemp].iWhich), sizeof (unsigned char), 1, fpDict);
                fread (&(table[0].rule[i].rule[iTemp].iIndex), sizeof (unsigned char), 1, fpDict);
            }
        }
    }

    recordHead = (RECORD *) malloc (sizeof (RECORD));
    currentRecord = recordHead;

    fread (&(table[0].iRecordCount), sizeof (unsigned int), 1, fpDict);
    //printf("%d %d %d\n",iTemp,table[0].bRule, table[0].iRecordCount);
    for (i = 0; i < SINGLE_HZ_COUNT; i++)
        tableSingleHZ[i] = (RECORD *) NULL;

    iRecordIndex = 0;
    for (i = 0; i < table[0].iRecordCount; i++)
    {
        fread (strCode, sizeof (char), table[0].iPYCodeLength + 1, fpDict);
        fread (&iTemp, sizeof (unsigned int), 1, fpDict);
        fread (strHZ, sizeof (char), iTemp, fpDict);

        recTemp = (RECORD *) malloc (sizeof (RECORD));
        recTemp->strCode = (char *) malloc (sizeof (char) * (strlen (strCode) + 1));
        strcpy (recTemp->strCode, strCode);
        recTemp->strHZ = (char *) malloc (sizeof (char) * iTemp);
        strcpy (recTemp->strHZ, strHZ);

        if (!iVersion) {
            fread (&cTemp, sizeof (char), 1, fpDict);
            recTemp->bPinyin = cTemp;
        }

        recTemp->flag = 0;

        fread (&(recTemp->iHit), sizeof (unsigned int), 1, fpDict);
        fread (&(recTemp->iIndex), sizeof (unsigned int), 1, fpDict);
        if (recTemp->iIndex > iTableIndex)
            iTableIndex = recTemp->iIndex;

        /* 建立索引 */
        if (cChar != recTemp->strCode[0])
        {
            cChar = recTemp->strCode[0];
            recordIndex[iRecordIndex].cCode = cChar;
            recordIndex[iRecordIndex].record = recTemp;
            iRecordIndex++;
        }
        /* **************************************************************** */
        /** 为单字生成一个表   */
        if (strlen (recTemp->strHZ) == 2 && !IsIgnoreChar (strCode[0]) && !recTemp->bPinyin)
        {
            iTemp = CalHZIndex (recTemp->strHZ);
            if (iTemp >= 0 && iTemp < SINGLE_HZ_COUNT)
            {
                if (tableSingleHZ[iTemp])
                {
                    if (strlen (strCode) > strlen (tableSingleHZ[iTemp]->strCode))
                    tableSingleHZ[iTemp] = recTemp;
                }
                else
                    tableSingleHZ[iTemp] = recTemp;
            }
        }

        if (recTemp->bPinyin)
            table[0].bHasPinyin = True;

        currentRecord->next = recTemp;
        recTemp->prev = currentRecord;
        currentRecord = recTemp;
    }

    currentRecord->next = recordHead;
    recordHead->prev = currentRecord;

    fclose (fpDict);
#ifdef _DEBUG
    fprintf (stderr, "Load Table Dict OK\n");
#endif

    //读取相应的特殊符号表
    /*
    snprintf (strPath, sizeof(strPath), "%s/.fcitx/%s",
              getenv ("HOME"),
              table[0].strSymbolFile);

    if (access (strPath, 0)) {
    snprintf (strPath, sizeof(strPath), PKGDATADIR "/data/%s",
                  table[0].strSymbolFile);
        fpDict = fopen (strPath, "rt");
    }

    fpDict = fopen (strPath, "rt");
    if (fpDict) {
    iFH = CalculateRecordNumber (fpDict);
    fh = (FH *) malloc (sizeof (FH) * iFH);

    for (i = 0; i < iFH; i++) {
        if (EOF == fscanf (fpDict, "%s\n", fh[i].strFH))
        break;
    }
    iFH = i;

    fclose (fpDict);
    }
    */
    strNewPhraseCode = (char *) malloc (sizeof (char) * (table[0].iCodeLength + 1));
    strNewPhraseCode[table[0].iCodeLength] = '\0';
    bTableDictLoaded = True;

    iAutoPhrase = 0;

    if (table[0].bAutoPhrase)
    {
        //为自动词组分配空间
        autoPhrase = (AUTOPHRASE *) malloc (sizeof (AUTOPHRASE) * AUTO_PHRASE_COUNT);

        //读取上次保存的自动词组信息
#ifdef _DEBUG
        fprintf (stderr, "Loading Autophrase...\n");
#endif
#ifdef MWBIME
        strcpy (strPath, (char *) getenv ("HOME"));
        strcat (strPath, "/.config/wubi/");
        strcat (strPath, table[0].strName);
        strcat (strPath, "_LastAutoPhrase.tmp");
        fpDict = fopen (strPath, "rb");
#else
        //fpDict = fopen ("./.LastAutoPhrase.tmp", "rb");

#endif
        i = 0;
        if (fpDict) {
            fread (&iAutoPhrase, sizeof (unsigned int), 1, fpDict);

            for (; i < iAutoPhrase; i++)
            {
                autoPhrase[i].strCode = (char *) malloc (sizeof (char) * (table[0].iCodeLength + 1));
                autoPhrase[i].strHZ = (char *) malloc (sizeof (char) * (PHRASE_MAX_LENGTH * 2 + 1));
                fread (autoPhrase[i].strCode, table[0].iCodeLength + 1, 1, fpDict);
                fread (autoPhrase[i].strHZ, PHRASE_MAX_LENGTH * 2 + 1, 1, fpDict);
                fread (&iTempCount, sizeof (unsigned int), 1, fpDict);
                autoPhrase[i].iSelected = iTempCount;
                if (i == AUTO_PHRASE_COUNT - 1)
                    autoPhrase[i].next = &autoPhrase[0];
                else
                    autoPhrase[i].next = &autoPhrase[i + 1];
            }
            fclose (fpDict);
        }

        for (; i < AUTO_PHRASE_COUNT; i++)
        {
            autoPhrase[i].strCode = (char *) malloc (sizeof (char) * (table[0].iCodeLength + 1));
            autoPhrase[i].strHZ = (char *) malloc (sizeof (char) * (PHRASE_MAX_LENGTH * 2 + 1));
            autoPhrase[i].iSelected = 0;
            if (i == AUTO_PHRASE_COUNT - 1)
                autoPhrase[i].next = &autoPhrase[0];
            else
                autoPhrase[i].next = &autoPhrase[i + 1];
        }

        if (i == AUTO_PHRASE_COUNT)
            insertPoint = &autoPhrase[0];
        else
            insertPoint = &autoPhrase[i - 1];

#ifdef _DEBUG
        fprintf (stderr, "Load Autophrase OK\n");
#endif
    }
    else
        autoPhrase = (AUTOPHRASE *) NULL;
    TableInit();
    /*
     * 然后初始化码表输入法
     TableInit();
     */

    /*
     * 呵呵，借用一下这儿来为一个词库文件生成五笔编码
     */

    /*
       FILE *temp1,*temp2;
       char str[100];
       temp1=fopen("phrase.txt","rt");
       temp2=fopen("table.txt","wt");

       while (!feof(temp1)) {
       fscanf(temp1,"%s\n",str);
       TableCreatePhraseCode(str);
       printf("\nProcessing %s ...  ",str);
       if ( !bCanntFindCode )
       fprintf(temp2,"%s %s\n", strNewPhraseCode,str);
       else
       puts("ERROR!");
       }
       printf("\nCreate table.txt ok!\n");
       fclose(temp2);
       fclose(temp1);    */
    /* ********************************************** */

    return True;
}

void TableInit (void)
{
    bSP = False;
    PYBaseOrder = baseOrder;
    strPYAuto[0] = '\0';
}

/*
 * 释放当前码表所占用的内存
 * 目的是切换码表时使占用的内存减少
 */
void FreeTableIM (void)
{
    RECORD         *recTemp, *recNext;
    INT16           i;

    if (!recordHead)
        return;

    if (iTableChanged || iTableOrderChanged)
        SaveTableDict ();

    //释放码表
    recTemp = recordHead->next;
    while (recTemp != recordHead)
    {
        recNext = recTemp->next;

        free (recTemp->strCode);
        free (recTemp->strHZ);
        free (recTemp);

        recTemp = recNext;
    }

    free (recordHead);
    recordHead = NULL;

    if (iFH)
    {
        free (fh);
        iFH = 0;
    }

    free (table[0].strInputCode);
    free (table[0].strIgnoreChars);
    free (table[0].strEndCode);
    table[0].iRecordCount = 0;
    bTableDictLoaded = False;

    free (strNewPhraseCode);

    //释放组词规则的空间
    if (table[0].rule)
    {
        for (i = 0; i < table[0].iCodeLength - 1; i++)
            free (table[0].rule[i].rule);
        free (table[0].rule);

        table[0].rule = NULL;
    }

    //释放索引的空间
    if (recordIndex)
    {
        free (recordIndex);
        recordIndex = NULL;
    }

    //释放自动词组的空间
    if (autoPhrase)
    {
        for (i = 0; i < AUTO_PHRASE_COUNT; i++)
        {
            free (autoPhrase[i].strCode);
            free (autoPhrase[i].strHZ);
        }
        free (autoPhrase);

        autoPhrase = (AUTOPHRASE *) NULL;
    }

    baseOrder = PYBaseOrder;
}

void TableResetStatus (void)
{
    bIsTableAddPhrase = False;
    bIsTableDelPhrase = False;
    bIsTableAdjustOrder = False;
    bIsDoInputOnly = False;
    //bSingleHZMode = False;
}

void SaveTableDict (void)
{

}

Bool IsInputKey (int iKey)
{
    char *p;

    p = table[0].strInputCode;
    if (!p)
        return False;
printf("%d %d\n", *(char *)p, iKey);
    while (*p < 123)
    {
        if ((char)iKey == *p)
            return True;
        p++;
    }

    if (table[0].bHasPinyin)
    {
        if (iKey >= 'a' && iKey <= 'z')
            return True;
    }

    return False;
}

Bool IsEndKey (char cChar)
{
    char *p;

    p = table[0].strEndCode;
    if (!p)
        return False;

    while (*p) {
        if (cChar == *p)
            return True;
        p++;
    }

    return False;
}

Bool IsIgnoreChar (char cChar)
{
    char           *p;

    p = table[0].strIgnoreChars;
    while (*p)
    {
        if (cChar == *p)
            return True;
        p++;
    }

    return False;
}

INPUT_RETURN_VALUE DoTableInput (int iKey)
{
    INPUT_RETURN_VALUE retVal;

    if (!bTableDictLoaded)
        LoadTableDict ();
    /* a ~ z: 97 ~ 122 */
    retVal = IRV_DO_NOTHING;
    if (IsInputKey (iKey) || IsEndKey (iKey) || iKey == table[0].cMatchingKey || iKey == table[0].cPinyin)
    {

        bIsInLegend = False;

        if (!bIsTableAddPhrase && !bIsTableDelPhrase && !bIsTableAdjustOrder)
        {
            if (strCodeInput[0] == table[0].cPinyin && table[0].bUsePY)
            {
                    retVal = IRV_DO_NOTHING;
            }
            else
            {
                if ((iCodeInputCount < table[0].iCodeLength) || (table[0].bHasPinyin && iCodeInputCount < table[0].iPYCodeLength))
                {
                    strCodeInput[iCodeInputCount++] = (char) iKey;
                    strCodeInput[iCodeInputCount] = '\0';

                    if (iCodeInputCount == 1 && strCodeInput[0] == table[0].cPinyin && table[0].bUsePY)
                    {
                        iCandWordCount = 0;
                        retVal = IRV_DISPLAY_LAST;
                    }
                    else
                    {
                        char		*strTemp = NULL;
                        char		*strLastFirstCand;
                        CANDTYPE	 lastFirstCandType;
                        RECORD		*pLastCandRecord = (RECORD *)NULL;

//
                        strLastFirstCand = (char *)NULL;
                        lastFirstCandType = CT_AUTOPHRASE;
                        if ( iCandWordCount ) /*to realize auto-sending HZ to client */
                        {
                            strLastFirstCand = _TableGetCandWord (0,False);
                            lastFirstCandType = (CANDTYPE) tableCandWord[0].flag;
                            pLastCandRecord = pCurCandRecord;

                        }

                        retVal = TableGetCandWords (SM_FIRST);
                        //strTemp = GetPunc (strCodeInput[0]);
                        if (IsEndKey (iKey))
                        {
                            if (iCodeInputCount == 1)
                            return IRV_TO_PROCESS;

                            if (!iCandWordCount)
                            {
                                iCodeInputCount = 0;
                                return IRV_CLEAN;
                            }

                            if (iCandWordCount == 1)
                            {
                                strcpy (strStringGet, TableGetCandWord (0));
                                return IRV_GET_CANDWORDS;
                            }

                            return IRV_DISPLAY_CANDWORDS;
                        }
                        else if (table[0].iTableAutoSendToClientWhenNone && (iCodeInputCount == (table[0].iTableAutoSendToClientWhenNone + 1)) && !iCandWordCount)
                        {
                            if ( strLastFirstCand && (lastFirstCandType!=CT_AUTOPHRASE))
                            {
                                strcpy (strStringGet, strLastFirstCand);
                                //TableUpdateHitFrequency(pLastCandRecord);
                                retVal=IRV_GET_CANDWORDS_NEXT;
                            }
                            else
                                retVal = IRV_DISPLAY_CANDWORDS;
                            iCodeInputCount = 1;
                            strCodeInput[0] = iKey;
                            strCodeInput[1] = '\0';
                            //printf("%d, %d, %s\n",__LINE__, iCodeInputCount, strCodeInput);
                            TableGetCandWords (SM_FIRST);
                        }
                        else if (table[0].iTableAutoSendToClient && (iCodeInputCount >= table[0].iTableAutoSendToClient))
                        {
                            if (iCandWordCount == 1 && (tableCandWord[0].flag != CT_AUTOPHRASE || (tableCandWord[0].flag == CT_AUTOPHRASE && !table[0].iSaveAutoPhraseAfter)))
                            {	//如果只有一个候选词，则送到客户程序中
                                retVal = IRV_DO_NOTHING;
                                if (tableCandWord[0].flag == CT_NORMAL)
                                {
                                    if (tableCandWord[0].candWord.record->bPinyin)
                                    retVal = IRV_DISPLAY_CANDWORDS;
                                }

                                if (retVal != IRV_DISPLAY_CANDWORDS)
                                {
                                    strcpy (strStringGet, TableGetCandWord (0));
                                    iCandWordCount = 0;
                                    if (bIsInLegend)
                                        retVal = IRV_GET_LEGEND;
                                    else
                                        retVal = IRV_GET_CANDWORDS;
                                }
                            }
                        }
                        else if ((iCodeInputCount == 1) && strTemp && !iCandWordCount)
                        {	//如果第一个字母是标点，并且没有候选字/词，则当做标点处理──适用于二笔这样的输入法
                            strcpy (strStringGet, strTemp);
                            retVal = IRV_PUNC;
                        }
                    }
                }
                else
                {
                    if (table[0].iTableAutoSendToClient)
                    {
                    printf("%d\n",__LINE__);
                        if (iCandWordCount)
                        {
                            if (tableCandWord[0].flag != CT_AUTOPHRASE)
                            {
                                strcpy (strStringGet, TableGetCandWord (0));
                                retVal = IRV_GET_CANDWORDS_NEXT;
                            }
                            else
                                retVal = IRV_DISPLAY_CANDWORDS;
                        }
                        else
                            retVal = IRV_DISPLAY_CANDWORDS;

                        iCodeInputCount = 1;
                        strCodeInput[0] = iKey;
                        strCodeInput[1] = '\0';
                        bIsInLegend = False;

                        if (retVal != IRV_DISPLAY_CANDWORDS)
                            TableGetCandWords (SM_FIRST);
                        }
                        else
                            retVal = IRV_DO_NOTHING;
                }
            }
        }
    }
    else
    {
        if (bIsTableAddPhrase)
        {

            return IRV_DISPLAY_MESSAGE;
        }

        if (!iCodeInputCount && !bIsInLegend)
            return IRV_TO_PROCESS;

        else if (iKey >= '0' && iKey <= '9')
        {
            iKey -= '0';
            if (iKey == 0)
            iKey = 10;

            if (!bIsInLegend) {
            if (!iCandWordCount)
                return IRV_TO_PROCESS;

            if (iKey > iCandWordCount)
                return IRV_DO_NOTHING;
            else {
                if (bIsTableDelPhrase)
                {
                    TableDelPhraseByIndex (iKey);
                    retVal = TableGetCandWords (SM_FIRST);
                }
                else if (bIsTableAdjustOrder) {
                    TableAdjustOrderByIndex (iKey);
                    retVal = TableGetCandWords (SM_FIRST);
                }
                else {

                    strcpy (strStringGet, TableGetCandWord (iKey - 1));

                    if (bIsInLegend)
                        retVal = IRV_GET_LEGEND;
                    else
                        retVal = IRV_GET_CANDWORDS;
                }
            }
            }
            else {
                strcpy (strStringGet, TableGetLegendCandWord (iKey - 1));
                retVal = IRV_GET_LEGEND;
            }
        }
        else if (!bIsTableDelPhrase && !bIsTableAdjustOrder)
        {

            if (iKey == ' ')
            {
                if (!bIsInLegend)
                {
                    if (!(table[0].bUsePY && iCodeInputCount == 1 && strCodeInput[0] == table[0].cPinyin))
                    {
                        if (strcmp (strCodeInput, table[0].strSymbol) && strCodeInput[0] == table[0].cPinyin && table[0].bUsePY)
                        {
                            //PYGetCandWord (0);
                        }
                        if (!iCandWordCount)
                        {
                            iCodeInputCount = 0;
                            return IRV_CLEAN;
                        }

                        strcpy (strStringGet, TableGetCandWord (0));
                    }

                    if (bIsInLegend)
                        retVal = IRV_GET_LEGEND;
                    else
                        retVal = IRV_GET_CANDWORDS;

                }
                else {
                    strcpy (strStringGet, TableGetLegendCandWord (0));
                    retVal = IRV_GET_LEGEND;
                }
            }
            else
                return IRV_TO_PROCESS;
        }
    }

    if (!bIsInLegend) {
        if (!bIsTableDelPhrase && !bIsTableAdjustOrder)
        {

        }
        else
            bIsDoInputOnly = True;
    }

    if (bIsTableDelPhrase || bIsTableAdjustOrder || bIsInLegend)
        bShowCursor = False;
    else
        bShowCursor = True;

    return retVal;
}

char         *TableGetCandWord (int iIndex)
{
    char *str;

    str=_TableGetCandWord(iIndex, True);
    if (str)
    {
        if (table[0].bAutoPhrase && (strlen (str) == 2 || (strlen (str) > 2 && table[0].bAutoPhrasePhrase)))
            UpdateHZLastInput (str);

        //TableUpdateHitFrequency (pCurCandRecord);
    }

    return str;
}

void		TableUpdateHitFrequency (RECORD * record)
{

}

/*
 * 第二个参数表示是否进入联想模式，实现自动上屏功能时，不能使用联想模式
 */
char           *_TableGetCandWord (int iIndex, Bool _bLegend)
{
    char	*pCandWord = NULL;

    if (!strcmp (strCodeInput, table[0].strSymbol))
        return TableGetFHCandWord (iIndex);

    bIsInLegend = False;

    if (!iCandWordCount)
        return NULL;

    if (iIndex > (iCandWordCount - 1))
        iIndex = iCandWordCount - 1;

    if (tableCandWord[iIndex].flag == CT_NORMAL)
        pCurCandRecord = tableCandWord[iIndex].candWord.record;

    iTableOrderChanged++;
    if (iTableOrderChanged == TABLE_AUTO_SAVE_AFTER)
        SaveTableDict ();

    switch (tableCandWord[iIndex].flag)
    {
        case CT_NORMAL:
            pCandWord = tableCandWord[iIndex].candWord.record->strHZ;
            break;
        case CT_AUTOPHRASE:
            if (table[0].iSaveAutoPhraseAfter)
            {
                /* 当_bLegend为False时，不应该计算自动组词的频度，因此此时实际并没有选则这个词 */
                if (table[0].iSaveAutoPhraseAfter >= tableCandWord[iIndex].candWord.autoPhrase->iSelected && _bLegend)
                    tableCandWord[iIndex].candWord.autoPhrase->iSelected++;
                if (table[0].iSaveAutoPhraseAfter == tableCandWord[iIndex].candWord.autoPhrase->iSelected)	//保存自动词组
                    TableInsertPhrase (tableCandWord[iIndex].candWord.autoPhrase->strCode, tableCandWord[iIndex].candWord.autoPhrase->strHZ);
            }
            pCandWord = tableCandWord[iIndex].candWord.autoPhrase->strHZ;
            break;
        case CT_PYPHRASE:
            pCandWord = tableCandWord[iIndex].candWord.strPYPhrase;
            break;
        default:
        ;
    }

    if (bUseLegend && _bLegend)
    {
        strcpy (strTableLegendSource, pCandWord);
        TableGetLegendCandWords (SM_FIRST);
#warning FIX ME!
        //临时解决一个问题，似乎是出现了内存泄露:
        //pCandWord经过上面这个函数调用后就变了
        pCandWord = strTableLegendSource;
    }

    if (strlen (pCandWord) == 2)
        lastIsSingleHZ = 1;
    else
        lastIsSingleHZ = 0;

    return pCandWord;
}

INPUT_RETURN_VALUE TableGetPinyinCandWords (SEARCH_MODE mode)
{

}

INPUT_RETURN_VALUE TableGetCandWords (SEARCH_MODE mode)
{
    int             i;//, iTemp;
    char            strTemp[3], *pstr;
    RECORD         *recTemp;

    if (strCodeInput[0] == '\0')
        return IRV_TO_PROCESS;

    if (bIsInLegend)
        return TableGetLegendCandWords (mode);
    if (!strcmp (strCodeInput, table[0].strSymbol))
        return TableGetFHCandWords (mode);

    if (strCodeInput[0] == table[0].cPinyin && table[0].bUsePY)
        TableGetPinyinCandWords (mode);
    else
    {
        if (mode == SM_FIRST)
        {
            iCandWordCount = 0;
            iTableCandDisplayed = 0;
            iTableTotalCandCount = 0;
            iCurrentCandPage = 0;
            iCandPageCount = 0;
            TableResetFlags ();
            if (TableFindFirstMatchCode () == -1 && !iAutoPhrase) {

                return IRV_DISPLAY_CANDWORDS;	//Not Found
            }
        }
        else
        {

            if (!iCandWordCount)
                return IRV_TO_PROCESS;

            if (mode == SM_NEXT)
            {
                if (iTableCandDisplayed >= iTableTotalCandCount)
                    return IRV_DO_NOTHING;
            }
            else
            {
                if (iTableCandDisplayed == iCandWordCount)
                    return IRV_DO_NOTHING;

                iTableCandDisplayed -= iCandWordCount;
                TableSetCandWordsFlag (iCandWordCount, False);
            }

            TableFindFirstMatchCode ();
        }

        iCandWordCount = 0;
        if (mode == SM_PREV && table[0].bRule && table[0].bAutoPhrase && iCodeInputCount == table[0].iCodeLength)
        {
            for (i = iAutoPhrase - 1; i >= 0; i--)
            {
                if (!TableCompareCode (strCodeInput, autoPhrase[i].strCode) && autoPhrase[i].flag)
                {
                    if (TableHasPhrase (autoPhrase[i].strCode, autoPhrase[i].strHZ))
                        TableAddAutoCandWord (i, mode);
                }
            }
        }

        if (iCandWordCount < iMaxCandWord)
        {
            while (currentRecord && currentRecord != recordHead)
            {
                if ((mode == SM_PREV) ^ (!currentRecord->flag))
                {
                    if (!TableCompareCode (strCodeInput, currentRecord->strCode) && CheckHZCharset (currentRecord->strHZ))
                    {
                        if (mode == SM_FIRST)
                            iTableTotalCandCount++;
                        TableAddCandWord (currentRecord, mode);
                        printf("%d %s %s\n",__LINE__, strCodeInput, currentRecord->strHZ);
                    }
                    else if(!strstr(strCodeInput,"z")) break;

                }
                currentRecord = currentRecord->next;
                if(iCandWordCount == iMaxCandWord)
                      break;
                //printf("%d %d %s\n",__LINE__, iCandWordCount, currentRecord->strHZ);

            }
        }

        if (table[0].bRule && table[0].bAutoPhrase && mode != SM_PREV && iCodeInputCount == table[0].iCodeLength)
        {
            for (i = iAutoPhrase - 1; i >= 0; i--)
            {
                if (!TableCompareCode (strCodeInput, autoPhrase[i].strCode) && !autoPhrase[i].flag)
                {
                    if (TableHasPhrase (autoPhrase[i].strCode, autoPhrase[i].strHZ))
                    {
                        if (mode == SM_FIRST)
                            iTableTotalCandCount++;
                        TableAddAutoCandWord (i, mode);
                    }
                }
            }
        }

        TableSetCandWordsFlag (iCandWordCount, True);

        if (mode != SM_PREV)
            iTableCandDisplayed += iCandWordCount;

        //由于iCurrentCandPage和iCandPageCount用来指示是否显示上/下翻页图标，因此，此处需要设置一下
        iCurrentCandPage = (iTableCandDisplayed == iCandWordCount) ? 0 : 1;
        iCandPageCount = (iTableCandDisplayed >= iTableTotalCandCount) ? 1 : 2;
        if (iCandWordCount == iTableTotalCandCount)
            iCandPageCount = 0;
        /* **************************************************** */
    }

    if (bPointAfterNumber)
    {
        strTemp[1] = '.';
        strTemp[2] = '\0';
    }
    else
        strTemp[1] = '\0';



    for (i = 0; i < iCandWordCount; i++)
    {
        strTemp[0] = i + 1 + '0';
        if (i == 9)
            strTemp[0] = '0';

        if (tableCandWord[i].flag == CT_PYPHRASE)
        {
            if (strlen (tableCandWord[i].candWord.strPYPhrase) == 2)
            {
                recTemp = tableSingleHZ[CalHZIndex (tableCandWord[i].candWord.strPYPhrase)];

                if (!recTemp)
                    pstr = (char *) NULL;
                else
                    pstr = recTemp->strCode;
            }
            else
            pstr = (char *) NULL;
        }
        else if ((tableCandWord[i].flag == CT_NORMAL) && (tableCandWord[i].candWord.record->bPinyin))
        {
            if (strlen (tableCandWord[i].candWord.record->strHZ) == 2)
            {
                recTemp = tableSingleHZ[CalHZIndex (tableCandWord[i].candWord.record->strHZ)];
                if (!recTemp)
                    pstr = (char *) NULL;
                else
                    pstr = recTemp->strCode;
            }
            else
                pstr = (char *) NULL;
        }
        else if (HasMatchingKey ())
            pstr = (tableCandWord[i].flag == CT_NORMAL) ? tableCandWord[i].candWord.record->strCode : tableCandWord[i].candWord.autoPhrase->strCode;
        else
            pstr = ((tableCandWord[i].flag == CT_NORMAL) ? tableCandWord[i].candWord.record->strCode : tableCandWord[i].candWord.autoPhrase->strCode) + iCodeInputCount;

    }

    return IRV_DISPLAY_CANDWORDS;
}

void TableAddAutoCandWord (INT16 which, SEARCH_MODE mode)
{
    int             i, j;

    if (mode == SM_PREV)
    {
        if (iCandWordCount == iMaxCandWord)
        {
            i = iCandWordCount - 1;
            for (j = 0; j < iCandWordCount - 1; j++)
                tableCandWord[j].candWord.autoPhrase = tableCandWord[j + 1].candWord.autoPhrase;
        }
        else
            i = iCandWordCount++;
        tableCandWord[i].flag = CT_AUTOPHRASE;
        tableCandWord[i].candWord.autoPhrase = &autoPhrase[which];
    }
    else {
        if (iCandWordCount == iMaxCandWord)
            return;

        tableCandWord[iCandWordCount].flag = CT_AUTOPHRASE;
        tableCandWord[iCandWordCount++].candWord.autoPhrase = &autoPhrase[which];
    }
}

void TableAddCandWord (RECORD * record, SEARCH_MODE mode)
{
    int             i = 0, j;

    switch (table[0].tableOrder)
    {
        case AD_NO:
            if (mode == SM_PREV)
            {
                if (iCandWordCount == iMaxCandWord)
                    i = iCandWordCount - 1;
                else {
                    for (i = 0; i < iCandWordCount; i++)
                        if (tableCandWord[i].flag != CT_NORMAL)
                            break;

                }
            }
            else {
                if (iCandWordCount == iMaxCandWord)
                    return;
                tableCandWord[iCandWordCount].flag = CT_NORMAL;
                tableCandWord[iCandWordCount++].candWord.record = record;
                return;
            }
            break;
        default:
            break;
    }

    if (mode == SM_PREV) {
        if (iCandWordCount == iMaxCandWord) {
            for (j = 0; j < i; j++) {
                tableCandWord[j].flag = tableCandWord[j + 1].flag;
                if (tableCandWord[j].flag == CT_NORMAL)
                    tableCandWord[j].candWord.record = tableCandWord[j + 1].candWord.record;
                else
                    tableCandWord[j].candWord.autoPhrase = tableCandWord[j + 1].candWord.autoPhrase;
                }
        }
        else {
            for (j = iCandWordCount; j > i; j--) {
                tableCandWord[j].flag = tableCandWord[j - 1].flag;
                if (tableCandWord[j].flag == CT_NORMAL)
                    tableCandWord[j].candWord.record = tableCandWord[j - 1].candWord.record;
                else
                    tableCandWord[j].candWord.autoPhrase = tableCandWord[j - 1].candWord.autoPhrase;
            }
        }
    }
    else {
        j = iCandWordCount;
        if (iCandWordCount == iMaxCandWord)
            j--;
        for (; j > i; j--)
        {
            tableCandWord[j].flag = tableCandWord[j - 1].flag;
            if (tableCandWord[j].flag == CT_NORMAL)
                tableCandWord[j].candWord.record = tableCandWord[j - 1].candWord.record;
            else
                tableCandWord[j].candWord.autoPhrase = tableCandWord[j - 1].candWord.autoPhrase;
        }
    }

    tableCandWord[i].flag = CT_NORMAL;
    tableCandWord[i].candWord.record = record;

    if (iCandWordCount != iMaxCandWord)
        iCandWordCount++;
}

void TableResetFlags (void)
{
    RECORD         *record = recordHead->next;
    INT16           i;

    while (record != recordHead)
    {
        record->flag = False;
        record = record->next;
    }
    for (i = 0; i < iAutoPhrase; i++)
        autoPhrase[i].flag = False;
}

void TableSetCandWordsFlag (int iCount, Bool flag)
{
    int             i;

    for (i = 0; i < iCount; i++)
    {

        if (tableCandWord[i].flag == CT_NORMAL)
            tableCandWord[i].candWord.record->flag = flag;
        else
            tableCandWord[i].candWord.autoPhrase->flag = flag;
    }
}
Bool HasMatchingKey (void)
{
    char           *str;

    str = strCodeInput;
    while (*str) {
        if (*str++ == table[0].cMatchingKey)
            return True;
    }
    return False;
}

int TableCompareCode (char *strUser, char *strDict)
{
    int             i;

    for (i = 0; i < strlen (strUser); i++)
    {
        if (!strDict[i])
            return strUser[i];
        if (strUser[i] != table[0].cMatchingKey || !table[0].bUseMatchingKey)
        {
            if (strUser[i] != strDict[i])
                return (strUser[i] - strDict[i]);
        }
    }
    if (table[0].bTableExactMatch)
    {
        if (strlen (strUser) != strlen (strDict))
            return -999;	//随意的一个值
    }

    return 0;
}

int TableFindFirstMatchCode (void)
{
    int             i = 0;

    if (!recordHead)
        return -1;

    if (table[0].bUseMatchingKey && (strCodeInput[0] == table[0].cMatchingKey))
        i = 0;
    else
    {
        while (strCodeInput[0] != recordIndex[i].cCode)
        {
            if (!recordIndex[i].cCode)
                break;
            i++;
        }
    }
    currentRecord = recordIndex[i].record;
    if (!currentRecord)
        return -1;

    while (currentRecord != recordHead)
    {
        if (!TableCompareCode (strCodeInput, currentRecord->strCode))
        {
            //printf("%s\n",currentRecord->strHZ);
            if (CheckHZCharset (currentRecord->strHZ))
                return i;
        }
        currentRecord = currentRecord->next;
        i++;
    }

    return -1;			//Not found
}

/*
 * 反查编码
 * bMode=True表示用于组词，此时不查一、二级简码。但如果只有二级简码时返回二级简码，不查一级简码
 */
/*RECORD         *TableFindCode (char *strHZ, Bool bMode)
{
    RECORD         *recShort = NULL;	//记录二级简码的位置
    int             i;

    for (i = 0; i < iSingleHZCount; i++) {
    if (!strcmp (tableSingleHZ[i]->strHZ, strHZ) && !IsIgnoreChar (tableSingleHZ[i]->strCode[0])) {
        if (!bMode)
        return tableSingleHZ[i];

        if (strlen (tableSingleHZ[i]->strCode) == 2)
        recShort = tableSingleHZ[i];
        else if (strlen (tableSingleHZ[i]->strCode) > 2)
        return tableSingleHZ[i];
    }
    }

    return recShort;
}*/

/*
 * 根据序号调整词组顺序，序号从1开始
 * 将指定的字/词调整到同样编码的最前面
 */
void TableAdjustOrderByIndex (int iIndex)
{
    RECORD         *recTemp;
    int             iTemp;

    if (tableCandWord[iIndex - 1].flag != CT_NORMAL)
        return;

    recTemp = tableCandWord[iIndex - 1].candWord.record;
    while (!strcmp (recTemp->strCode, recTemp->prev->strCode))
        recTemp = recTemp->prev;
    if (recTemp == tableCandWord[iIndex - 1].candWord.record)	//说明已经是第一个
        return;

    //将指定的字/词放到recTemp前
    tableCandWord[iIndex - 1].candWord.record->prev->next = tableCandWord[iIndex - 1].candWord.record->next;
    tableCandWord[iIndex - 1].candWord.record->next->prev = tableCandWord[iIndex - 1].candWord.record->prev;
    recTemp->prev->next = tableCandWord[iIndex - 1].candWord.record;
    tableCandWord[iIndex - 1].candWord.record->prev = recTemp->prev;
    recTemp->prev = tableCandWord[iIndex - 1].candWord.record;
    tableCandWord[iIndex - 1].candWord.record->next = recTemp;

    iTableChanged++;

    //需要的话，更新索引
    if (tableCandWord[iIndex - 1].candWord.record->strCode[1] == '\0')
    {
        for (iTemp = 0; iTemp < strlen (table[0].strInputCode); iTemp++)
        {
            if (recordIndex[iTemp].cCode == tableCandWord[iIndex - 1].candWord.record->strCode[0])
            {
                recordIndex[iTemp].record = tableCandWord[iIndex - 1].candWord.record;
                break;
            }
        }
    }
    if (iTableChanged == 5)
        SaveTableDict ();
}

/*
 * 根据序号删除词组，序号从1开始
 */
void TableDelPhraseByIndex (int iIndex)
{
    if (tableCandWord[iIndex - 1].flag != CT_NORMAL)
        return;

    if (strlen (tableCandWord[iIndex - 1].candWord.record->strHZ) <= 2)
        return;

    TableDelPhrase (tableCandWord[iIndex - 1].candWord.record);
}

/*
 * 根据字串删除词组
 */
void TableDelPhraseByHZ (char *strHZ)
{
    RECORD         *recTemp;

    recTemp = TableFindPhrase (strHZ);
    if (recTemp)
        TableDelPhrase (recTemp);
}

void TableDelPhrase (RECORD * record)
{
    record->prev->next = record->next;
    record->next->prev = record->prev;

    free (record->strCode);
    free (record->strHZ);
    free (record);

    table[0].iRecordCount--;

    SaveTableDict ();
}

/*
 *判断某个词组是不是已经在词库中,有返回NULL，无返回插入点
 */
RECORD         *TableHasPhrase (char *strCode, char *strHZ)
{
    RECORD         *recTemp;
    int             i;

    i = 0;
    while (strCode[0] != recordIndex[i].cCode)
        i++;

    recTemp = recordIndex[i].record;
    while (recTemp != recordHead)
    {
        if (!recTemp->bPinyin)
        {
            if (strcmp (recTemp->strCode, strCode) > 0)
                break;
            else if (!strcmp (recTemp->strCode, strCode))
            {
                if (!strcmp (recTemp->strHZ, strHZ))	//该词组已经在词库中
                    return NULL;
            }
        }
        recTemp = recTemp->next;
    }

    return recTemp;
}

/*
 *根据字串判断词库中是否有某个字/词，注意该函数会忽略拼音词组
 */
RECORD         *TableFindPhrase (char *strHZ)
{
    RECORD         *recTemp;
    char            strTemp[3];
    int             i;

    //首先，先查找第一个汉字的编码
    strTemp[0] = strHZ[0];
    strTemp[1] = strHZ[1];
    strTemp[2] = '\0';

    recTemp = tableSingleHZ[CalHZIndex (strTemp)];
    if (!recTemp)
        return (RECORD *) NULL;

    //然后根据该编码找到检索的起始点
    i = 0;
    while (recTemp->strCode[0] != recordIndex[i].cCode)
        i++;

    recTemp = recordIndex[i].record;
    while (recTemp != recordHead)
    {
        if (recTemp->strCode[0] != recordIndex[i].cCode)
            break;
        if (!strcmp (recTemp->strHZ, strHZ))
        {
            if (recTemp->bPinyin)
                break;
            return recTemp;
        }

        recTemp = recTemp->next;
    }

    return (RECORD *) NULL;
}

void TableInsertPhrase (char *strCode, char *strHZ)
{
    RECORD         *insertPoint, *dictNew;

    insertPoint = TableHasPhrase (strCode, strHZ);

    if (!insertPoint)
        return;

    dictNew = (RECORD *) malloc (sizeof (RECORD));
    dictNew->strCode = (char *) malloc (sizeof (char) * (table[0].iCodeLength + 1));
    strcpy (dictNew->strCode, strCode);
    dictNew->strHZ = (char *) malloc (sizeof (char) * (strlen (strHZ) + 1));
    strcpy (dictNew->strHZ, strHZ);
    dictNew->iHit = 0;
    dictNew->iIndex = iTableIndex;

    dictNew->prev = insertPoint->prev;
    insertPoint->prev->next = dictNew;
    insertPoint->prev = dictNew;
    dictNew->next = insertPoint;

    table[0].iRecordCount++;

    SaveTableDict ();
}

void TableCreateNewPhrase (void)
{

}

void TableCreatePhraseCode (char *strHZ)
{
    unsigned char   i;
    unsigned char   i1, i2;
    size_t          iLen;
    char            strTemp[3];
    RECORD         *recTemp;

    strTemp[2] = '\0';
    bCanntFindCode = False;
    iLen = strlen (strHZ) / 2;
    if (iLen >= table[0].iCodeLength)
    {
        i2 = table[0].iCodeLength;
        i1 = 1;
    }
    else {
        i2 = iLen;
        i1 = 0;
    }

    for (i = 0; i < table[0].iCodeLength - 1; i++) {
        if (table[0].rule[i].iWords == i2 && table[0].rule[i].iFlag == i1)
            break;
    }

    for (i1 = 0; i1 < table[0].iCodeLength; i1++)
    {
        if (table[0].rule[i].rule[i1].iFlag)
        {
            strTemp[0] = strHZ[(table[0].rule[i].rule[i1].iWhich - 1) * 2];
            strTemp[1] = strHZ[(table[0].rule[i].rule[i1].iWhich - 1) * 2 + 1];
        }
        else {
            strTemp[0] = strHZ[(iLen - table[0].rule[i].rule[i1].iWhich) * 2];
            strTemp[1] = strHZ[(iLen - table[0].rule[i].rule[i1].iWhich) * 2 + 1];
        }

        recTemp = tableSingleHZ[CalHZIndex (strTemp)];

        if (!recTemp)
        {
            bCanntFindCode = True;
            break;
        }

        strNewPhraseCode[i1] = recTemp->strCode[table[0].rule[i].rule[i1].iIndex - 1];
    }
}

/*
 * 获取联想候选字列表
 */
INPUT_RETURN_VALUE TableGetLegendCandWords (SEARCH_MODE mode)
{
    char            strTemp[3];
    int             iLength;
    //int             i;
    RECORD         *tableLegend = NULL;
    unsigned int    iTableTotalLengendCandCount = 0;

    if (!strTableLegendSource[0])
        return IRV_TO_PROCESS;

    iLength = strlen (strTableLegendSource);
    if (mode == SM_FIRST)
    {
        iCurrentLegendCandPage = 0;
        iLegendCandPageCount = 0;
        TableResetFlags ();
    }
    else {
        if (!iLegendCandPageCount)
            return IRV_TO_PROCESS;

        if (mode == SM_NEXT)
        {
            if (iCurrentLegendCandPage == iLegendCandPageCount)
            return IRV_DO_NOTHING;

            iCurrentLegendCandPage++;
        }
        else {
            if (!iCurrentLegendCandPage)
            return IRV_DO_NOTHING;

            TableSetCandWordsFlag (iLegendCandWordCount, False);
            iCurrentLegendCandPage--;
        }
    }

    iLegendCandWordCount = 0;
    tableLegend = recordHead->next;

    while (tableLegend != recordHead)
    {
        if (((mode == SM_PREV) ^ (!tableLegend->flag)) && ((iLength + 2) == strlen (tableLegend->strHZ)))
        {
            if (!strncmp (tableLegend->strHZ, strTableLegendSource, iLength) && tableLegend->strHZ[iLength] && CheckHZCharset (tableLegend->strHZ))
            {
                if (mode == SM_FIRST)
                    iTableTotalLengendCandCount++;
                TableAddLegendCandWord (tableLegend, mode);
            }
        }

        tableLegend = tableLegend->next;
    }

    TableSetCandWordsFlag (iLegendCandWordCount, True);

    if (mode == SM_FIRST && bDisablePagingInLegend)
        iLegendCandPageCount = iTableTotalLengendCandCount / iMaxCandWord - ((iTableTotalLengendCandCount % iMaxCandWord) ? 0 : 1);

    if (bPointAfterNumber) {
    strTemp[1] = '.';
    strTemp[2] = '\0';
    }
    else
    strTemp[1] = '\0';


    bIsInLegend = (iLegendCandWordCount != 0);

    return IRV_DISPLAY_CANDWORDS;
}

void TableAddLegendCandWord (RECORD * record, SEARCH_MODE mode)
{
    int             i, j;

    if (mode == SM_PREV)
    {
        for (i = iLegendCandWordCount - 1; i >= 0; i--) {
            if (tableCandWord[i].candWord.record->iHit >= record->iHit)
            break;
        }

        if (iLegendCandWordCount == iMaxCandWord) {
            if (i < 0)
            return;
        }
        else
            i++;
    }
    else
    {
        for (i = 0; i < iLegendCandWordCount; i++) {
            if (tableCandWord[i].candWord.record->iHit < record->iHit)
            break;
        }
        if (i == iMaxCandWord)
            return;
    }

    if (mode == SM_PREV)
    {
        if (iLegendCandWordCount == iMaxCandWord) {
            for (j = 0; j < i; j++)
            tableCandWord[j].candWord.record = tableCandWord[j + 1].candWord.record;
        }
        else {
            for (j = iLegendCandWordCount; j > i; j--)
            tableCandWord[j].candWord.record = tableCandWord[j - 1].candWord.record;
        }
    }
    else
    {
        j = iLegendCandWordCount;
        if (iLegendCandWordCount == iMaxCandWord)
            j--;
        for (; j > i; j--)
            tableCandWord[j].candWord.record = tableCandWord[j - 1].candWord.record;
    }

//#warning *****************************************
//#warning FIX ME!
//#warning *****************************************
    //第一个FIX ME！处的问题与下面这行代码好象有关系
    tableCandWord[i].flag = CT_NORMAL;
    tableCandWord[i].candWord.record = record;

    if (iLegendCandWordCount != iMaxCandWord)
        iLegendCandWordCount++;
}

char           *TableGetLegendCandWord (int iIndex)
{
    if (iLegendCandWordCount)
    {
        if (iIndex > (iLegendCandWordCount - 1))
            iIndex = iLegendCandWordCount - 1;

        tableCandWord[iIndex].candWord.record->iHit++;
        strcpy (strTableLegendSource, tableCandWord[iIndex].candWord.record->strHZ + strlen (strTableLegendSource));
        TableGetLegendCandWords (SM_FIRST);

        return strTableLegendSource;
    }
    return NULL;
}

INPUT_RETURN_VALUE TableGetFHCandWords (SEARCH_MODE mode)
{
    char            strTemp[3];
    int             i = 0;

    if (!iFH)
        return IRV_DISPLAY_MESSAGE;

    if (bPointAfterNumber) {
        strTemp[1] = '.';
        strTemp[2] = '\0';
    }
    else
        strTemp[1] = '\0';

    if (mode == SM_FIRST) {
        iCandPageCount = iFH / iMaxCandWord - ((iFH % iMaxCandWord) ? 0 : 1);
        iCurrentCandPage = 0;
    }
    else {
        if (!iCandPageCount)
            return IRV_TO_PROCESS;

        if (mode == SM_NEXT) {
            if (iCurrentCandPage == iCandPageCount)
                return IRV_DO_NOTHING;

            iCurrentCandPage++;
        }
        else {
            if (!iCurrentCandPage)
                return IRV_DO_NOTHING;

            iCurrentCandPage--;
        }
    }

    iCandWordCount = i;
    return IRV_DISPLAY_CANDWORDS;
}

char           *TableGetFHCandWord (int iIndex)
{
    if (iCandWordCount)
    {
        if (iIndex > (iCandWordCount - 1))
            iIndex = iCandWordCount - 1;

        return fh[iCurrentCandPage * iMaxCandWord + iIndex].strFH;
    }
    return NULL;
}

Bool TablePhraseTips (void)
{
    return False;
}

void TableCreateAutoPhrase (INT8 iCount)
{
    char *strHZ;
    short int i,j,k;

    if (!autoPhrase)
        return;

    strHZ=(char *)malloc((table[0].iAutoPhrase * 2 + 1)*sizeof(char));
    j = iHZLastInputCount - table[0].iAutoPhrase - iCount;
    if (j < 0)
        j = 0;

    for (; j < iHZLastInputCount - 1; j++)
    {
        for (i = table[0].iAutoPhrase; i >= 2; i--)
        {
            if ((j + i - 1) > iHZLastInputCount)
                continue;

            strcpy (strHZ, hzLastInput[j].strHZ);
            for (k = 1; k < i; k++)
                strcat (strHZ, hzLastInput[j + k].strHZ);

            for (k = 0; k < iAutoPhrase; k++)
            {
                if (!strcmp (autoPhrase[k].strHZ, strHZ))
                    goto _next;
            }
            if (TableFindPhrase (strHZ))
                goto _next;

            TableCreatePhraseCode (strHZ);
            if (iAutoPhrase != AUTO_PHRASE_COUNT)
            {
                autoPhrase[iAutoPhrase].flag = False;
                strcpy (autoPhrase[iAutoPhrase].strCode, strNewPhraseCode);
                strcpy (autoPhrase[iAutoPhrase].strHZ, strHZ);
                autoPhrase[iAutoPhrase].iSelected = 0;
                iAutoPhrase++;
            }
            else
            {
                insertPoint->flag = False;
                strcpy (insertPoint->strCode, strNewPhraseCode);
                strcpy (insertPoint->strHZ, strHZ);
                insertPoint->iSelected = 0;
                insertPoint = insertPoint->next;
            }

          _next:
            continue;
        }

    }

    free(strHZ);
    /*
       for (i=0;i<iAutoPhrase;i++ ) {
       printf("%d: %s  %s\n",i+1,autoPhrase[i].strCode,autoPhrase[i].strHZ);
       puts("===========================================");
       }
     */
}

void UpdateHZLastInput (char *str)
{
    int             i, j;

    for (i = 0; i < strlen (str) / 2; i++)
    {
        if (iHZLastInputCount < PHRASE_MAX_LENGTH)
            iHZLastInputCount++;
        else
        {
            for (j = 0; j < (iHZLastInputCount - 1); j++)
            {
                hzLastInput[j].strHZ[0] = hzLastInput[j + 1].strHZ[0];
                hzLastInput[j].strHZ[1] = hzLastInput[j + 1].strHZ[1];
            }
        }
        hzLastInput[iHZLastInputCount - 1].strHZ[0] = str[2 * i];
        hzLastInput[iHZLastInputCount - 1].strHZ[1] = str[2 * i + 1];
        hzLastInput[iHZLastInputCount - 1].strHZ[2] = '\0';
    }

    if (table[0].bRule && (table->bAutoPhrase))
        TableCreateAutoPhrase ((INT8) (strlen (str) / 2));
}

#endif
