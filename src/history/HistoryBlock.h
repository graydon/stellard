#ifndef __HISTORYBLOCK__
#define __HISTORYBLOCK__

using namespace ripple;

namespace stellar
{

    class HistoryBlock
    {
        /**
         * The ledger history is kept in a K-V store as a sequence of HistoryBlocks. At rest, a
         * HistoryBlock is a transmission-ready serialized object holding one ledger-header and a
         * sequence of all the transactions that occurred in that ledger, in the order they were
         * applied. In memory, it is deserialized to the in-memory forms of each of those objects
         * (ledger header and transaction sequence) respectively; but if you "just want to send what
         * happened in ledger #N:HASH" to another peer, it suffices to read the Blob for
         * HistoryBlock #N:HASH and send it, no transformation required.
         *
         * Given a starting account-state and a sequence of HistoryBlocks, a peer should be able to
         * replay the HistoryBlocks and reconstruct the account-state of every ledger along the way.
         *
         * The K-V store may also, in the future, store larger contiguous runs of HistoryBlocks in a
         * separate table, flat file or database, in order to further amortize framing and I/O
         * overheads. But for now we're going with one HistoryBlock per ledger.
         *
         * A HistoryBlock ought to be composed and saved for any ledger that is closed, before its
         * closure is announced publicly. It is the "canonical history" from which all other
         * histories are constructed, so it should be most-aggressively and cautiously preserved.
         */

    public:
        typedef std::shared_ptr<HistoryBlock> pointer;

        HistoryBlock(Ledger::pointer ledger,
                     std::vector<Transaction::pointer> const& transactions)
            : mLedger (ledger),
              mTransactions (transactions);
            {}

        Ledger::pointer getLedger() const { return mLedger; }
        Transaction::pointer getTransactions() const; { return mTransactions; }

        HistoryBlock(Blob const& b);
        void serialize (Blob& b) const;

        /**
         * Apply a HistoryBlock to a CLF. Precondition is that the CLF has the
         * same value as this HistoryBlock's parent.
         */
        void applyTransactions (CanonicalLedgerForm::pointer ledger) const;

    private:
        Ledger::pointer mLedger;
        std::vector<Transaction::pointer> mTransactions;
    };
}

#endif
