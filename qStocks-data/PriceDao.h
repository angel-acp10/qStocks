#ifndef PRICEDAO_H
#define PRICEDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class Price;

class PriceDao
{
public:
    explicit PriceDao(QSqlDatabase& db);
    void init() const;

    void add(Price& price) const;
    void update(const Price& price) const;
    void remove(int id) const;
    std::unique_ptr<std::vector<std::unique_ptr<Price>>> prices() const;

private:
    QSqlDatabase& mDb;
};

#endif // PRICEDAO_H
