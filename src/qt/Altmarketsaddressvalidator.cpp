// Copyright (c) 2009-2018 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2016-2018 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Altmarketsaddressvalidator.h"

#include "base58.h"
#include "pubkey.h"
#include "utilstrencodings.h"

/* Base58 characters are:
     "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"

  This is:
  - All numbers except for '0'
  - All upper-case letters except for 'I' and 'O'
  - All lower-case letters except for 'l'
*/

AltmarketsAddressEntryValidator::AltmarketsAddressEntryValidator(QObject *parent) :
    QValidator(parent)
{
}

QValidator::State AltmarketsAddressEntryValidator::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos);

    // Empty address is "intermediate" input
    if (input.isEmpty())
        return QValidator::Intermediate;

    // Correction
    for (int idx = 0; idx < input.size();)
    {
        bool removeChar = false;
        QChar ch = input.at(idx);
        // Corrections made are very conservative on purpose, to avoid
        // users unexpectedly getting away with typos that would normally
        // be detected, and thus sending to the wrong address.
        switch(ch.unicode())
        {
        // Qt categorizes these as "Other_Format" not "Separator_Space"
        case 0x200B: // ZERO WIDTH SPACE
        case 0xFEFF: // ZERO WIDTH NO-BREAK SPACE
            removeChar = true;
            break;
        default:
            break;
        }

        // Remove whitespace
        if (ch.isSpace())
            removeChar = true;

        // To next character
        if (removeChar)
            input.remove(idx, 1);
        else
            ++idx;
    }

    // Validation
    QValidator::State state = QValidator::Acceptable;
    for (int idx = 0; idx < input.size(); ++idx)
    {
        int ch = input.at(idx).unicode();

        if (((ch >= '0' && ch<='9') ||
            (ch >= 'a' && ch<='z') ||
            (ch >= 'A' && ch<='Z')))
        {
            // Alphanumeric
        }
        else
        {
            state = QValidator::Invalid;
        }
    }

    return state;
}

AltmarketsAddressCheckValidator::AltmarketsAddressCheckValidator(QObject *parent) :
    QValidator(parent)
{
}

QValidator::State AltmarketsAddressCheckValidator::validate(QString &input, int &pos) const
{
    Q_UNUSED(pos);
    // Validate the passed Altmarkets address
    const std::string strInput = input.toStdString();
    CAltmarketsAddress addr(strInput);
    if (addr.IsValid())
        return QValidator::Acceptable;

    // Validate the passed Altmarkets public key
    if (IsHex(strInput))
    {
        CPubKey vchPubKey(ParseHex(strInput));
        if (vchPubKey.IsFullyValid())
            return QValidator::Acceptable;
    }

    return QValidator::Invalid;
}