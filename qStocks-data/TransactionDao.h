#ifndef TRANSACTIONDAO_H
#define TRANSACTIONDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class Transaction;

class TransactionDao
{
public:
    explicit TransactionDao(QSqlDatabase& db);
    void init() const;

    void add(Transaction& transaction) const;
    void update(const Transaction& transaction) const;
    void remove(int id) const;
    std::unique_ptr<std::vector<std::unique_ptr<Transaction>>> transactions() const;

private:
    QSqlDatabase& mDb;
};

#endif // TRANSACTIONDAO_H
