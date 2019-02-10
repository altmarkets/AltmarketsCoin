// Copyright (c) 2009-2018 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2016-2018 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Altmarketsunits.h"

#include "chainparams.h"
#include "primitives/transaction.h"

#include <QSettings>
#include <QStringList>

AltmarketsUnits::AltmarketsUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<AltmarketsUnits::Unit> AltmarketsUnits::availableUnits()
{
    QList<AltmarketsUnits::Unit> unitlist;
    unitlist.append(ALTM);
    unitlist.append(mALTM);
    unitlist.append(uALTM);
    return unitlist;
}

bool AltmarketsUnits::valid(int unit)
{
    switch(unit)
    {
    case ALTM:
    case mALTM:
    case uALTM:
        return true;
    default:
        return false;
    }
}

QString AltmarketsUnits::id(int unit)
{
    switch(unit)
    {
    case ALTM: return QString("altm");
    case mALTM: return QString("maltm");
    case uALTM: return QString("ualtm");
    default: return QString("???");
    }
}

QString AltmarketsUnits::name(int unit)
{
    if(Params().NetworkIDString() == CBaseChainParams::MAIN)
    {
        switch(unit)
        {
        case ALTM: return QString("ALTM");
        case mALTM: return QString("mALTM");
        case uALTM: return QString::fromUtf8("μALTM");
        default: return QString("???");
        }
    }
    else
    {
        switch(unit)
        {
            case ALTM: return QString("tALTM");
            case mALTM: return QString("mtALTM");
            case uALTM: return QString::fromUtf8("μtALTM");
            default: return QString("???");
        }
    }
}

QString AltmarketsUnits::description(int unit)
{
    if(Params().NetworkIDString() == CBaseChainParams::MAIN)
    {
        switch(unit)
        {
        case ALTM: return QString("Altmarkets");
        case mALTM: return QString("Milli-Altmarkets (1 / 1" THIN_SP_UTF8 "000)");
        case uALTM: return QString("Micro-Altmarkets (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
        default: return QString("???");
        }
    }
    else
    {
        switch(unit)
        {
            case ALTM: return QString("TestAltmarkets");
            case mALTM: return QString("Milli-TestAltmarkets (1 / 1" THIN_SP_UTF8 "000)");
            case uALTM: return QString("Micro-TestAltmarkets (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
            default: return QString("???");
        }
    }
}

qint64 AltmarketsUnits::factor(int unit)
{
    switch(unit)
    {
    case ALTM:  return 100000000;
    case mALTM: return 100000;
    case uALTM: return 100;
    default:   return 100000000;
    }
}

int AltmarketsUnits::decimals(int unit)
{
    switch(unit)
    {
    case ALTM: return 8;
    case mALTM: return 5;
    case uALTM: return 2;
    default: return 0;
    }
}

QString AltmarketsUnits::format(int unit, const CAmount& nIn, bool fPlus, SeparatorStyle separators)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 n = (qint64)nIn;
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    qint64 remainder = n_abs % coin;
    QString quotient_str = QString::number(quotient);
    QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');

    // Use SI-style thin space separators as these are locale independent and can't be
    // confused with the decimal marker.
    QChar thin_sp(THIN_SP_CP);
    int q_size = quotient_str.size();
    if (separators == separatorAlways || (separators == separatorStandard && q_size > 4))
        for (int i = 3; i < q_size; i += 3)
            quotient_str.insert(q_size - i, thin_sp);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');

    if (num_decimals <= 0)
        return quotient_str;

    return quotient_str + QString(".") + remainder_str;
}


// TODO: Review all remaining calls to AltmarketsUnits::formatWithUnit to
// TODO: determine whether the output is used in a plain text context
// TODO: or an HTML context (and replace with
// TODO: AltmarketsUnits::formatHtmlWithUnit in the latter case). Hopefully
// TODO: there aren't instances where the result could be used in
// TODO: either context.

// NOTE: Using formatWithUnit in an HTML context risks wrapping
// quantities at the thousands separator. More subtly, it also results
// in a standard space rather than a thin space, due to a bug in Qt's
// XML whitespace canonicalisation
//
// Please take care to use formatHtmlWithUnit instead, when
// appropriate.

QString AltmarketsUnits::formatWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    return format(unit, amount, plussign, separators) + QString(" ") + name(unit);
}

QString AltmarketsUnits::formatHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(formatWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}

QString AltmarketsUnits::floorWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QSettings settings;
    int digits = settings.value("digits").toInt();

    QString result = format(unit, amount, plussign, separators);
    if(decimals(unit) > digits) result.chop(decimals(unit) - digits);

    return result + QString(" ") + name(unit);
}

QString AltmarketsUnits::floorHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(floorWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}

bool AltmarketsUnits::parse(int unit, const QString &value, CAmount *val_out)
{
    if(!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);

    // Ignore spaces and thin spaces when parsing
    QStringList parts = removeSpaces(value).split(".");

    if(parts.size() > 2)
    {
        return false; // More than one dot
    }
    QString whole = parts[0];
    QString decimals;

    if(parts.size() > 1)
    {
        decimals = parts[1];
    }
    if(decimals.size() > num_decimals)
    {
        return false; // Exceeds max precision
    }
    bool ok = false;
    QString str = whole + decimals.leftJustified(num_decimals, '0');

    if(str.size() > 18)
    {
        return false; // Longer numbers will exceed 63 bits
    }
    CAmount retvalue(str.toLongLong(&ok));
    if(val_out)
    {
        *val_out = retvalue;
    }
    return ok;
}

QString AltmarketsUnits::getAmountColumnTitle(int unit)
{
    QString amountTitle = QObject::tr("Amount");
    if (AltmarketsUnits::valid(unit))
    {
        amountTitle += " ("+AltmarketsUnits::name(unit) + ")";
    }
    return amountTitle;
}

int AltmarketsUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant AltmarketsUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}

CAmount AltmarketsUnits::maxMoney()
{
    return MAX_MONEY;
}
