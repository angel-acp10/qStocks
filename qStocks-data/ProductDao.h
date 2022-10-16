#ifndef PRODUCTDAO_H
#define PRODUCTDAO_H

#include <memory>
#include <vector>

class QSqlDatabase;
class Product;

class ProductDao
{
public:
    explicit ProductDao(QSqlDatabase& db);
    void init() const;

    void add(Product& product) const;
    void update(const Product& product) const;
    void remove(int id) const;
    std::unique_ptr<std::vector<std::unique_ptr<Product>>> products() const;

private:
    QSqlDatabase& mDb;
};

#endif // PRODUCTDAO_H
