#ifndef SPLITDAO_H
#define SPLITDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class Split;

class SplitDao
{
public:
    explicit SplitDao(QSqlDatabase& db);
    void init() const;

    void add(Split& split) const;
    void update(const Split& split) const;
    void remove(int id) const;
    std::unique_ptr<std::vector<std::unique_ptr<Split>>> splits() const;

private:
    QSqlDatabase& mDb;
};

#endif // SPLITDAO_H
