#ifndef BROKERDAO_H
#define BROKERDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class Broker;

class BrokerDao
{
public:
    explicit BrokerDao(QSqlDatabase& db);
    void init() const;

    void add(Broker& broker) const;
    void update(const Broker& broker) const;
    void remove(int id) const;
    std::unique_ptr<std::vector<std::unique_ptr<Broker>>> brokers() const;

private:
    QSqlDatabase& mDb;
};

#endif // BROKERDAO_H
