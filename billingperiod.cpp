#include "billingperiod.h"

unsigned short BillingPeriod::startDay() const
{
    return mStartDay;
}

unsigned short BillingPeriod::startMonth() const
{
    return mStartMonth;
}

unsigned short BillingPeriod::startYear() const
{
    return mStartYear;
}

unsigned short BillingPeriod::endDay() const
{
    return mEndDay;
}

unsigned short BillingPeriod::endMonth() const
{
    return mEndMonth;
}

unsigned short BillingPeriod::endYear() const
{
    return mEndYear;
}

QString BillingPeriod::startDayStr() const
{
    QString num;
    return num.number(mStartDay);
}

QString BillingPeriod::startMonthStr() const
{
    QString num;
    return num.number(mStartMonth);
}

QString BillingPeriod::startYearStr() const
{
    QString num;
    return num.number(mStartYear);
}

QString BillingPeriod::endDayStr() const
{
    QString num;
    return num.number(mEndDay);
}

QString BillingPeriod::endMonthStr() const
{
    QString num;
    return num.number(mEndMonth);
}

QString BillingPeriod::endYearStr() const
{
    QString num;
    return num.number(mEndYear);
}


bool BillingPeriod::isPeriodFound() const
{
    return mPeriodFound;
}

BillingPeriod::BillingPeriod()
{
    mPeriodFound = false;
}

BillingPeriod::BillingPeriod(const BillingPeriod &bp)
{
    mPeriodFound = bp.mPeriodFound;
    mStartDay = bp.mStartDay;
    mStartMonth = bp.mStartMonth;
    mStartYear = bp.mStartYear;
    mEndDay = bp.mEndDay;
    mEndMonth = mEndMonth;
    mEndYear = mEndYear;
}

BillingPeriod::~BillingPeriod()
{

}

bool BillingPeriod::findPeriod(const QString &source, const QString &dayRegExpStr, const QString &monthRegExpStr, const QString &yearRegExpStr)
{
    QRegExp dayRegExp, monthRegExp, yearRegExp;

    dayRegExp.setPattern(dayRegExpStr);
    monthRegExp.setPattern(monthRegExpStr);
    yearRegExp.setPattern(yearRegExpStr);

    int dayIndex = dayRegExp.indexIn(source, 0);
    int monthIndex = monthRegExp.indexIn(source, dayIndex+3);
    int yearIndex = yearRegExp.indexIn(source, monthIndex+3);
    int dayIndex2 = dayRegExp.indexIn(source, yearIndex+4);
    int monthIndex2 = monthRegExp.indexIn(source, dayIndex2+3);
    int yearIndex2 = yearRegExp.indexIn(source, monthIndex2+3);

    if (dayIndex == -1 ||
            monthIndex == -1 ||
            yearIndex == -1 ||
            dayIndex2 == -1 ||
            monthIndex2 == -1 ||
            yearIndex2 == -1) {
        std::cerr << "BillingPeriod.findPeriod: Regular expression did not match for " << source.toStdString() << "." << std::endl;
        mPeriodFound = false;
        return false;
    }

    QString startDayStr = source.mid(dayIndex, 2);
    QString startMonthStr = source.mid(monthIndex, 2);
    QString startYearStr = source.mid(yearIndex, 4);
    QString endDayStr = source.mid(dayIndex2, 2);
    QString endMonthStr = source.mid(monthIndex2, 2);
    QString endYearStr = source.mid(yearIndex2, 4);

    if (startDayStr == NULL ||
            startMonthStr == NULL ||
            startYearStr == NULL ||
            endDayStr == NULL ||
            endMonthStr == NULL ||
            endYearStr == NULL) {
        std::cerr << "BillingPeriod.findPeriod: Substring extraction failed for " << source.toStdString() << "." << std::endl;
        mPeriodFound = false;
        return false;
    }

    bool strToIntOK = true;
    mStartDay = startDayStr.toInt(&strToIntOK);
    mStartMonth = startMonthStr.toInt(&strToIntOK);
    mStartYear = startYearStr.toInt(&strToIntOK);
    mEndDay = endDayStr.toInt(&strToIntOK);
    mEndMonth = endMonthStr.toInt(&strToIntOK);
    mEndYear = endYearStr.toInt(&strToIntOK);

    if (strToIntOK == false) {
        std::cerr << "BillingPeriod.findPeriod: String to Integer conversion failed for " << source.toStdString() << "." << std::endl;
        mPeriodFound = false;
        return false;
    }

    mPeriodFound = true;
    return true;
}
