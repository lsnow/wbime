#include <QCoreApplication>
//#include <QServiceManager>

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "wbimengine.h"

extern "C"
{
#include "table.h"
}

#define BKSP_SINGLE_CHR 1
#define SPACE_CHR ' '

wbimengine::wbimengine(QObject* parent)
    : QObject(parent),
      predictionEnabledFlag(true),
      correctionEnabledFlag(true),
      completionEnabledFlag(true),
      maximumErrorsCount(0),
      exactWordPos(MImEngine::ExactInListFirst)
{
    fprintf(stderr,"%s\n",__func__);
    //LoadTableInfo();
}

wbimengine::~wbimengine()
{
    fprintf(stderr,"%s\n",__func__);
    FreeTableIM ();
}

/*!
 *
 */
bool wbimengine::init()
{
    fprintf(stderr,"%s\n",__func__);
    LoadTableInfo();
    return true;
}

/*!
 * Clean up resources during the unload of prediction engine plug-in.
 */
// TODO: Kept as place holder. Currently there is no call back to inform plug-in to get unloaded.
// Once that API is available, below function needs to be renamed and re-tested.
#ifdef mmmmm
bool wbimengine::cleanUpDriver()
{
    FreeTableIM();
    fprintf(stderr,"%s\n",__func__);
    return true;
}
#endif

/*!
 * Adding words to specified dictionary type.
 * Currently we are only adding words to user personal dictionary.
 */
bool wbimengine::addDictionaryWord(const QString &aWord, MImEngine::DictionaryType type)
{
    Q_UNUSED(aWord);
    Q_UNUSED(type);
    fprintf(stderr,"%s\n",__func__);
    return true;
}

/*!
 * Removing words from specified dictionary type.
 * Currently we are only removing words from user personal dictionary.
 */
bool wbimengine::removeDictionaryWord(const QString &aWord, MImEngine::DictionaryType type)
{
    Q_UNUSED(aWord);
    Q_UNUSED(type);
    fprintf(stderr,"%s\n",__func__);
    return true;
   
}

/*!
 * Disable the currently active language dictionary.
 */
bool wbimengine::disableDictionary(MImEngine::DictionaryType type)
{
   fprintf(stderr,"%s\n",__func__);
   Q_UNUSED(type);
   return true;
}

/*!
 * Enable the language dictionary for the current active language.
 */
bool wbimengine::enableDictionary(MImEngine::DictionaryType type)
{
    fprintf(stderr,"%s\n",__func__);
    Q_UNUSED(type);
    return true;
}

bool wbimengine::removeDictionary(MImEngine::DictionaryType type)
{
    fprintf(stderr,"%s\n",__func__);
    Q_UNUSED(type);
    return true;
}

/*!
 * Appending a string to the core engine buffer.
 */
void wbimengine::appendString(const QString &s)
{
    char *istr;
    QByteArray ba = s.toLatin1();
    istr = ba.data();
    int i = (strlen(istr) > 4) ? 4 : strlen(istr), j = strlen (strCodeInput),k;
    memset (strCodeInput, 0, j);
    for(k = 0; k < i-1; k++)
        strCodeInput[k] = istr[k];
    iCodeInputCount = i-1;
    DoTableInput(istr[i-1]);

    fprintf(stderr,"%s %d %s, %s\n",__func__, i, istr,strCodeInput);
    memset (strCodeInput, 0, j);
    //free(istr);
    //s.clear();
    ba.clear();
}

/*!
 * Appending a character to the core engine buffer.
 */
void wbimengine::appendCharacter(const QChar &aChar)
{
    //Q_UNUSED(aChar);
    //strCodeInput[strlen(strCodeInput)] = *aChar;
    iCodeInputCount = strlen(strCodeInput);
    char ikey = aChar.toAscii();
    DoTableInput(ikey);
    fprintf(stderr,"%s\n",__func__);
}

/*!
 * Appending a character from the tapKeyboard to the core engine buffer.
 */
void wbimengine::tapKeyboard(const QPoint &position, bool shift, QChar symbol)
{
    //Q_UNUSED(position);
    printf("%d %d\n",position.x(),position.y());
    fprintf(stderr,"%s\n",__func__);
    Q_UNUSED(shift);
    //Q_UNUSED(symbol);
    appendCharacter(symbol);
}

/*!
 * reselectString called when backspace is pressed during pre-edit.
 * Removing the current composition and Appending the string to the core engine buffer.
 */
void wbimengine::reselectString(const QString &s)
{
     Q_UNUSED(s);
     fprintf(stderr,"%s\n",__func__);
     strCodeInput[strlen(strCodeInput) - 1] = '\0';
     DoTableInput(0);
}

/*!
 * Helper function to refresh the candidates list. This function needs to be called when
 * there is a change in input buffer content and new suggestion list is desired.
 */
 

void wbimengine::refreshCandidateList()
{
    fprintf(stderr,"%s\n",__func__);
}


/*!
 * To set the context; core engine buffer is reset only when the setContext is called as part of
 * Cut,Paste or any other action that requires core buffer to be reset.
 * Append the string without learning, so that it will not increase any contextual statistics.
 * Moving the cursor to the given position.
 */
void wbimengine::setContext(const QString &aContextStr, int cursor)
{
    Q_UNUSED(aContextStr);
    Q_UNUSED(cursor);
    fprintf(stderr,"%s %d\n",__func__, cursor);
}


void wbimengine::disablePrediction()
{
    predictionEnabledFlag = false;
    fprintf(stderr,"%s\n",__func__);
}

/*!
 * Prediction is enabled.
 */
void wbimengine::enablePrediction()
{
    predictionEnabledFlag = true;
    fprintf(stderr,"%s\n",__func__);
}

/*!
 * is Prediction is enabled.
 */
bool wbimengine::predictionEnabled()
{
    fprintf(stderr,"%s\n",__func__);
    return predictionEnabledFlag;    
}

/*!
 * Correction is disabled.
 */
void wbimengine::disableCorrection()
{
    correctionEnabledFlag = false;
    fprintf(stderr,"%s\n",__func__);
}

/*!
 * Correction is enabled.
 */
void wbimengine::enableCorrection()
{
    correctionEnabledFlag = true;
    fprintf(stderr,"%s\n",__func__);
}

/*!
 * is Correction is disabled.
 */
bool wbimengine::correctionEnabled()
{
    fprintf(stderr,"%s\n",__func__);
    return correctionEnabledFlag;
}

/*!
 * Completion is disabled.
 */
void wbimengine::disableCompletion()
{
    completionEnabledFlag = false;
    fprintf(stderr,"%s\n",__func__);
}

/*!
 * Completion is enabled.
 */
void wbimengine::enableCompletion()
{
    fprintf(stderr,"%s\n",__func__);
    completionEnabledFlag = true;
}

/*!
 * is Completion is enabled.
 */
bool wbimengine::completionEnabled()
{
    fprintf(stderr,"%s\n",__func__);
    return completionEnabledFlag;
}

/*!
 * Populates the candidate prior to this call
 * Returns the candidates List.
 */
QStringList wbimengine::candidates(unsigned int uStartIndex, unsigned int uNum)
{
    QStringList testlist;
    QString temp;
    fprintf(stderr,"%s unum= %d\n",__func__, uNum);
    int i;
    uNum = (uNum < iCandWordCount) ? uNum : iCandWordCount;
    for(i = uStartIndex; i < uNum; i++)
    {
        fprintf(stderr, "%s\n", tableCandWord[i].candWord.record->strHZ);
        temp = QString().fromUtf8( tableCandWord[i].candWord.record->strHZ, -1);
        testlist += temp;
    }
    return testlist;
}

/*!
 * Returns the total number of elements in the candidates List.
 */
int wbimengine::totalCandidates()
{
    fprintf(stderr,"%s\n",__func__);
    return iCandWordCount;
}

/*!
 * Returns number if character matches in the composition and the choosen word
 */
int wbimengine::matchedLength()
{
    int match = strlen(strCodeInput);
    fprintf(stderr,"%s\n",__func__);
    return match;
}

QStringList wbimengine::matchedSyllables()
{
    fprintf(stderr,"%s\n",__func__);
    int i = strlen (strCodeInput),j;
    QStringList ret;
    for(j=0; j<i; j++)
        ret += QString().fromAscii(strCodeInput+j, 1);
    return ret;
}

int wbimengine::suggestedCandidateIndex()
{
    fprintf(stderr,"%s\n",__func__);
    return 0;
}

/*! This API should be called, to inform engine which one is the user preferred,
 * then the engine can store the relative information( new word, used frequency, etc.)
 *
 * Call stack when user selects a suggestion from the candidate popup list.
 * setSuggestedCandidateIndex()
 * saveAndClearEngineBuffer()
 * clearEngineBuffer()
 * setContext()
 */

bool  wbimengine::setSuggestedCandidateIndex(int index)
{
    Q_UNUSED(index);
    fprintf(stderr,"%s\n",__func__);
    return true;
}

/*!
 * Setting related to exact-typed word position in the list.
 */
void wbimengine::setExactWordPositionInList(MImEngine::ExactInListType setting)
{
    fprintf(stderr,"%s\n",__func__);
    Q_UNUSED(setting);
    //exactWordPos = setting;
}

/*!
 * Which dictionary type the candidate belongs to.
 */
MImEngine::DictionaryType wbimengine::candidateSource(int index)
{
    Q_UNUSED(index);
    fprintf(stderr,"%s\n",__func__);
    return  MImEngine::DictionaryTypeInvalid;
}

/*!
 * Clearing the compostion,current word and also candidate list.
 */
void wbimengine::clearEngineBuffer()
{
    fprintf(stderr,"%s\n",__func__);
    int i = strlen(strCodeInput), k;
    for (k=0; k<i; k++)
        strCodeInput[k] = '\0';
}


/*!
 * saveAndClearEngineBuffer gets called when user selects sugestion from the candidate pop-up.
 * This gets called after call to bool wbimengine::setSuggestedCandidateIndex(int)
 * Removing the composition characters from the coreEngine.
 * Committing the word to the core engine so contextual statistics can be updated.
 * Then reseting the core engine buffer content.
 */
void wbimengine::saveAndClearEngineBuffer()
{  
    int i = strlen(strCodeInput), k;
    for (k=0; k<i; k++)
        strCodeInput[k] = '\0';
    fprintf(stderr,"%s\n",__func__);
}

/*!
 * Returns currently active driver language.
 */
//QString driverLanguage = "zh@pingyin";
QString  wbimengine::language()
{
    fprintf(stderr,"%s\n",__func__);
    return QString("zh@wubi");
}

/*!
 * Sets the language to be currently active driver language.
 */
bool wbimengine::setLanguage(const QString  &l, MImEngine::LanguagePriority p)
{
	Q_UNUSED(p);
    fprintf(stderr,"%s\n",__func__);
	driverLanguage = l;
    return true;
}

/*!
 * Sets the the given keyboard layout to be currently active keyboard layout.
 * A converted keyboard layout is passed to KPT core engine for further use.
 */
bool wbimengine::setKeyboardLayoutKeys(const QList<MImEngine::KeyboardLayoutKey> &keys)
{
    fprintf(stderr,"%s\n",__func__);
    layoutKeys = keys;   
    return true;
}

/*!
 * Returns the active key layout.
 */
QList<MImEngine::KeyboardLayoutKey> wbimengine::keyboardLayoutKeys()
{
    fprintf(stderr,"%s\n",__func__);
    return layoutKeys;
}

/*!
 * Vendor specific API.
 */
bool wbimengine::exportAsNokiaDictionary(const QString &fileName)
{
    Q_UNUSED(fileName);
    fprintf(stderr,"%s\n",__func__);
    return true;
}

/*!
 * Vendor specific API.
 */
bool wbimengine::importNokiaUserDictionary(const QString &fileName)
{
    Q_UNUSED(fileName);
    fprintf(stderr,"%s\n",__func__);
    return true;
}

void wbimengine::setMaximumErrors(int maxErrors)
{
    maximumErrorsCount = maxErrors;
}

int wbimengine::maximumErrors()
{
    return maximumErrorsCount;
}

/*!
 * errorcorrection/prediction plug-in driver name.
 */
 
QString wbimengine::driver()
{
    return "wbimengine";
}

/*!
 * Setting the maximum number of suggestions to be generated from core engine.
 */
void wbimengine::setMaximumCandidates(int maxCandidates)
{
    fprintf(stderr,"%s\n",__func__);
    iMaxCandWord = maxCandidates > 5 ? 5 : maxCandidates;
}

/*!
 * maximum number of suggestions currently being generated from KPT core engine.
 */
int wbimengine::maximumCandidates()
{
    fprintf(stderr,"%s\n",__func__);
    int maxCandidates = iMaxCandWord;

    return maxCandidates;
}

/*!
 * Error occurred  during any operation.
 */
QString wbimengine::error()
{
    fprintf(stderr,"%s\n",__func__);
    QString err = "errors";
    return err;
}

/*!
 * Setting the script to be used.
 */


bool wbimengine::setScript(const QString &s)
{
    fprintf(stderr,"%s\n",__func__);
    Q_UNUSED(s);
    return true;
}

/*!
 * currently being used script.
 */
QString wbimengine::script()
{
    fprintf(stderr,"%s\n",__func__);
    QString driverScript("wbimengine");
    return driverScript;
}

QString wbimengine::transliterate(const QString &targetScript, unsigned int index)
{
    Q_UNUSED(targetScript);
    Q_UNUSED(index);
    QString tran ("targetScript");
    fprintf(stderr,"%s\n",__func__);
    return tran;
}

void wbimengine::insertCharacters(const QString &text, int index)
{
    fprintf(stderr,"%s\n",__func__);
}

void wbimengine::removeCharacters(int count, int index)
{
    fprintf(stderr,"%s\n",__func__);
}

void wbimengine::commitWord(int index, bool ignoreNewWord)
{
    fprintf(stderr,"%s\n",__func__);
}

Q_EXPORT_PLUGIN2(wbimengine, wbimengine);
