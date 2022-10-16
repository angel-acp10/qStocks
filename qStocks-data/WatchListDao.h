#ifndef WATCHLISTDAO_H
#define WATCHLISTDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class WatchList;

class WatchListDao
{
public:
    explicit WatchListDao(QSqlDatabase& db);
    void init() const;

    void add(WatchList& watchList) const;
    void update(const WatchList& watchList) const;
    void remove(int id) const;
    std::unique_ptr<std::vector<std::unique_ptr<WatchList>>> watchLists() const;

private:
    QSqlDatabase& mDb;
};

#endif // WATCHLISTDAO_H
