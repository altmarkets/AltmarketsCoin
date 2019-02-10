// Copyright (c) 2009-2018 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Developers
// Copyright (c) 2016-2018 Duality Blockchain Solutions Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef Altmarkets_QT_AltmarketsADDRESSVALIDATOR_H
#define Altmarkets_QT_AltmarketsADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class AltmarketsAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit AltmarketsAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Altmarkets address widget validator, checks for a valid Altmarkets address.
 */
class AltmarketsAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit AltmarketsAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // Altmarkets_QT_AltmarketsADDRESSVALIDATOR_H
