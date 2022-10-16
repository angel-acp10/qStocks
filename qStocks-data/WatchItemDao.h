#ifndef WATCHITEMDAO_H
#define WATCHITEMDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class WatchItem;

class WatchItemDao
{
public:
    explicit WatchItemDao(QSqlDatabase& db);
    void init() const;

    void add(WatchItem& WatchItem) const;
    void update(const WatchItem& WatchItem) const;
    void remove(int id) const;
    std::unique_ptr<std::vector<std::unique_ptr<WatchItem>>> watchItems() const;

private:
    QSqlDatabase& mDb;
};

#endif // WATCHITEMDAO_H
