#ifndef CURRENCYDAO_H
#define CURRENCYDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class Currency;

class CurrencyDao
{
public:
    explicit CurrencyDao(QSqlDatabase& db);
    void init() const;

    void add(Currency& currency) const;
    void update(const Currency& currency) const;
    void remove(int id) const;
    std::unique_ptr<std::vector<std::unique_ptr<Currency>>> currencies() const;

private:
    QSqlDatabase& mDb;
};

#endif // CURRENCYDAO_H
