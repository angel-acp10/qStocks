#ifndef APIDAO_H
#define APIDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class Api;

class ApiDao
{
public:
    explicit ApiDao(QSqlDatabase& db);
    void init() const;

    void add(Api& api) const;
    void update(const Api& api) const;
    void remove(int id) const;
    std::unique_ptr<std::vector<std::unique_ptr<Api>>> apis() const;

private:
    QSqlDatabase& mDb;
};

#endif // APIDAO_H
