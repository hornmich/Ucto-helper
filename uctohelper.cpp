#include "uctohelper.h"

bool UctoHelper::calculateAlignment(const QString &newVal, const QString &oldVal, int *prefixLen, int *suffixLen) const
{
    int oldLen = oldVal.length();
    int newLen = newVal.length();

    if (oldLen < newLen) {
        std::cerr << "New value \"" << newVal.toStdString() << "\" will not fit to the place release by value \"" << oldVal.toStdString() << "\"" << std::endl;
        *prefixLen = *suffixLen = 0;
        return false;
    }
    else if (oldLen == newLen) {
        *prefixLen = *suffixLen = 0;
    }
    else {
        *prefixLen = (oldLen - newLen + 1) / 2;
        *suffixLen = (oldLen - newLen) / 2;
    }
    return true;
}

UctoHelper::UctoHelper()
{
    mPatternBillPerDaySource = "\\d{2}\\.";
    mPatternBillPerMonthSource = "\\d{2}\\.";
    mPatternBillPerYearSource = "\\d{4}";
    mPatternBillPerSource = "<b>\\d{2}\\.\\d{2}\\.\\d{4} až \\d{2}\\.\\d{2}\\.\\d{4}</b>";
    mPatternBillPerMonthsDest = "...........";
    mPatternBillPerYearDest = "...........";
    mPatternBillPerYearLine = "- na zdaňovací období";
    mPatternBillPerMonthsLine = "na tyto měsíce zdaňovacího období";
    mPatternSignature = "podepsal/nepodepsal";
    mSignedText = "podepsal";
    mNotSignedText = "nepodepsal";
    mDocument = UctoDocument();
    mBillingPeriod = BillingPeriod();
}

bool UctoHelper::openFile(const QString &fileName)
{
    if (!mDocument.loadDocument(fileName)) {
        std::cerr << "File loading: " << fileName.toStdString() << " failed.\n";
        return false;
    }
    return true;
}

bool UctoHelper::saveFile(const QString &fileName) const
{
    if (!mDocument.saveDocument(fileName)) {
        std::cerr << "File saving: " << fileName.toStdString() << "failed.\n";
        return false;
    }
    return true;
}

bool UctoHelper::retrieveBillinPeriod()
{
    int lineBPeriodSource = mDocument.findLine(mPatternBillPerSource);
    if (lineBPeriodSource == -1) {
        std::cerr << "Billing period not found in the document" << std::endl;
        return false;
    }

    if (!mBillingPeriod.findPeriod(mDocument.getLine(lineBPeriodSource), mPatternBillPerDaySource, mPatternBillPerMonthSource, mPatternBillPerYearSource)) {
        std::cerr << "Billing period pattern not found in the document." << std::endl;
        return false;
    }
    return true;
}

bool UctoHelper::includeBillingPeriodYear()
{
    int lineBPerYearDest = mDocument.findLine(mPatternBillPerYearLine);
    if (lineBPerYearDest == -1) {
        std::cerr << "Billing period column not found." << std::endl;
        return false;
    }

    QString lineBPerYearStr = mDocument.getLine(lineBPerYearDest);

    QString delimiter = "-";
    QString inVal;
    if (mBillingPeriod.startYear() != mBillingPeriod.endYear()) {
        inVal = mBillingPeriod.startYearStr() + delimiter + mBillingPeriod.endYearStr();
    }
    else {
        inVal = mBillingPeriod.startYearStr();
    }
    int prefixLen;
    int sufixLen;
    QString prefix;
    QString sufix;
    if (!calculateAlignment(inVal, mPatternBillPerYearDest, &prefixLen, &sufixLen)) {
        std::cerr << "Text alignment may break the table layout." << std::endl;
    }
    prefix.fill(' ', prefixLen);
    sufix.fill(' ', sufixLen);

    lineBPerYearStr.replace(mPatternBillPerYearDest, prefix + inVal + sufix);
    mDocument.setLine(lineBPerYearDest, lineBPerYearStr);
    return true;
}

bool UctoHelper::includeBillingPeriodMonths()
{
    int lineBPerMonthsDest = mDocument.findLine(mPatternBillPerMonthsLine);
    if (lineBPerMonthsDest == -1) {
        std::cerr << "Billing period months column not found." << std::endl;
        return false;
    }
    QString lineBPerMonthsStr = mDocument.getLine(lineBPerMonthsDest);
    QString delimiter = " - ";

    QString inVal = mBillingPeriod.startMonthStr() + delimiter + mBillingPeriod.endMonthStr();
    int prefixLen;
    int sufixLen;
    QString prefix;
    QString sufix;
    if (!calculateAlignment(inVal, mPatternBillPerMonthsDest, &prefixLen, &sufixLen)) {
        std::cerr << "Text alignment may break the table layout." << std::endl;
    }
    prefix.fill(' ', prefixLen);
    sufix.fill(' ', sufixLen);

    lineBPerMonthsStr.replace(mPatternBillPerMonthsDest, prefix + inVal + sufix);
    mDocument.setLine(lineBPerMonthsDest, lineBPerMonthsStr);
    return true;
}

bool UctoHelper::modifySignature(bool payerSigned)
{
    int linePayerSigned = mDocument.findLine(mPatternSignature);
    if (linePayerSigned == -1) {
        std::cerr << "Signature pattern did not found." << std::endl;
        return false;
    }

    QString linePayerSignedStr = mDocument.getLine(linePayerSigned);

    QString inVal = (payerSigned) ? mSignedText : mNotSignedText;
    int prefixLen;
    int sufixLen;
    QString prefix;
    QString sufix;
    if (!calculateAlignment(inVal, mPatternSignature, &prefixLen, &sufixLen)) {
        std::cerr << "Text alignment may break the table layout." << std::endl;
    }
    prefix.fill(' ', prefixLen);
    sufix.fill(' ', sufixLen);
    linePayerSignedStr.replace(mPatternSignature, prefix + inVal + sufix);
    mDocument.setLine(linePayerSigned, linePayerSignedStr);
    return true;
}

void UctoHelper::setSignaturePattern(const QString &pattern)
{
    mPatternSignature = pattern;
}

void UctoHelper::setBillingPeriodSourcePattern(const QString &pattern)
{
    mPatternBillPerSource = pattern;
}

void UctoHelper::setBillingPeriodDaySourcePattern(const QString &pattern)
{
    mPatternBillPerDaySource = pattern;
}

void UctoHelper::setBillingPeriodMonthSourcePattern(const QString &pattern)
{
    mPatternBillPerMonthSource = pattern;
}

void UctoHelper::setBillingPeriodYearSourcePattern(const QString &pattern)
{
    mPatternBillPerYearSource = pattern;
}

void UctoHelper::setBillingPeriodYearDestinationPattern(const QString &pattern)
{
    mPatternBillPerYearDest = pattern;
}

void UctoHelper::setBillingPeriodMonthsDestionationPattern(const QString &pattern)
{
    mPatternBillPerMonthsDest = pattern;
}

void UctoHelper::setBillingPeriodMonthsLinePattern(const QString &pattern)
{
    mPatternBillPerMonthsLine = pattern;
}

void UctoHelper::setBillingPeriodYearLinePattern(const QString &pattern)
{
    mPatternBillPerYearLine = pattern;
}

void UctoHelper::setSignedText(const QString &signedStr)
{
    mSignedText = signedStr;
}

void UctoHelper::setNotSignedText(const QString &notSignedStr)
{
    mNotSignedText = notSignedStr;
}

UctoDocument UctoHelper::getDocument() const
{
    return mDocument;
}

BillingPeriod UctoHelper::getBillingPeriod() const
{
    return mBillingPeriod;
}

UctoHelper::~UctoHelper()
{

}

