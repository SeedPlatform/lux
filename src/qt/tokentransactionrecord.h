#ifndef BITCOIN_QT_TOKENTRANSACTIONRECORD_H
#define BITCOIN_QT_TOKENTRANSACTIONRECORD_H

#include "amount.h"
#include "uint256.h"

#include <QList>
#include <QString>

class CWallet;
class CTokenTx;

/** UI model for token transaction status. The token transaction status is the part of a token transaction that will change over time.
 */
class TokenTransactionStatus
{
public:
    TokenTransactionStatus():
        countsForBalance(false), sortKey(""),
        status(Offline), depth(0), cur_num_blocks(-1)
    { }

    enum Status {
        Confirmed,          /**< Have 6 or more confirmations (normal tx) or fully mature (mined tx) **/
        /// Normal (sent/received) token transactions
        Offline,            /**< Not sent to any other nodes **/
        Unconfirmed,        /**< Not yet mined into a block **/
        Confirming         /**< Confirmed, but waiting for the recommended number of confirmations **/
    };

    /// Token transaction counts towards available balance
    bool countsForBalance;
    /// Sorting key based on status
    std::string sortKey;

    /** @name Reported status
       @{*/
    Status status;
    qint64 depth;

    /**@}*/

    /** Current number of blocks (to know whether cached status is still valid) */
    int cur_num_blocks;
};

/** UI model for a token transaction. A core token transaction can be represented by multiple UI token transactions if it has
    multiple outputs.
 */
class TokenTransactionRecord
{
public:
    enum Type
    {
        Other,
        SendToAddress,
        SendToOther,
        RecvWithAddress,
        RecvFromOther,
        SendToSelf
    };

    /** Number of confirmation recommended for accepting a token transaction */
    static const int RecommendedNumConfirmations = 10;

    TokenTransactionRecord():
            hash(), time(0), type(Other), address(""), debit(0), credit(0)
    {
    }

    TokenTransactionRecord(uint256 _hash, qint64 _time):
            hash(_hash), time(_time), type(Other), address(""), debit(0),
            credit(0)
    {
    }

    TokenTransactionRecord(uint256 _hash, qint64 _time,
                Type _type, const std::string &_address,
                const CAmount& _debit, const CAmount& _credit):
            hash(_hash), time(_time), type(_type), address(_address), debit(_debit), credit(_credit)
    {
    }

    /** Decompose Token transaction into a record.
     */
    static QList<TokenTransactionRecord> decomposeTransaction(const CWallet *wallet, const CTokenTx &wtx);

    /** @name Immutable token transaction attributes
      @{*/
    uint256 hash;
    qint64 time;
    Type type;
    std::string address;
    CAmount debit;
    CAmount credit;
    /**@}*/

    QString getTxID() const;

    /** Status: can change with block chain update */
    TokenTransactionStatus status;

    /** Update status from core wallet tx.
     */
    void updateStatus(const CTokenTx &wtx);

    /** Return whether a status update is needed.
     */
    bool statusUpdateNeeded();
};

#endif // BITCOIN_QT_TRANSACTIONRECORD_H