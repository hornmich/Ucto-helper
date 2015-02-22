#ifndef BILLINGPERIOD_H
#define BILLINGPERIOD_H

#include<QString>
#include<QRegExp>
#include<iostream>

class BillingPeriod
{
private:
    static const char* TAG;
    unsigned short mStartDay;
    unsigned short mStartMonth;
    unsigned short mStartYear;
    unsigned short mEndDay;
    unsigned short mEndMonth;
    unsigned short mEndYear;
    bool mPeriodFound;
public:
    BillingPeriod();
    ~BillingPeriod();
    bool findPeriod(const QString &source, const QString &dayRegExpStr = "\\d{2}\\.", const QString &monthRegExpStr = "\\d{2}\\.", const QString &yearRegExpStr = "\\d{4}");
    unsigned short startDay() const;
    unsigned short startMonth() const;
    unsigned short startYear() const;
    unsigned short endDay() const;
    unsigned short endMonth() const;
    unsigned short endYear() const;
    bool isPeriodFound() const;
};

#endif // BILLINGPERIOD_H
