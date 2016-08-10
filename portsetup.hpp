#ifndef PORTSETUP_HPP
#define PORTSETUP_HPP


#include <QList>
#include <QString>


class PortSetup
{
    QStringList mPorts;
    QList<qint32> mListBoudRates;
    QList<qint32> mDataBits;
    QList<qint32> mParity;
    QList<qint32> mStopBits;
    QList<qint32> mFlowControl;

public:
    PortSetup();

    qint32 getIndexBoudRates(const QString str);
    qint32 getIndexBoudRates(const qint32 str);
    QString getStringBuodRates(const qint32 index);


};

#endif // PORTSETUP_HPP
