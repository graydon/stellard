#include "HistoryBlock.h"

using namespace ripple;
namespace stellar
{

HistoryBlock::HistoryBlock(Ledger::Pointer ledger,
                           std::vector<Transaction>& transactions)
    : mLedgerHeader (ledger),
    , mTransactions (transactions)
{
}

HistoryBlock::HistoryBlock(Blob const& b)
{
    Serializer ser (b);
    SerializerIterator sit (ser);

    Blob ledgerBlob (sit.getVL ());
    Blob txBlob (sit.getVL ());

    mLedgerHeader = std::make_shared<Ledger> (ledgerHash);

    uint256 transHash = Serializer (txBlob).getSHA512Half ();
    require_equal (mLedgerHeader->getTransHash (), transHash,
                   "transaction hash");

    Serializer txSer (txsBlob);
    SerializerIterator txSit (txSer);

    while (! txSit.empty ())
    {
        mTransactions.emplace_back (std::make_shared<Transaction> (txSit.getVL ()));
    }
}

void
HistoryBlock::serialize(Blob& b)
{
    Blob headerBlob;
    ledger->serialize (headerBlob);

    Blob txBlob;
    Serializer txSer
    for (auto const& t : mTransactions)
    {
        Blob tBlob;
        t.serialize (tBlob);
        txSer.addVL (tBlob);
    }

    uint256 transHash = txSer.getSHA512Half ();
    require_equal (mLedgerHeader->getTransHash (), transHash,
                   "transaction hash");

    b.clear ();
    Serializer ser (b);
    ser.addVL (headerBlob);
    set.addVL (txBlob);
}


void
HistoryBlock::applyTransactions (CanonicalLedgerForm::pointer ledger) const;
{
    TransactionEngine engine (ledger);
    TransactionEngineParams parms (tapNONE);
    for (auto const& t : mTransactions)
    {
        bool didApply;
        TER result = engine.applyTransaction (t, parms, didApply);
        if (! didApply)
        {
            throw std::runtime_error("filed to apply txn");
        }
    }
}
#endif
