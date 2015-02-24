#ifndef UCTOHELPER_H
#define UCTOHELPER_H

#include <uctodocument.h>
#include <billingperiod.h>
#include <QString>

class UctoHelper
{
private:
    QString mPatternSignature;
    QString mPatternBillPerSource;
    QString mPatternBillPerDaySource;
    QString mPatternBillPerMonthSource;
    QString mPatternBillPerYearSource;
    QString mPatternBillPerYearDest;
    QString mPatternBillPerMonthsDest;
    QString mPatternBillPerYearLine;
    QString mPatternBillPerMonthsLine;

    QString mSignedText;
    QString mNotSignedText;

    UctoDocument mDocument;
    BillingPeriod mBillingPeriod;

    bool calculateAlignment(const QString &newVal, const QString &oldVal, int *prefixLen, int *suffixLen) const;
public:
    UctoHelper();
    bool openFile(const QString &fileName);
    bool saveFile(const QString &fileName) const;
    bool retrieveBillinPeriod();
    bool includeBillingPeriodYear();
    bool includeBillingPeriodMonths();
    bool modifySignature(bool payerSigned);
    BillingPeriod* getBillingPeriod() const;
    void setSignaturePattern(const QString &pattern);
    void setBillingPeriodSourcePattern(const QString &pattern);
    void setBillingPeriodDaySourcePattern(const QString &pattern);
    void setBillingPeriodMonthSourcePattern(const QString &pattern);
    void setBillingPeriodYearSourcePattern(const QString &pattern);
    void setBillingPeriodYearDestinationPattern(const QString &pattern);
    void setBillingPeriodMonthsDestionationPattern(const QString &pattern);
    void setBillingPeriodMonthsLinePattern(const QString &pattern);
    void setBillingPeriodYearLinePattern(const QString &pattern);
    void setSignedText(const QString &signedStr);
    void setNotSignedText(const QString &notSignedStr);
    ~UctoHelper();
};

#endif // UCTOHELPER_H
