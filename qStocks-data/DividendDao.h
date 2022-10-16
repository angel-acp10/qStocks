#ifndef DIVIDENDDAO_H
#define DIVIDENDDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class Dividend;

class DividendDao
{
public:
    explicit DividendDao(QSqlDatabase& db);
    void init() const;

    void add(Dividend& dividend) const;
    void update(const Dividend& dividend) const;
    void remove(int id) const;
    std::unique_ptr<std::vector<std::unique_ptr<Dividend>>> dividends() const;

private:
    QSqlDatabase& mDb;
};

#endif // DIVIDENDDAO_H
